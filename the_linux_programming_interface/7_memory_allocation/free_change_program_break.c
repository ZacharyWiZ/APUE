#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
/*
 * usage: ./a.out num-allocs block-size free-step free-min free-max
 * just test malloc and free to change the program break.
 */

#define MAX_ALLOCS 1000

int err_sys(char *fmt, ...) {
    va_list va_args;
    char buf[512] = {0};

    va_start(va_args, fmt);
    vsprintf(buf, fmt, va_args);
    printf("%s\n", buf);
    va_end(va_args);
    exit(1);     
}

int main(int argc, char *argv[]) {

    int num_allocs, block_size, free_step, free_min, free_max, i;    
    char *ptr[MAX_ALLOCS] = {0};

    printf("\n");

    if (argc <3 || strcmp(argv[1], "-h")==0) {
        printf("argc : %d.\n", argc);
        err_sys("usage: %s num-allocs block-size"
            "[step [min [max]]]", argv[0]);
    }
    
    num_allocs = atoi(argv[1]);
    num_allocs = num_allocs > MAX_ALLOCS ? MAX_ALLOCS : num_allocs;

    block_size = atoi(argv[2]);
    free_step  = (argc > 3) ? atoi(argv[3]) : 1;
    free_min   = (argc > 4) ? atoi(argv[4]) : 1;
    free_max   = (argc > 5) ? atoi(argv[5]) : num_allocs;
    free_max   = free_max > num_allocs ? num_allocs: free_max;

    printf("Initial program break : %10p\n", sbrk(0));
    
    printf("Allocating %d*%d bytes\n", num_allocs, block_size);
    for (i=0; i<num_allocs; ++i) {
        ptr[i] = malloc(block_size);
        if (ptr[i] == NULL)
            err_sys("malloc error");
    }

    printf("Program break is now : %10p\n", sbrk(0));

    printf("Freeing block from %d to %d in steps of %d\n",
        free_min, free_max, free_step);

    for (i=free_min; i<free_max; i+=free_step) {
        free(ptr[i]);
    }
    printf("After free(), program break is %10p\n", sbrk(0));

    exit(0);
}
