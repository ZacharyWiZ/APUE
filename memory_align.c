#include <stdio.h>

//#pragma pack(show)

#define struct_position(str, var) (int)((void*)(&str.var) - (void*)&str)

struct st1 {
  char a;
  int  b;
  short c;
} st1;

struct str2 {
  short c;
  char  a;
  int   b;
} st2;

/*
struct st3 {
  char a;
  char b;
  int c;
  long d;
  int e;
  int f;
  char g;
} st3;
*/

struct st3 {
  char a;
  char b;
  int *c;
  char d;
}st3;

int main() {
  printf("sizeof(st1) is %d.\n", sizeof(st1));
  printf("sizeof(st2) is %d.\n", sizeof(st2));
  printf("sizeof(st3) is %d.\n", sizeof(st3));
  printf("position st3->b : %d, ->c:%d, ->d:%d.\n", struct_position(st3, b),
    struct_position(st3, c), struct_position(st3, d));
  printf("sizeof(double) : %d.\n", sizeof(double));
  printf("sizeof(long) : %d.\n", sizeof(long));
  printf("sizeof(int*) : %d.\n", sizeof(int *));
  return 0;
}
