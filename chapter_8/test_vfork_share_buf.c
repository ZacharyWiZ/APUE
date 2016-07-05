#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "pr_exit.h"

static int globvar = 6;

int main(void) {
  int var = 88;
  pid_t pid;
  int status;
  int i;
  char buf[512] = {0};

  printf("before vfork.\n");

  if ((pid = vfork()) < 0)
    printf("vfork error.\n");
  else if (pid == 0) { /* child */
    ++globvar;
    ++var;
    printf("pid = %d.\n", (long)getpid());
    fclose(stdout);
    exit(0);
  }
  
  if (wait(&status) != pid) {
    printf("wait error.\n");
  }
  pr_exit(status);

  i = printf("pid = %ld, child_pid = %ld, glob = %d, var = %d.\n", 
        (long)getpid(), (long)pid, globvar, var);
  
  sprintf(buf, "printf's ret :%d.\n", i);
  write(STDOUT_FILENO, buf, strlen(buf));
  
  exit(0);
}

