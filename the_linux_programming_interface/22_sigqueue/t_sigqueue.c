#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int sig, num_sigs, j, sig_data;
    union sigval sv;
    pid_t pid;

    if (argc < 4 || strcmp(argv[1], "-h") == 0) {
        printf("%s pid sig-num data [num-sigs]\n", argv[0]);
        exit(1);
    }

    printf("%s: PID is %ld, UID is %ld\n", argv[0],
        (long)getpid(), (long)getuid());

    pid = (pid_t)atoi(argv[1]);
    sig = atoi(argv[2]);
    sig_data = atoi(argv[3]);
    num_sigs = argc>4 ? atoi(argv[4]) : 1;

    for (j=0; j<num_sigs; ++j) {
        sv.sival_int = sig_data;
        if (sigqueue(pid, sig, sv) == -1) {
            printf("sigqueue error\n");
            exit(1);
        }
    }
    exit(0);
}
