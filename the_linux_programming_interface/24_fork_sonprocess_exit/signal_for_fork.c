#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define SYNC_SIG SIGUSR1

static void
sync_sig_handler(int sig) {
    printf("get sig : %d( %s )\n", sig, strsignal(sig));
}

int main(int argc, char *argv[]) {
    pid_t child_pid;
    sigset_t block_mask, orig_mask, empty_mask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SYNC_SIG);
    if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1) {
        printf("sigprocmask error\n");
        exit(1);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags |= SA_RESTART;
    sa.sa_handler = sync_sig_handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1) {
        printf("sigaction error\n");
        exit(1);
    }

    child_pid = fork();
    if (child_pid == -1) {
        printf("fork error\n");
        exit(1);
    }
    if (child_pid == 0) { // child process
        printf("Child started - doing some work\n");
        sleep(1);
        printf("Child about to signal parent\n");
        kill(getppid(), SYNC_SIG);
        exit(0);
    }

    printf("Parent about to wait for signal\n");
    sigemptyset(&empty_mask);
    if (sigsuspend(&empty_mask) == -1 && errno != EINTR) {
        printf("sigsuspend error\n");
        exit(1);
    }
    printf("Parent got signal\n");

    if (sigprocmask(SIG_SETMASK, &orig_mask, NULL) == -1) {
        printf("sigprocmask error\n");
        exit(1);
    }
    return 0;
}
