#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include "signal_functions.h"

/*
 *  Print list of signals within a signal set
 */
void print_sigset(FILE *of, const char *prefix, const sigset_t *sigset) {
    int sig, sig_cnt;

    sig_cnt = 0;
    for (sig=1; sig<NSIG; ++sig) {
        if (sigismember(sigset, sig) == 1) {
            ++sig_cnt;
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }

    if (sig_cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}

int  print_sigmask(FILE *of, const char *msg){
    sigset_t curr_mask;
    
    if (msg != NULL)
        fprintf(of, "%s", msg);
   
     if (sigprocmask(SIG_BLOCK, NULL, &curr_mask) == -1)
        return -1;
    
    print_sigset(of, "\t\t", &curr_mask);
    
    return 0;
}

int  print_pendingSigs(FILE *of, const char *msg){
    sigset_t pending_sigs;
    
    if (msg != NULL)
        fprintf(of, "%s", msg);
    
    if (sigpending(&pending_sigs) == -1)
        return -1;

    print_sigset(of, "\t\t", pending_sigs);
    
    return 0;
}


