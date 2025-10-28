#include "../includes/ping.h"
#include <stdbool.h>

bool is_help(int argc, char **argv) {

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0) {
      return true;
    }
  }

  return false;
}

bool is_verbose(int argc, char **argv) {

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
      return true;
    }
  }

  return false;
}

bool is_valid_args(int argc, char **argv) {

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0 ||
          strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0) {
        return true;
      }
      return false;
    }
  }

  return true;
}

char *get_hostname(int argc, char **argv) {

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      return argv[i];
    }
  }

  return NULL;
}
