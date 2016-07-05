#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "APUE.h"

int intRet = 55;

void *thr_fn1(void *arg) {
  //char s[] = "hi msy.";
  //int intRet = 55;

  printf("%s run.\n", __FUNCTION__);
  pthread_exit((void*)&intRet);
}

void *thr_fn2(void *arg) {
  
  printf("%s run.\n", __FUNCTION__);
  return (void*)2;
}

int main(void) {
  int err;
  pthread_t tid1, tid2;
  void *tret = NULL;
  //char *sret = NULL;
  int *intRet = NULL;

  if ( pthread_create(&tid1, NULL, thr_fn1, NULL) != 0)
    err_sys("pthread_create tid1 failed.\n");
  
  if ( pthread_create(&tid2, NULL, thr_fn2, NULL) != 0)
    err_sys("pthread_create tid2 failed.\n");

  if ( pthread_join(tid1, (void **)&intRet) != 0)
    err_sys("cannot join thread %ld.\n", (unsigned long)tid1);
  printf("thread 1 exit code : %d.\n", *intRet);

  if ( pthread_join(tid2, &tret) != 0)
    err_sys("cannot join thread %ld.\n", (unsigned long)tid2);
  printf("thread 2 exit code : %ld.\n", (long)tret);

  exit(0);
}
