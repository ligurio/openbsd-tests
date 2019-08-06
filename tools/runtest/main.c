#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define DEFAULT_DIRECTORY "/usr/src"
#define DEFAULT_TIMEOUT 300
#define DEFAULT_EXEC_FILE "Makefile"

static inline void print_usage(FILE *stream, char *progname) {
  fprintf(stream,
          "Usage: %s [-d directory] [-f filter] [-l list] [-t timeout]"
          " [-o report] [-h]\n",
          progname);
}

int main(int argc, char *argv[]) {
  int opt;
  int rc;

  struct test_list *tests = NULL;
  struct test_options opts;

  opts.directory = strdup(DEFAULT_DIRECTORY);
  opts.exec_file = strdup(DEFAULT_EXEC_FILE);
  opts.filter = NULL;
  opts.list = 0;
  opts.report = NULL;
  opts.tests = NULL;
  opts.timeout = DEFAULT_TIMEOUT;

  while ((opt = getopt(argc, argv, "d:f:lt:o:h")) != -1) {
    switch (opt) {
    case 'd':
      free(opts.directory);
      opts.directory = realpath(optarg, NULL);
      break;
    case 'f':
      opts.filter = strdup(optarg);
      break;
    case 'l':
      opts.list = 1;
      break;
    case 't':
      opts.timeout = atoi(optarg);
      break;
    case 'h':
      print_usage(stdout, argv[0]);
      exit(0);
      break;
    case 'o':
      opts.report = strdup(optarg);
      break;
    default:
      print_usage(stdout, argv[0]);
      exit(1);
      break;
    }
  }

  fprintf(stdout, "Searching tests in %s\n", opts.directory);
  tests = test_discovery(opts.directory, opts.exec_file);
  if (tests == NULL || test_list_length(tests) == 0) {
    fprintf(stderr, MSG_TESTS_NOT_FOUND);
    return 1;
  }

  if (opts.filter) {
    filter_tests(tests, opts.filter);
  }

  if (opts.list) {
    print_tests(tests, stdout);
    return 0;
  }

  rc = run_tests(tests, opts, argv[0], stdout, stderr);

  if (opts.report) {
    FILE *fp = NULL;
    if ((fp = fopen(opts.report, "w+")) == NULL)
      return 1;
    print_report(tests, fp);
  }

  free_tests(tests);

  return rc;
}
