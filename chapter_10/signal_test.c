#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static void sig_usr(int);

int main(void) {
  if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
    printf("can't catch SIGUSR1.\n");
    exit(-1);
  }

  if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
    printf("can't catch SIGUSR2.\n");
    exit(-1);
  }

  for ( ; ; ) {
    printf("heihei.\n");
    sleep(10);
  }
  exit(0);
}

static void sig_usr(int signo) { /* argument is a signal number */
  if (signo == SIGUSR1)
    printf("received SIGUSR1.\n");
  else if (signo == SIGUSR2) 
    printf("received SIGUSR2.\n");
  else {
    printf("received signal : %d.\n", signo);
  }
}
