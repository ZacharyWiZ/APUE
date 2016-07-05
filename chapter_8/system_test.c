#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "pr_exit.h"

int main(void) {
  int status;
  
  if ((status = system("date")) < 0) 
    goto ERR_EXIT;
  pr_exit(status);

  if ((status = system("nosuchcommand")) < 0)
    goto ERR_EXIT;
  pr_exit(status);

  if ((status = system("who; exit 44")) < 0)
    goto ERR_EXIT;
  pr_exit(status);

  exit(0);

ERR_EXIT:
  printf("system() error.\n");
  exit(-1);
 
}
