#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define THREAD_NUM 2;

struct file_counter {
    char *filename;
    int  words_count;
    int  id;
};

struct file_counter *mailbox = NULL;
pthread_mutex_t mailbox_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  mailbox_cond = PTHREAD_COND_INITIALIZER;

void *count_words(void *file_counter_a);

int main(int argc, char *argv[]) {

    struct file_counter counter1 = {0};
    struct file_counter counter2 = {0};
    int reporter_num = 0;
    int total_counter = 0;
    pthread_t tid1, tid2;


    if (argc != 3) {
        printf("usage: a.out file1 file2.\n");
        exit(-1);
    }
    
    pthread_mutex_lock(&mailbox_lock);
    
    counter1.filename = argv[1];
    counter1.words_count = 0;
    counter1.id = 1;
    pthread_create(&tid1, NULL, count_words, (void*)&counter1);

    counter2.filename = argv[2];
    counter2.words_count = 0;
    counter2.id = 2;
    pthread_create(&tid2, NULL, count_words, (void*)&counter2);    

    while (reporter_num < 2) {
        while (mailbox == NULL) {
            pthread_cond_wait(&mailbox_cond, &mailbox_lock);
        }
        //pthread_mutex_unlock(&mailbox_lock);
        printf("MAIN: get the mailbox.\n");
        
        if (mailbox == &counter1) 
            pthread_join(tid1, NULL);
        
        if (mailbox == &counter2) 
            pthread_join(tid2, NULL);

        total_counter += mailbox->words_count;
        printf("MAIN: get mailmsg: id:%d, words:%d\n", mailbox->id,
            mailbox->words_count);
        mailbox = NULL;
        pthread_cond_signal(&mailbox_cond);
        //pthread_mutex_lock(&mailbox_lock);
        ++reporter_num; 
    }
    printf("MAIN: file: %s, %s 's words total num : %d\n", argv[1],
        argv[2], total_counter);
    exit(0);
}

void *count_words(void *file_counter_a) {
    struct file_counter *counter = (struct file_counter *)file_counter_a;
    FILE *fp = NULL;
    char c, prevc = '\0';
    if ((fp=fopen(counter->filename, "r")) != NULL) {
        while ((c = fgetc(fp)) != EOF) {
            if (!isalnum(c) && isalnum(prevc)) 
                ++counter->words_count;
            prevc = c;
        }
    } else
        printf("open the file failed\n", counter->filename);
    
    pthread_mutex_lock(&mailbox_lock);
    while (mailbox != NULL) {
        printf("COUNT %d : mailbox is not NULL, cond_wait it\n", counter->id);
        pthread_cond_wait(&mailbox_cond, &mailbox_lock);
    }
    mailbox = counter;
    printf("COUNT %d : signal the mailbox_cond\n", counter->id);
    pthread_cond_signal(&mailbox_cond);
    pthread_mutex_unlock(&mailbox_lock);
    return NULL;
}
