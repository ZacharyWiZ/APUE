#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char *env_init[] = {"USER=unknown", "PATH=/root/exec_test", NULL};

static void add_path(char *addpath) {
  char *pathvar;
  char newpath[1000];
  
  pathvar = getenv("PATH");
  strcpy(newpath, pathvar);
  strcat(newpath, addpath);
  setenv("PATH", newpath, 1);

  pathvar = getenv("PATH");
  printf("the new pathvar:%s.\n",pathvar);
}

int main(void) {
  pid_t pid;
  
  if ((pid = fork()) < 0) {
    printf("fork error.\n");
    exit(-1);
  }else if (pid == 0) { //child 
    if (execle("/root/exec_test/echoall", "echoall", "myarg1",
      "MYARG2", (char*)0, env_init) < 0) {
      printf("execle error.\n");
      exit(-1);
    }
  }

  if (waitpid(pid, NULL, 0) < 0) {
    printf("wait error.\n");
    exit(-1);
  }

  if ((pid = fork()) < 0) {
    printf("fork error.\n");
    exit(-1);
  } else if (pid == 0) {
    //add_path(":/root/exec_test:");
    if (execlp("echoall", "echoall", "only 1 arg", (char*)0) < 0) {
      printf("execlp error.\n");
      printf("execlp error.: errno:%d, strerr:%s.\n", errno, strerror(errno));
      exit(-1);
    }
  }

  waitpid(pid, NULL, 0);
  printf("successful exit.\n");
   
  exit(0);
}
