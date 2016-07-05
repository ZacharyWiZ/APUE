#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int main(void) {
  char str[3] = {0};
  int num = 888;

  itoa(num, str, 3);
  printf("num: %d, str:%s\n", num, str);
  return 0;
}
