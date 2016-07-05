#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static volatile int handler_sleep_time;
static volatile int sig_cnt = 0;
static volatile sig_atomic_t all_done = 0;

static void
siginfo_handler(int sig, siginfo_t *si, void *ucontext){
    if (sig == SIGINT || sig == SIGTERM) {
        all_done = 1;
        return ;
    }
    
    ++sig_cnt;
    printf("caught signal : %d\n", sig);
    
    printf("   si_signo=%d, si_code=%d (%s),", si->si_signo, 
        si->si_code, (si->si_code == SI_USER) ? "SI_USER" : 
                     (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");

    printf("si_value=%d\n", si->si_value.sival_int);
    printf("    si_pid=%ld, si_uid=%ld\n", (long)si->si_pid,
        (long)si->si_uid);
    sleep(handler_sleep_time);
}

int main(int argc, char *argv[]) {
    struct sigaction sa;
    int sig;
    sigset_t pre_mask, block_mask;

    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        printf("%s [block-time [handler-sleep-time]]\n");
        exit(1);
    }
    
    memset(&sa, 0x00, sizeof(sa));
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction = siginfo_handler;
    sigfillset(&sa.sa_mask);
    
    for (sig = 1; sig < NSIG; ++sig) 
        if (sig != SIGTSTP && sig != SIGQUIT)
            sigaction(sig, &sa, NULL);

    if (argc > 1) {
        sigfillset(&block_mask);
        sigdelset(&block_mask, SIGINT);
        sigdelset(&block_mask, SIGTERM);
        
        if (sigprocmask(SIG_SETMASK, &block_mask, &pre_mask) == -1) {
            printf("sigprocmask error\n");
            exit(1);
        }
        
        printf("%s: signals blocks - sleeping %s seconds\n",
            argv[0], argv[1]);
        sleep(atoi(argv[1]));
        printf("%s: sleep complete\n", argv[0]);

        if (sigprocmask(SIG_SETMASK, &pre_mask, NULL) == -1) {
            printf("sigprocmask error\n");
            exit(1);
        }
        
        handler_sleep_time = argc>2 ? atoi(argv[2]) : 1;
    }
    while (!all_done)
        pause();
    exit(0);
}
