#ifndef _UTILS_H_
#define _UTILS_H_

#include "test_list.h"

#define MSG_TESTS_NOT_FOUND "No tests found.\n"
#define MSG_TESTS_AVAILABLE "Found tests:\n"

struct test_options {
  char *directory;
  char *filter;
  int list;
  int timeout;
  char **tests;
  char *report;
  char *exec_file;
};

extern struct test_list *test_discovery(char *, const char *);
extern int print_tests(struct test_list *, FILE *);
extern int run_tests(struct test_list *, const struct test_options,
                     const char *, FILE *, FILE *);
extern int test_list_length(struct test_list *tests);

extern void print_report(struct test_list *, FILE *);
void filter_tests(struct test_list *, const char *);

#endif /* _UTILS_H_ */
