#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>

int main(void) {
  struct rlimit *limit = (struct rlimit*)malloc(sizeof(struct rlimit));
  getrlimit(RLIMIT_AS, limit);
  printf("soft limit:%ld, hard limit:%ld.\n", 
    limit->rlim_cur, limit->rlim_max);
 
  return 0;
}
