#include <stdio.h>
#include <signal.h>
#include "err_sys.h"
#include <stdlib.h>

static void sig_quit(int);

int main(void) {
  sigset_t newmask, oldmask, pendmask;
  
  if (signal(SIGQUIT, sig_quit) == SIG_ERR) 
    err_sys("can't catch SIGQUIT.\n");

  sigemptyset(&newmask);
  sigaddset(&newmask, SIGQUIT);
  if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    err_sys("SIG_BLOCK error.\n");

  sleep(6);

  if (sigpending(&pendmask) < 0)
    err_sys("sigpending error.\n");
  if (sigismember(&pendmask, SIGQUIT))
    printf("\nSIGQUIT pending.\n");

  /*
   * Restore signal mask which unblocks SIGQUIT
   */
  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    err_sys("SIG_SETMASK error.\n");
  printf("SIGQUIT unblocked.\n");

  sleep(6);
  exit(0);
}

static void sig_quit(int signo) {
  printf("caught SIGQUIT.\n");
  if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
    err_sys("can't reset SIGQUIT.\n");
}
