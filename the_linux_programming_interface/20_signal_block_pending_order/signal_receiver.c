#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static int sig_cnt[NSIG] = {0};
static volatile sig_atomic_t got_sigint = 0;

void print_sigset(FILE *of, const char *prefix, const sigset_t *sigset) {
    int sig, sig_cnt;

    sig_cnt = 0;
    for (sig=1; sig<NSIG; ++sig) {
        if (sigismember(sigset, sig) == 1) {
            ++sig_cnt;
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
            //fprintf(of, "%s%d \n", prefix, sig);
        }
    }

    if (sig_cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}


int err_sys(char *fmt, ...) {
    va_list va_args;
    char buf[512] = {0};

    va_start(va_args, fmt);
    vsprintf(buf, fmt, va_args);
    printf("%s\n", buf);
    va_end(va_args);
    exit(1);     
}

static void handler(int sig) {
    if (sig == SIGINT)
        got_sigint = 1;
    else 
        ++sig_cnt[sig];
}

int main(int argc, char *argv[]) {
    sigset_t pending_mask, blocking_mask, empty_mask;
    int n, num_secs;

    printf("%s: PID is %ld\n", argv[0], (long)getpid());

    for (n=1; n<NSIG; ++n)
        (void)signal(n, handler);

    if (argc > 1) { // if u want sleep
        num_secs = atoi(argv[1]);
        sigfillset(&blocking_mask);
        if (sigprocmask(SIG_SETMASK, &blocking_mask, NULL) == -1)
            err_sys("sigprocmask error");

        printf("%s : sleeping for %d secondes\n", argv[0], num_secs);
        sleep(num_secs);

        if (sigpending(&pending_mask) == -1)
            err_sys("sigpending error");

        printf("%s : pending signals are:\n", argv[0]);
        print_sigset(stdout, "\t\t", &pending_mask);

        sigemptyset(&empty_mask);
        if (sigprocmask(SIG_SETMASK, &empty_mask, NULL) == -1)
            err_sys("sigprocmask error");
    }

    while (!got_sigint) {
        sleep(1);
    }

    for (n=1; n<NSIG; ++n)
        if (sig_cnt[n] != 0) 
            printf("%s : signal %d caught %d time%s\n", argv[0],
                n, sig_cnt[n], (sig_cnt[n] == 1) ? "" : "s");
    exit(0);
}
