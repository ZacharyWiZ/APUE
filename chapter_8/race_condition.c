#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static void charatatime(char *);

int main(void) {
  pid_t pid;

  //TELL_WAIT();
  
  if ((pid = fork()) < 0) {
    printf("fork error.\n");
    exit(0);
  }else if (pid == 0) {
    charatatime("output from child.\n"); 
    //TELL_PARENT(getppid());
  }
  else {
    //WAIT_CHILD();
    charatatime("output from parent.\n");
  }
  exit(0);
}

static void charatatime(char *str) {
  char *ptr;
  int  c;
  
  setbuf(stdout, NULL);
  for (ptr = str; (c=*str++)!=0; )
    putc(c, stdout);
}
