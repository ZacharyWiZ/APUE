#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 512

int main(void) {
    int addend1, addend2;
    char line[MAXLINE];

    while (fgets(line, MAXLINE, stdin) != NULL) {
        if (sscanf(line, "%d%d", &addend1, &addend2) == 2) {
            if (printf("%s`d\n", addend1 + addend2) == EOF) {
                printf("printf error\n");
                exit(1);
            } 
            fflush(stdin);
            fflush(stdout);
        }
        else {
            if (printf("invalid args\n") == EOF)
                exit(1);
        }
    }
    exit(0);
}
