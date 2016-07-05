#define _GNU_SOURCE  /*get strsignal() declareation from <string.h> */
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

static void 
sigsegv_handler(int sig) {
    int x;

    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
    printf("Top of handler stack near  %10p\n", (void*)&x);
    fflush(NULL);
    exit(1);
}

static void
overflow_stack(int call_num) {
    char a[100000];
    
    printf("Call %4d - top of stack near %10p\n", call_num, (void*)a);
    overflow_stack(call_num + 1);
}

int main(int argc, char *argv[]) {
    stack_t sigstack;
    struct sigaction sa;
    int stack_param;

    printf("Top of standard stack is near %10p\n", &stack_param);
    
    sigstack.ss_sp = malloc(SIGSTKSZ);
    if (sigstack.ss_sp == NULL)
        exit(1);
    sigstack.ss_size = SIGSTKSZ;
    sigstack.ss_flags = 0;
    if (sigaltstack(&sigstack, NULL) != 0) {
        printf("sigaltstack error\n");
        exit(1);
    }
    printf("Alternate stack is at  %10p-%p\n",
        sigstack.ss_sp, (char *) sbrk(0) - 1);

    
    memset(&sa, 0x00, sizeof(sa));
    sa.sa_handler = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;
    
    if (sigaction(SIGSEGV, &sa, NULL) != 0) {
        printf("sigaction error");
        exit(1);
    }
    
    overflow_stack(1);
}
