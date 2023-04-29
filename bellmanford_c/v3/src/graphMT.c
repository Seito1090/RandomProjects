#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFERSIZE 10

uint32_t firstbuffer[BUFFERSIZE];
thread_data_t buffer[BUFFERSIZE];
int head = 0;
int tail = 0;

pthread_mutex_t mutex;
pthread_cond_t not_empty;

graph_t *graph;
bool verbose = false;

void *readthread(void *arg) {
    char *filename = (char *) arg;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        pthread_exit(NULL);
    }

    // do something with the file
    graph = (graph_t *)get_file_info(file);
    printf("Number of nodes: %d\n", graph->file_infos->nb_nodes);
    if (graph == NULL) {
        printf("Error reading graph from file\n");
        pthread_exit(NULL);
    }
    for (uint32_t src; src < graph->file_infos->nb_nodes; src++) {
        pthread_mutex_lock(&mutex);
        while ((head + 1) % BUFFERSIZE == tail) {
            pthread_cond_wait(&not_empty, &mutex);
        }
        firstbuffer[head] = src;
        printf("Read thread: %d\n", src);
        head = (head + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&not_empty);
    }
    pthread_exit(NULL);
}

void *computers(void *arg) {
    uint32_t source;
    bool* done = (bool *)arg;
    thread_data_t *data = (thread_data_t *)malloc(sizeof(thread_data_t));
    while (true){
        pthread_mutex_lock(&mutex);
        while (head == tail){
            pthread_cond_wait(&not_empty, &mutex);

            if (*done) {
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }
        }
        source = firstbuffer[tail];
        tail = (tail + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&mutex);
        if (graph == NULL) {
            printf("Error: graph is null\n");
            pthread_exit(NULL);
        }
        ford_t *ford = (ford_t *)bellman_ford(graph->file_infos->nb_nodes, graph->file_infos->nb_edges, graph->graph_data, source, verbose);
        printf("Distance from %d to %d: %d\n", source, 1, ford->dist[1]);
        mcost_t * max = (mcost_t *)get_max(graph->file_infos->nb_nodes, ford->dist, source);
        printf("Max cost: %ld\n", max->cost);
        printf("Max node: %d\n", max->node);
        int32_t size = graph->file_infos->nb_nodes;
        int32_t * path = (int32_t *)get_path(max->node, source, ford->path, &size);
        // data->source = source;
        // data->max = max;
        // data->size = size;
        data->path = path;
        printf("path size:  %d\n", size);
        free_path(path);
        free_max_struct(max);
        free_ford_struct(ford);
    }
    pthread_exit(NULL);
}

void *writethread(void *arg){
    thread_data_t *data = (thread_data_t *)arg;
    if (data == NULL) {
        printf("Error: data is null\n");
        pthread_exit(NULL);
    }
    int wrote;
    wrote = write_to_file(data->filename, data->source, data->max, data->size, data->path);
    if (wrote == 1) {
        printf("Error writing to file\n");
        pthread_exit(NULL);
    }
    fclose(data->filename);
    pthread_exit(NULL);
}

int main(int args, char *argv[]) {
    pthread_t thread, computethread1, computethread2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);

    //start producer thread
    char *filename = "tests/graph_bin/default.bin";
    int grph, rep;
    grph = pthread_create(&thread, NULL, readthread, (void *) filename);
    if (grph != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    // start consumer thread
    bool done = false;
    rep = pthread_create(&computethread1, NULL, computers, (void *)&done);
    pthread_create(&computethread2, NULL, computers, (void *)&done);
    if (rep != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    // wait for threads to finish 
    pthread_join(thread, NULL);

    // signal consumer thread to finish
    pthread_mutex_lock(&mutex);
    done = true;
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&mutex);

    // wait for consumer thread to finish
    pthread_join(computethread1, NULL);
    pthread_join(computethread2, NULL);  

    //cleanup 
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    free_graph_struct(graph);
    return 0;
}
