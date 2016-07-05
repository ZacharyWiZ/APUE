#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "APUE.h"

struct arg_set{     /*two value in one arg*/
    int number;
    char *fname;    /*file to examine*/
    int count;      /*number of words*/
};

struct arg_set *mailbox = NULL;
pthread_mutex_t lock    = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  flag    = PTHREAD_COND_INITIALIZER;

void *count_words(void *);

int main(int argc, char *argv[]) {

    pthread_t       pid_t1, pid_t2; /*two threads*/
    struct arg_set  args1, args2;   /*two argsets*/
    int             reports_in  = 0;
    int             total_words = 0;

    if (argc != 3) {
        err_sys("usage: %s file1 file2.\n", argv[0]); 
    }

    pthread_mutex_lock(&lock);
    args1.fname  = argv[1];
    args1.number = 1;
    args1.count  = 0;
    pthread_create(&pid_t1, NULL, count_words, &args1);

    args2.fname  = argv[2];
    args2.number = 2;
    args2.count  = 0;
    pthread_create(&pid_t2, NULL, count_words, &args2);

    while (reports_in < 2) {
        printf("MAIN : waiting for flag to go on.\n");
        pthread_cond_wait(&flag, &lock);
        printf("MAIN : Wow! flag was raised ,I have the lock");
        printf("%7d: %s.\n", mailbox->count, mailbox->fname);
        total_words += mailbox->count;
        if (mailbox == &args1)
            pthread_join(pid_t1, NULL);
        if (mailbox == &args2)
            pthread_join(pid_t2, NULL);
        mailbox = NULL;
        pthread_cond_signal(&flag);
        ++reports_in;
    }
    printf("%7d: total words.\n", total_words);
    exit(0);
}

void *count_words(void *a) {

    struct arg_set *args = (struct arg_set *)a;
    FILE *fp = NULL;
    int c, prevc = '\0';
    
    if ((fp = fopen(args->fname, "r")) != NULL) {
        while ((c = getc(fp)) != EOF) {
            if (!isalnum(c) && isalnum(prevc))
                ++args->count;
            prevc = c;
        }
        fclose(fp);
    }else 
        err_sys("cannot open file %s.", args->fname);
    printf("%d COUNT: waiting to get lock.\n", args->number);
    pthread_mutex_lock(&lock);
    printf("%d COUNT: have lock , storing data.\n", args->number);
    
    while (mailbox != NULL) {
        printf("%d COUNT: oops..mailbox not empty wait for signal.\n", args->number);
        pthread_cond_wait(&flag, &lock);
    }
    mailbox = args;
    printf("%d COUNT: raising flag.\n", args->number);
    pthread_cond_signal(&flag);
    printf("%d COUNT: unlocking lock.\n", args->number);
    pthread_mutex_unlock(&lock);
    return NULL;
}
