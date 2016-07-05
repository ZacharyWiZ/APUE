#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>

static volatile int num_live_children = 0;

char *curr_time() {
    time_t timep;
    time(&timep);
    return ctime(&timep);
}

static void
sigchld_handler(int sig) {
    int child_status;
    int save_errno;
    pid_t child_pid;

    save_errno = errno;

    printf("[%s %s] : get sig : %d (%s)\n\n",
         __FUNCTION__, curr_time(), sig, strsignal(sig));

    while ((child_pid = waitpid(-1, &child_status, WNOHANG)) > 0) {
        printf("[%s %s] : wait the child process ,id:%d, exitcode:%d\n\n",
            __FUNCTION__, curr_time(), child_pid, WEXITSTATUS(child_status));
        num_live_children--;
    }

    if (child_pid == -1 && errno != ECHILD) {
        printf("waitpid error\n");
        exit(1);
    }
    sleep(6);
    printf("[%s %s] : handler returning\n\n", __FUNCTION__, curr_time());

    errno = save_errno;
}

int main(int argc, char *argv[]) {
    sigset_t block_mask, empty_mask, orig_mask;
    struct sigaction sa;    
    int i, child_cnt, working_time;
    pid_t fork_ret;

    if (argc < 2 || strcmp(argv[1], "-h")==0) {
        printf("usage: ./a.out child-proc-work-time1 ... and so no\n");
        exit(1);
    }

    setbuf(stdout, NULL);
    num_live_children = argc - 1;
    child_cnt = 0;
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sigchld_handler;
    sa.sa_flags |= SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);
    //block the SIGCHLD
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1) {
        printf("sigprocmask error\n");
        exit(1);
    }
    
    for (i=1; i<argc; ++i) {
        fork_ret = fork();
        switch(fork_ret) {
        case -1:
            printf("fork error\n");
            exit(1);
            break;
        case 0: //child process
            working_time = atoi(argv[i]);
            //printf("[%s %s]: change child process ,id:%d , sleep:%d\n\n",
            //    __FUNCTION__, curr_time(), getpid(), working_time);
            sleep(working_time);
            printf("[%s %s]: child process id:%d exit\n\n", 
                __FUNCTION__, curr_time(), getpid());
            _exit(working_time);
            break;
        default:
            continue;
        }
    }

    sigemptyset(&empty_mask);
    while (num_live_children > 0) {
        if (sigsuspend(&empty_mask) == -1 && errno != EINTR) {
            printf("sigsuspend errno\n");
            exit(1);
        }
        child_cnt++;
    }
    
    printf("[%s %s] main fork exit, %d children have terminate;" 
        "SIGCHLD was caught %d times\n\n",
        __FUNCTION__, curr_time(), argc-1, child_cnt);
    exit(0);
}
