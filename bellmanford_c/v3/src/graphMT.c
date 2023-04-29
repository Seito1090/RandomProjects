#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFERSIZE 3

uint32_t readbuffer[BUFFERSIZE];
int readhead = 0;
int readtail = 0;

pthread_mutex_t read_mutex;
pthread_cond_t read_not_empty;
pthread_cond_t read_not_full;

thread_data_t writebuffer[BUFFERSIZE];
int writehead = 0;
int writetail = 0;

pthread_mutex_t write_mutex;
pthread_cond_t write_not_empty;
pthread_cond_t write_not_full;

graph_t *graph;
bool verbose = false;

//readthread done = true
void *readthread(void *arg) {

    //Open and check file
    char *filename = (char *) arg;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        pthread_exit(NULL);
    }

    //Initialize graph and checks if it went well, stop if it went wrong
    graph = (graph_t *)get_file_info(file); //graph is global so that each thread can access it
    if (graph == NULL) {
        printf("Error reading graph from file\n");
        pthread_exit(NULL);
    }

    //Decomment this line for debug purposes
    //printf("Number of nodes: %d\n", graph->file_infos->nb_nodes);

    //Fill the buffer with the source nodes and signal the other threads
    for (uint32_t src; src < graph->file_infos->nb_nodes; src++) {
        pthread_mutex_lock(&read_mutex);
        while ((readhead + 1) % BUFFERSIZE == readtail) {
            pthread_cond_wait(&read_not_full, &read_mutex);
        }
        readbuffer[readhead] = src;
        // printf("Read thread: %d\n", src);
        readhead = (readhead + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&read_mutex);

        pthread_cond_signal(&read_not_empty);
    }
    pthread_exit(NULL);
}

//computers done = false / still need to adapt it for the second part 
void *computers(void *arg) {
    uint32_t source;
    bool *read_done = (bool *)arg;

    thread_data_t *data = (thread_data_t *)malloc(sizeof(thread_data_t));
    if (data == NULL) {
        printf("Error: Failed to allocate thread data.\n");
        free(data);
        pthread_exit(NULL);
    }
    while (true){
        pthread_mutex_lock(&read_mutex);
        while (readhead == readtail){
            pthread_cond_wait(&read_not_empty, &read_mutex);

            if (*read_done) {
                pthread_mutex_unlock(&read_mutex);
                pthread_exit(NULL);
            }
        }
        source = readbuffer[readtail];
        readtail = (readtail + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&read_mutex);

        pthread_cond_signal(&read_not_full);

        if (graph == NULL) {
            printf("Error: graph is null\n");
            pthread_exit(NULL);
        }
        ford_t *ford = (ford_t *)bellman_ford(graph->file_infos->nb_nodes, graph->file_infos->nb_edges, graph->graph_data, source, verbose);
        // printf("Distance from %d to %d: %d\n", source, 1, ford->dist[1]);
        mcost_t * max = (mcost_t *)get_max(graph->file_infos->nb_nodes, ford->dist, source);
        // printf("Max cost: %ld\n", max->cost);
        // printf("Max node: %d\n", max->node);
        int32_t size = graph->file_infos->nb_nodes;
        int32_t * path = (int32_t *)get_path(max->node, source, ford->path, &size);
        data->source = source;
        data->max = max;
        data->size = size;
        data->path = path;
        // for (int i = 0; i < size; i++) {
        //     printf("path[%d]: %d\n", i, path[i]);
        // }
        // printf("path size:  %d\n", size);
        // printf("cost: %ld\n", max->cost);
        // printf("cost in data: %ld\n", data->cost);
        free_ford_struct(ford);
        pthread_mutex_lock(&write_mutex);
        while ((writehead + 1) % BUFFERSIZE == writetail) {
            pthread_cond_wait(&write_not_full, &write_mutex);
        }
        writebuffer[writehead] = *data;
        writehead = (writehead + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&write_mutex);

        pthread_cond_signal(&write_not_empty);
        
    }
    pthread_exit(NULL);
}

// writethread done = false / need to rethink the structure + function and then test it 
void *writethread(void *arg){
    thread_data_t *data;
    // printf("hey :)\n");
    bool *write_done = (bool *)arg;
    FILE *filename = fopen("multithreadtest.bin", "w+"); //will just replace with the argument passed
    int wrote;
    while (true) {
        pthread_mutex_lock(&write_mutex);
        while (writehead == writetail) {
            pthread_cond_wait(&write_not_empty, &write_mutex);

            if (*write_done) {
                pthread_mutex_unlock(&write_mutex);
                pthread_exit(NULL);
            }
        }
        data = &writebuffer[writetail];
        // printf("Write thread: \nSource: %u\nMax cost: %ld\nMax node: %u\nSize: %d\n", data->source, data->cost, data->node, data->size);
        writetail = (writetail + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&write_mutex);

        pthread_cond_signal(&write_not_full);

        if (data == NULL) {
            printf("Error: data is null\n");
            pthread_exit(NULL);
        }
        // printf("Write Thread\n");
        // for (int i = 0; i < data->size; i++) {
        //     printf("path[%d]%d \n", i, data->path[i]);
        // }
        wrote = write_to_file(filename, data->source, data->max, data->size, data->path);
        if (wrote == 1) { //TODO: check for potential errors in write_to_file, return 1 in those cases 
            printf("Error writing to file\n");
            pthread_exit(NULL);
        }
        free_max_struct(data->max);
        free_path(data->path);
        printf("Write thread done\n");  
    }
    fclose(filename);
    pthread_exit(NULL);
}

int main(int args, char *argv[]) {
    pthread_t thread, computethread1, computethread2, lastthread;
    pthread_mutex_init(&read_mutex, NULL);
    pthread_cond_init(&read_not_empty, NULL);
    pthread_cond_init(&read_not_full, NULL);
    pthread_mutex_init(&write_mutex, NULL);
    pthread_cond_init(&write_not_empty, NULL);
    pthread_cond_init(&write_not_full, NULL);

    //start producer thread
    char *filename = "tests/graph_bin/default.bin";
    int grph, rep, write;
    grph = pthread_create(&thread, NULL, readthread, (void *) filename);
    if (grph != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    // start consumer thread
    bool read_done = false;
    rep = pthread_create(&computethread1, NULL, computers, (void *)&read_done);
    pthread_create(&computethread2, NULL, computers, (void *)&read_done);
    if (rep != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    bool write_done = false;
    write = pthread_create(&lastthread, NULL, writethread, (void *)&write_done);
    if (write != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    // wait for threads to finish 
    pthread_join(thread, NULL);

    // signal consumer thread to finish
    pthread_mutex_lock(&read_mutex);
    read_done = true;
    pthread_cond_broadcast(&read_not_empty);
    pthread_mutex_unlock(&read_mutex);

    // wait for consumer thread to finish
    pthread_join(computethread1, NULL);
    pthread_join(computethread2, NULL);  
    printf("Done1\n");

    // signal last thread to finish
    pthread_mutex_lock(&write_mutex);
    write_done = true;
    pthread_cond_broadcast(&write_not_empty);
    pthread_mutex_unlock(&write_mutex);
    // wait for last thread to finish
    pthread_join(lastthread, NULL);
    printf("Done2\n");
    //cleanup 
    pthread_mutex_destroy(&read_mutex);
    pthread_cond_destroy(&read_not_empty);
    pthread_cond_destroy(&read_not_full);
    pthread_mutex_destroy(&write_mutex);
    pthread_cond_destroy(&write_not_empty);
    pthread_cond_destroy(&write_not_full);
    free_graph_struct(graph);
    return 0;
}
