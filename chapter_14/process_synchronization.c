#include "process_synchronization.h"
#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

int err_sys(char *fmt, ...) {
    va_list args;
    char outbuf[512];
    
    va_start(args, fmt);
    vsprintf(outbuf, fmt, args);
    printf("%s", outbuf);
    va_end(args);
    exit(0);    
}

#define TELL_PARENT(pid) TELL_ONE((pid), SIGUSR2)
#define TELL_CHILD(pid) TELL_ONE((pid), SIGUSR1)
#define WAIT_PARENT() WAIT_ONE()
#define WAIT_CHILD() WAIT_CHILD()

void
sig_usr(int signo) {
    sigflag = 1;
    printf("in the sig_usr : signo:%d\n", signo);
}

void TELL_WAIT(void) {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        err_sys("signal(SIGUSR1) error\n");
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        err_sys("signal(SIGUSR2) error\n");
    }
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);
    
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error\n");
}

void TELL_ONE(pid_t pid, int signo) {
    kill(pid, signo);
}
void WAIT_ONE(void) {
    while (sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;
    
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error\n");
}

/*
int main(void) {
    pid_t pid;
    printf("main begin\n");

    TELL_WAIT(); 

    if ((pid = fork()) < 0) {
        err_sys("fork error\n");
    } else if (pid == 0) { //child process
        printf("hello ,I am child fork first, pid:%d, ppid:%d\n",
             getpid(), getppid());
        TELL_PARENT(getppid());
    } else { // parent process 
        WAIT_PARENT();
        printf("hello , I am parent fork later, pid:%d\n", getpid());
    }
    return 0;
}
*/
