#ifndef __SIGNAL_FUNCTIONS_H_
#define __SIGNAL_FUNCTIONS_H_

#include <string.h>
#include <signal.h>
#include <stdio.h>

void print_sigset(FILE *of, const char *prefix, const sigset_t *sigset);
int  print_sigmask(FILE *of, const char *msg);
int  print_pendingSigs(FILE *of, const char *msg);

#endif
