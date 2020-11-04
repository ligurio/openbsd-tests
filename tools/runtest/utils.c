#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <poll.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <err.h>
#include <fnmatch.h>
#include <fts.h>
#include <errno.h>

#include "test_list.h"
#include "utils.h"

#define TIME_BUF_SIZE 1024
#define WAIT_CHILD_POLL_TIMEOUT_MS 200
#define WAIT_CHILD_BUF_MAX_SIZE 1024

static inline char *format_time(char *strtime, size_t size) {
  time_t t;
  struct tm *lt;

  t = time(NULL);
  lt = localtime(&t);
  strftime(strtime, size, "%Y-%m-%dT%H:%M", lt);

  return strtime;
}

/* Compare files by name. */
int entcmp(const FTSENT **a, const FTSENT **b)
{
  return strcmp((*a)->fts_name, (*b)->fts_name);
}
 
/*
 * Print all files in the directory tree that match the glob pattern.
 * Example: test_discovery("/usr/src/test", "Makefile");
 */
struct test_list *test_discovery(char *dir, const char *pattern) {
  struct test_list *tests;
  tests = calloc(1, sizeof(struct test_list));
  if (tests == NULL) {
    fprintf(stderr, "malloc failed");
    return NULL;
  }
  TAILQ_INIT(tests);

  FTS *tree;
  FTSENT *f;
  char *argv[] = { dir, NULL };

  /*
   * FTS_LOGICAL follows symbolic links, including links to other
   * directories. It detects cycles, so we never have an infinite
   * loop. FTS_NOSTAT is because we never use f->statp. It uses
   * our entcmp() to sort files by name.
   */
  tree = fts_open(argv, FTS_LOGICAL | FTS_NOSTAT, entcmp);
  if (tree == NULL)
    err(1, "fts_open");
 
  while ((f = fts_read(tree))) {
    switch (f->fts_info) {
    case FTS_DNR:	/* Cannot read directory */
    case FTS_ERR:	/* Miscellaneous error */
    case FTS_NS:	/* stat() error */
      /* Show error, then continue to next files. */
      warn("%s", f->fts_path);
      continue;
    case FTS_DP:
      /* Ignore post-order visit to directory. */
      continue;
    }

    if (fnmatch(pattern, f->fts_name, FNM_PERIOD) == 0) {
      struct test *t = NULL;
      t = calloc(1, sizeof(struct test));
      if (t == NULL) {
        fprintf(stderr, "malloc failed");
        break;
      }
      t->name = dirname(strdup(f->fts_path));
      t->path = strdup(f->fts_path);
      TAILQ_INSERT_TAIL(tests, t, entries);
    }

    /*
     * A cycle happens when a symbolic link (or perhaps a
     * hard link) puts a directory inside itself. Tell user
     * when this happens.
     */
    if (f->fts_info == FTS_DC) {
      warnx("%s: cycle in directory tree", f->fts_path);
    }
  }
 
  /* fts_read() sets errno = 0 unless it has error. */
  if (errno != 0)
    err(1, "fts_read");

  if (fts_close(tree) < 0)
    err(1, "fts_close");

  return tests;
}

int print_tests(struct test_list *tests, FILE *fp) {
  if (tests == NULL || test_list_length(tests) <= 0) {
    fprintf(fp, MSG_TESTS_NOT_FOUND);
    return 1;
  } else {
    fprintf(fp, MSG_TESTS_AVAILABLE);
    struct test *t = NULL;
    TAILQ_FOREACH(t, tests, entries) {
      if (t->status != STATUS_SKIP)
        fprintf(fp, "%s\n", t->name);
    }

    return 0;
  }
}

static inline void run_child(char *run_test, int fd_stdout, int fd_stderr) {
  if (chdir(dirname(strdup(run_test))) == -1) {
    exit(-1);
  }

  char make_bin[] = "/usr/bin/make";
  char make_opt[] = "-C";
  char *argv[] = {make_bin, make_opt, run_test, NULL};
  int rc = execv(argv[0], argv);

  dup2(fd_stdout, STDOUT_FILENO);
  dup2(fd_stderr, STDERR_FILENO);

  exit(rc);
}

static inline int wait_child(const char *test_dir, const char *run_test,
                             pid_t pid, int timeout, int *fds, FILE **fps) {
  struct pollfd pfds[2];
  struct timespec sentinel;
  clockid_t clock = CLOCK_MONOTONIC;
  int r;

  int timeouted = 0;
  int status;
  int waitflags;

  pfds[0].fd = fds[0];
  pfds[0].events = POLLIN;
  pfds[1].fd = fds[1];
  pfds[1].events = POLLIN;

  if (clock_gettime(clock, &sentinel) == -1) {
    clock = CLOCK_REALTIME;
    clock_gettime(clock, &sentinel);
  }

  while (1) {
    waitflags = WNOHANG;

    r = poll(pfds, 2, WAIT_CHILD_POLL_TIMEOUT_MS);
    if (r > 0) {
      char buf[WAIT_CHILD_BUF_MAX_SIZE];
      ssize_t n;

      if (pfds[0].revents != 0) {
        while ((n = read(fds[0], buf, WAIT_CHILD_BUF_MAX_SIZE)) > 0)
          fwrite(buf, n, 1, fps[0]);
      }

      if (pfds[1].revents != 0) {
        while ((n = read(fds[1], buf, WAIT_CHILD_BUF_MAX_SIZE)) > 0)
          fwrite(buf, n, 1, fps[1]);
      }

      clock_gettime(clock, &sentinel);
    } else if (timeout >= 0) {
      struct timespec time;

      clock_gettime(clock, &time);
      if ((time.tv_sec - sentinel.tv_sec) > timeout) {
        timeouted = 1;
        kill(pid, SIGKILL);
        waitflags = 0;
      }
    }

    if (waitpid(pid, &status, waitflags) == pid)
      break;
  }

  if (status) {
    fprintf(fps[0], "\nERROR: Exit status is %d\n", status);
    if (timeouted)
      fprintf(fps[0], "TIMEOUT: %s\n", test_dir);
  }

  return status;
}

int run_tests(struct test_list *tests, const struct test_options opts,
              const char *progname, FILE *fp_stdout, FILE *fp_stderr) {
  int rc = 0;
  char time[TIME_BUF_SIZE];

  pid_t child;
  int pipefd_stdout[2];
  int pipefd_stderr[2];

  do {
    if ((rc = pipe2(pipefd_stdout, O_NONBLOCK)) == -1)
      break;

    if ((rc = pipe2(pipefd_stderr, O_NONBLOCK)) == -1) {
      close(pipefd_stdout[0]);
      close(pipefd_stdout[1]);
      break;
    }

    struct test *t = NULL;
    TAILQ_FOREACH(t, tests, entries) {
      if (t->status == STATUS_SKIP) {
        fprintf(fp_stdout, "%s SKIPPED: %s\n", format_time(time, TIME_BUF_SIZE),
                t->name);
        continue;
      }
      char *test_dir = strdup(t->name);
      if (test_dir == NULL) {
        rc = -1;
        break;
      }

      child = fork();
      if (child == -1) {
        fprintf(fp_stdout, "ERROR: Fork %s\n", strerror(errno));
        rc = -1;
        break;
      } else if (child == 0) {
        run_child(t->name, pipefd_stdout[1], pipefd_stderr[1]);
      } else {
        int status;
        int fds[2];
        fds[0] = pipefd_stdout[0];
        fds[1] = pipefd_stderr[0];
        FILE *fps[2];
        fps[0] = fp_stdout;
        fps[1] = fp_stderr;

        fprintf(fp_stdout, "%s BEGIN: %s\n", format_time(time, TIME_BUF_SIZE),
                test_dir);

        status = wait_child(test_dir, t->name, child, opts.timeout, fds, fps);
        if (status)
          rc += 1;

        if (WIFEXITED(status)) {
          if (WEXITSTATUS(status) != 0) {
            t->status = STATUS_NOTOK;
          } else {
            t->status = STATUS_OK;
          }
        }

        fprintf(fp_stdout, "%s END: %s\n", format_time(time, TIME_BUF_SIZE),
                test_dir);
      }
    }

    close(pipefd_stdout[0]);
    close(pipefd_stdout[1]);
    close(pipefd_stderr[0]);
    close(pipefd_stderr[1]);
  } while (0);

  if (rc == -1)
    fprintf(fp_stderr, "run_tests fails: %s", strerror(errno));

  return rc;
}

int test_list_length(struct test_list *tests) {
  int num = 0;
  struct test *t = NULL;
  TAILQ_FOREACH(t, tests, entries) { num++; }
  return num;
}

void print_report(struct test_list *tests, FILE *fp) {

  int n = 1;
  fprintf(fp, "TAP version 13\n");
  fprintf(fp, "1..%d\n", test_list_length(tests));
  struct test *t = NULL;
  TAILQ_FOREACH(t, tests, entries) {
    dirname(t->name);
    if (t->status == STATUS_OK)
      fprintf(fp, "ok %d - %s\n", n, t->name);
    else if (t->status == STATUS_NOTOK)
      fprintf(fp, "not ok %d - %s\n", n, t->name);
    else if (t->status == STATUS_SKIP)
      fprintf(fp, "ok %d - %s # SKIP\n", n, t->name);
    n++;
  }
}

void filter_tests(struct test_list *tests, const char *filter) {
  regex_t reg;
  regmatch_t match[1];

  int r;
  r = regcomp(&reg, filter, REG_ICASE | REG_EXTENDED);
  if (r != 0) {
    fprintf(stderr, "%s\n", strerror(errno));
  }
  struct test *t = NULL;
  TAILQ_FOREACH(t, tests, entries) {
    r = regexec(&reg, t->name, 1, match, 0);
    if (r != 0) {
      t->status = STATUS_SKIP;
    }
  }
  regfree(&reg);
}
