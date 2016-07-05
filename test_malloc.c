#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int * a = NULL;
  a = (int*) malloc(sizeof(int) * 2);
  a[0] = 1;
  a[1] = 2;
  printf("%d %d %d %d.\n", a[-2], a[-1], a[0], a[1]);
  return 0;
}
