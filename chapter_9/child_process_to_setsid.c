/*
 * In the child process ,it setsid and make itself to the group leader, to test is it has tht controlling terminal.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


static void pr_ids(char *prefix) {
  printf("%s : pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld.\n",
    prefix, (long)getpid(), (long)getppid(), (long)getpgrp(),
    (long)tcgetpgrp(STDIN_FILENO));
  fflush(stdout);
}

int main(void) {
  pid_t pid;
  int result;  

  if ((pid = fork()) < 0) {
    printf("fork error.\n");
    exit(-1);
  } else if (pid == 0) {
    result = open("/dev/tty", 0);
    if (result < 0) {
      printf("In child process can't open /dev/tty before setsid().\n");
    } else {
      printf("In child process can opend /dev/tty before setsid().\n");
    }
    
    pr_ids("In child process ,before setsid() ");
    setsid();
    pr_ids("In child process ,after setsid() ");
  
    result = open("/dev/tty", 0);
    if (result < 0) 
      printf("In child process can't open /dev/tty after setsid().\n");
  } else {
    pr_ids("In parent process , ");
  }
  
  exit(0);
}
