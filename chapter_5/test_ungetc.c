#include <stdio.h>

#define EXCHANGE_LINE '\n'

int main(void) {
  int c;
  char a;
  char prev = '\n';
  
  while ((c = getc(stdin)) != EOF) {
    a = (char) c;

    if (a == EXCHANGE_LINE)
      continue;

    printf("%c~ ", a);
    if (prev != c) {
      ungetc(a, stdin);
      prev = a;
    }else {
      prev = EXCHANGE_LINE;
      printf("\n");
    }
  }

  return 0;
}
