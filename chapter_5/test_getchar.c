#include <stdio.h>

/*
 * just test getchar()'s return is int , 
 * and how to convert to unsigned char.
 */


int main(void) {
  int c;
  unsigned char a;

  c = getc(stdin);
  a = (char) c;
  printf("out: %c.\n", a);
  printf("sizeof(int): %d.\n", sizeof(c));
  printf("sizeof(unsigned char): %d.\n", sizeof(a));
  return 0;
}
