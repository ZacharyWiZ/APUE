#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

unsigned long long count;
struct timeval end;

void checktime(char *str) {
  struct timeval tv;
  
  gettimeofday(&tv, NULL);
  if (tv.tv_sec >= end.tv_sec &&
    tv.tv_usec >= end.tv_usec) {
    printf("%s count = %lld, pid:%ld.\n", str, count, (long)getpid());
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  pid_t pid;
  char *s;
  int nzero, ret;
  int adj = 0;
  int i=0;
  setbuf(stdout, NULL);

#if defined(NZERO)
  nzero = NZERO;
#elif defined(_SC_NZERO)
  nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif
  
  printf("NZERO = %d.\n", nzero);
  
  if (argc == 2)
    adj = strtol(argv[1], NULL, 10);
  gettimeofday(&end, NULL);
  end.tv_sec += 10;  /*run for 10 secondes */

  for (i=0; i<3; i++)
  if ((pid = fork()) < 0) {
    printf("fork failed.\n");
    exit(-1);
  } else if (pid == 0) { /* child */
    s = "child";
    printf("current nice value in child is %d, adjusting by %d, pid:%ld.\n",
      nice(0)+nzero, adj, (long)getpid());
    errno = 0;
    if ((ret = nice(adj)) == -1 && errno != 0) {
      printf("child set scheduling priority.str:%s.\n", strerror(errno));
      exit(-1);
    }
    printf("now child nice value is %d, pid:%ld.\n", ret+nzero, (long)getpid());
  }else { /* parent */
    s = "parent";
    printf("current nice value in parent is %d, pid:%ld.\n", 
      nice(0) + nzero, (long)getpid());
  }

  for (;;) {
    if (++count == 0) {
      printf("%s counter wrap.\n", s);
      exit(-1);
    }
    checktime(s);
  }
}
