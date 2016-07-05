#include <stdio.h>
#include <string.h>

int parse_str(char *buf, char *p_strs[], const char *delime) {
  int p_num = 0;
  char *str = buf;

  while (NULL != (p_strs[p_num] = strtok(str, delime))) {
    ++p_num;
    str = NULL;
  }
  return p_num;
}

int main() {
  char parse_buf[256] = {0};
  char *p_strs[6] = {0};
  int num = 0;
  int i;
  snprintf(parse_buf, 256, "192.168.24.69");
  
  num = parse_str(parse_buf, p_strs, ".");
  for (i=0; i<num; i++) {
    printf("num:%d : %s\n", i, p_strs[i]);
  }
  return 0;
}
