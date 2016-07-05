#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sig_alrm(int signo) {
//  printf("in the function sig_alrm.\n");
}

int main(void) {
  int n;
  char line[1024];


  if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
    printf("signal error.\n");
    exit(-1);
  }

  alarm(10);
  if ((n = read(STDIN_FILENO, line, 1024)) < 0) {
    printf("read error.\n");
    exit(-1);
  }
  alarm(0);
  write(STDOUT_FILENO, line, n);
  exit(0);
}

