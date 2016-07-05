#include <sys/wait.h>
#include <stdio.h>

static void sig_int(int signo) {
  printf("interrupt \n%% ");
  exit(0);
}

int main(void) {
  if (signal(SIGINT, sig_int) == SIG_ERR) {
    perror("signal error.");
  }
  printf("%%\n");
  while (1);
  return 0;
}
