#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int err_sys(const char *fmt, ...) {
  va_list argp;
  char buf[512] = {0};
  
  va_start(argp, fmt);
  vsprintf(buf, fmt, argp);
  va_end(argp);

  printf("%s", buf);
  exit(-1);
}

