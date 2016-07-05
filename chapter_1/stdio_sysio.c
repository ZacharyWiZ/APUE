/*
 * STDIO: putchar ..and so on
 * SYSIO: just use system function
 */
#include <stdio.h>
#include <unistd.h>

int main(void) {
  putchar('a');
  write(1, "b", 1);

  putchar('a');
  write(1, "b", 1);

  putchar('a');
  write(1, "b", 1);

  printf("\n");
  return 0;
}
