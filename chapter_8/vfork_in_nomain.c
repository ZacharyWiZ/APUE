#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "pr_exit.h"

pid_t pid;

int function_use_vfork(){
  
  if ((pid = vfork()) < 0) {
    printf("vfork error.\n");
    exit(-1);
  } else if (pid == 0) {
    printf("create the child vfork , pid:%ld, father's pid:%ld.\n",
      (long)getpid(), (long)getppid());
  }
  
  sleep(2);
  return 1;
}

int main(void) {
  int status;
  printf("begin vfork in function.\n");

  function_use_vfork();

  if (pid != wait(&status)) {
    printf("wait error.\n");
  }

  pr_exit(status);
  return 1;
}
