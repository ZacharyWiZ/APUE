#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 512

int main(void) {
    int io_cnt, addend1, addend2;
    char line[MAXLINE];

    while ((io_cnt = read(STDIN_FILENO, line, MAXLINE)) > 0) {
        line[io_cnt] = 0;
        if (sscanf(line, "%d%d", &addend1, &addend2) == 2) {
            sprintf(line, "%d\n", addend1 + addend2);
            io_cnt = strlen(line);
            sleep(4);
            if (write(STDOUT_FILENO, line, io_cnt) != io_cnt) {
                printf("write error\n");
                exit(1);
            }
        } else {
            if (write(STDOUT_FILENO, "invalid args\n", 13) != 13) {
                printf("write error\n");
                exit(1);
            }
        }
    }
    exit(0);
}
