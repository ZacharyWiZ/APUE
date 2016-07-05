#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int globvar = 6;

int main() {
  pid_t pid;
  int var;
  char buf[] = "a write to stdout(SYSIO).\n";

  printf("sizeof(buf) : %d, strlen : %d.\n", 
    sizeof(buf), strlen(buf));
  
  var = 99;
  if (write(1, buf, sizeof(buf)-1) != sizeof(buf)-1)
    printf("write error.\n");

  printf("a printf to stdout(STDIO).\n");

  if ((pid = fork()) < 0) 
    printf("fork error");
  else if (pid == 0) {
    ++globvar;
    ++var;
  }else
    sleep(2);
  
  printf("pid = %d, glo = %d, var = %d.\n", getpid(),
    globvar, var);

  exit(0);
}

