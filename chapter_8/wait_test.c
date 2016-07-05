#include <stdio.h>
#include "pr_exit.h"
#include <sys/wait.h>
#include <stdlib.h>

int main(void) {
  pid_t pid;
  int   status;

  if ((pid = fork()) < 0) 
    printf("fork error.\n");
  else if (pid == 0)
    exit(7);
  

  if (wait(&status) != pid)
    printf("wait error.\n");
  pr_exit(status);

  if ((pid = fork()) < 0)
    printf("fork error.\n");
  else if (pid == 0)
    abort();

  if (wait(&status) != pid)
    printf("wait error.\n");
  pr_exit(status);

  if ((pid = fork()) < 0) 
    printf("fork error.\n");
  else if (pid == 0)
    status /= 0;

  if (wait(&status) != pid)
    printf("wait error.\n");
  pr_exit(status);

  exit(0);
}
