#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
/*
 * In this test, we can know that : char [] / char * is diff.
 * char [] is in the stack
 * char *  is in the .rodata
 * it is very import.
 */

void make_temp(char *template);

void test_const_str(void);

int main(void) {
  char good_template[] = "/tmp/dirXXXXXX";
  char *bad_template   = "/tmp/dirXXXXXX";

  printf("good: %x, bad: %x.\n", good_template, bad_template);

  test_const_str();

  printf("trying to create first temp file...\n");
  make_temp(good_template);
  printf("trying to create second temp file...\n");
  make_temp(bad_template);
  return 0;
}

void make_temp(char *template) {
  int fd;
  struct stat sbuf = {0};
  
  if ((fd = mkstemp(template)) < 0)
    perror("cannot create temp.\n");

  printf("temp name = %s.\n", template);
  close(fd);

  if (stat(template, &sbuf) < 0) {
    if (errno == ENOENT)
      printf("file doesnot exist.\n");
    else
      printf("stat failed.\n");
  }else {
    printf("file exist.\n");
    unlink(template);
  }
}


void test_const_str(void) {
  char *str1  = "/tmp/dirXXXXXX";
  char *str2  = "/tmp/dirXXXXXX";
  char str3[] = "/tmp/dirXXXXXX";
  char str4[] = "/tmp/dirXXXXXX";

  printf("char* : %x , %x\n", str1, str2); 
  printf("char[]: %x , %x\n", str3, str4);
}
