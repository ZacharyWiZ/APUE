#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int func(int malloc_size) {
    char *buf;
    if ((buf = (char*)malloc(malloc_size)) == NULL ) {
        printf("malloc failed\n");
        exit(1);
    }

    printf("%s : program break is now : %10p in the son process\n", __FUNCTION__, sbrk(0));
    return 9;
}

int main(int argc, char *argv[]) {
    int malloc_size;
    pid_t child_pid;
    int status;

    if (argc>1 && strcmp(argv[1], "-h")==0) {
        printf("usage: ./a.out malloc_size\n");
        exit(0);
    }

    printf("%s : program break is : %10p before fork\n", __FUNCTION__, sbrk(0));

    malloc_size = argc>1 ? atoi(argv[1]) : 100000;
    
    child_pid = fork();
    if (child_pid == -1) {
        printf("fork error\n");
        exit(1);
    } else if (child_pid == 0) {
        exit(func(malloc_size));
        //func(malloc_size);
        //return 0;
    }

    if (wait(&status) == -1) {
        printf("wait error\n");
        exit(1);
    }
    printf("%s : program break is now : %10p\n", __FUNCTION__, sbrk(0));
    printf("%s : wait the status is %d\n", __FUNCTION__, status);
    exit(0);
}
