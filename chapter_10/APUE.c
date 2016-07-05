#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

int err_sys(const char *fmt, ...) {
  va_list argp;
  char buf[512] = {0};
  
  va_start(argp, fmt);
  vsprintf(buf, fmt, argp);
  va_end(argp);

  printf("%s", buf);
  exit(-1);
}

void pr_mask(const char *str) {
  sigset_t sigset;
  int errno_save; 
       
  errno_save = errno;   /* we can be called by signal handlers */
  if (sigprocmask(0, NULL, &sigset) < 0) {
    printf("sigprocmask error.\n");
  } else {
    printf("%s", str);
    if (sigismember(&sigset, SIGINT)) 
      printf(" SIGINT");
    if (sigismember(&sigset, SIGQUIT))
      printf(" SIGQUIT");
    if (sigismember(&sigset, SIGUSR1))
      printf(" SIGUSR1");
    if (sigismember(&sigset, SIGUSR2))
      printf(" SIGUSR2");
    if (sigismember(&sigset, SIGALRM))
      printf(" SIGALRM");

    printf(".\n");
  }
  errno = errno_save;
}

