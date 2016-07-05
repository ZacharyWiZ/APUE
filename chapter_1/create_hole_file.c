/*
 * create a hole file
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define BUF_LEN 10
#define HOLE_LEN 1000

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH

int main(void) {
  int fd;
  char buf1[] = "abcdefghij";
  char buf2[] = "ABCDEFGHIJ";
  
  if ((fd = creat("file.hole", FILE_MODE)) < 0)
    perror("create error");
  
  if (write(fd, buf1, BUF_LEN) != BUF_LEN)
    perror("buf1 write error");
  /*offset now is 10*/

  if (lseek(fd, HOLE_LEN, SEEK_SET) == -1)
    perror("lseek file_hole error");
  /*offset now is 1000*/
  
  if (write(fd, buf2, BUF_LEN) != BUF_LEN)
    perror("buf2 write error");

  return 0;
}
