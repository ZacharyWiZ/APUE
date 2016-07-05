#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static void f1(void), f2(void);

int main(void) {
  f1();
  f2();
  _exit(0);
}

static void f1(void) {
  pid_t pid ;
  if ((pid = vfork()) < 0) {
    printf("vfork error.\n");
    return ;
  }
  printf("pid:%ld, getpid():%ld.\n", pid, (long)getpid());
  return ;
}

static void f2(void) {
  char buf[1000];
  int i;
  for (i = 0; i<sizeof(buf); i++)
    buf[i] = 0;
}
