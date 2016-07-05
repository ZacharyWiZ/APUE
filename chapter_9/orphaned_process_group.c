#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

static void sig_hup(int signo) {
  printf("SIGHUP received, pid = %ld.\n", (long)getpid());
}

static void pr_ids(char *name) {
  printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld.\n",
    name, (long)getpid(), (long)getppid(), (long)getpgrp(),
    (long)tcgetpgrp(STDIN_FILENO));
  fflush(stdout);
}

int main(void) {
  char  c;
  pid_t pid;
  
  pr_ids("parent");
  if ((pid = fork()) < 0) {
    printf("fork error.\n");
    exit(-1);
  } else if (pid > 0) { /* parent */
    sleep(5);           /* sleep to let child stop itself*/
  } else {              /* child */
    pr_ids("child");
    signal(SIGHUP, sig_hup);
    kill(getpid(), SIGTSTP);
    pr_ids("child");
    if (read(STDIN_FILENO, &c, 1) != 1) 
      printf("read error %d on controlling TTY. \n", errno);
  }
  exit(0);
}
