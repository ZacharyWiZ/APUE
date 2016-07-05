#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <signal.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo) {
  printf("sig_alrm starting.\n");
  longjmp(env_alrm, 1);
  //sleep(3);
  printf("sig_alrm finished.\n");
}

static void sig_int(int signo) {
  int i, j;
  volatile int k;
  /*
   * Tune these loops to run for more than 5 seconds
   *  on whatever system this test program is run
   */
  printf("\nsig_int starting.\n");
  for (i=0; i<300000; i++)
    for (j = 0; j<4000; j++)
      k+= i*j;
  printf("sig_int finished.\n");
}

unsigned int isleep(unsigned int seconds) {

  if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    return (seconds);
    if (setjmp(env_alrm) == 0) {
      alarm(seconds);
      pause();
      printf("pause get the sig.\n");
    }
  return (alarm(0));
}

int main(void) {
  int seconds;
  int remain_s;

  if (signal(SIGINT, sig_int) == SIG_ERR) {
    printf("signal(SIGINT) error.\n");
    exit(-1);
  }

  printf("Please cin the seconds which u want to sleep:\n");
  scanf("%d", &seconds);

  remain_s = isleep(seconds);
  printf("after sleep remain seconds: %d.\n", remain_s);
  sleep(11);
  exit(0);
}
