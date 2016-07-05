#include <stdio.h>
#include <string.h>
#include <error.h>

#define BSZ 48

#define Print_Memstream(premise) print_arr(buf, BSZ, premise)

inline print_offset(FILE *fp) {
  printf("offset: %ld.\n", ftell(fp));
}

void print_arr(char arr[], int size, const char *premise) {
  int i;
  printf("%s:\n", premise);
  for (i=0; i<size; i++) {
    printf("%d:%c  ", i, arr[i]);
  }
  printf("\n");
}


void init_buf(char arr[], int size, char init_char) {
  memset(arr, init_char, size-2);
  arr[size-2] = '\0';
  arr[size-1] = 'X';
  printf("init_buff: arr[0]:%c. arr:%s\n", arr[0], arr);
}

int main(void) {
  FILE *fp;
  char buf[BSZ];

  init_buf(buf, BSZ, 'a');

  if ((fp = fmemopen(buf, BSZ, "w+")) == NULL)
    perror("fmemopen failed.");
  /* fmemopen will put '\0' in buf[0] */

  print_offset(fp);
  print_arr(buf, BSZ, "initial buffer contents");
  fprintf(fp, "hello, world");
  print_arr(buf, BSZ, "before fflush");
  fflush(fp);
  Print_Memstream("after fflush");  
  printf("len of string in buf = %ld, buf:%s\n", (long)strlen(buf), buf);

  printf("\n\n=============\n\n");
  print_offset(fp);
  init_buf(buf, BSZ, 'b');
  fprintf(fp, "hello world");
  Print_Memstream("before fseek");
  fseek(fp, 0, SEEK_SET);
  Print_Memstream("after fseek");
  printf("len of string in buf = %ld, buf:%s\n", (long)strlen(buf), buf);

  printf("\n\n=============\n\n");
  print_offset(fp);
  init_buf(buf, BSZ, 'c');
  fprintf(fp, "hello, world");
  Print_Memstream("before fclose");
  print_offset(fp);
  fclose(fp);
  Print_Memstream("after fclose");
  printf("len of string in buf = %ld, buf:%s\n", (long)strlen(buf), buf);

  return 0;
}
