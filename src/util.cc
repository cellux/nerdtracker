#include "util.h"

void LOG(const char *fmt, ...) {
  va_list argptr;
  va_start(argptr, fmt);
  vfprintf(stderr, fmt, argptr);
  fprintf(stderr, "\n");
  va_end(argptr);
}

void DIE(const char *fmt, ...) {
  va_list argptr;
  va_start(argptr, fmt);
  vfprintf(stderr, fmt, argptr);
  fprintf(stderr, "\n");
  va_end(argptr);
  exit(1);
}
