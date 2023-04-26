#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

int global = 0;

void error(int err, char *msg) {
    fprintf(stderr, "%s: %s\n", msg, err, strerror(errno));
    exit(EXIT_FAILURE);
}

void *thread_first(void *arg) {
    global++;
    return NULL;
}

void *thread_second(void *arg) {
    global++;
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t first;
    pthread_t second;
    int err;
    
    err= pthread_create(&first, NULL, thread_first, NULL);
    if (err != 0) {
        error(err, "Impossible de creer le thread first");
    }

    err= pthread_create(&second, NULL, thread_second, NULL);
    if (err != 0) {
        error(err, "Impossible de creer le thread second");
    }

    for (int i = 0; i < 1000000; i++) {
        global++;
    }

    err = pthread_join(second, NULL);
    if (err != 0) {
        error(err, "Impossible de joindre le thread second");
    }

    err = pthread_join(first, NULL);
    if (err != 0) {
        error(err, "Impossible de joindre le thread first");
    }

    printf("global = %d\n", global);
}
