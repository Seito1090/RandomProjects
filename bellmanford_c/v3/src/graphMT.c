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
        printf("Read thread: %d\n", src);
        readhead = (readhead + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&read_mutex);

        pthread_cond_signal(&read_not_empty);
    }
    pthread_exit(NULL);
}

//computers done = false / still need to adapt it for the second part 
void *computers(void *arg) {
    uint32_t source;
    bool* done = (bool *)arg;
    thread_data_t *data = (thread_data_t *)malloc(sizeof(thread_data_t));
    while (true){
        pthread_mutex_lock(&read_mutex);
        while (readhead == readtail){
            pthread_cond_wait(&read_not_empty, &read_mutex);

            if (*done) {
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
        printf("Distance from %d to %d: %d\n", source, 1, ford->dist[1]);
        mcost_t * max = (mcost_t *)get_max(graph->file_infos->nb_nodes, ford->dist, source);
        printf("Max cost: %ld\n", max->cost);
        printf("Max node: %d\n", max->node);
        int32_t size = graph->file_infos->nb_nodes;
        int32_t * path = (int32_t *)get_path(max->node, source, ford->path, &size);
        data->source = source;
        data->max = max;
        data->size = size;
        data->path = path;
        printf("path size:  %d\n", size);
        free_path(path);
        free_max_struct(max);
        free_ford_struct(ford);
    }

    pthread_exit(NULL);
}

//writethread done = false / need to rethink the structure + function and then test it 
// void *writethread(void *arg){
//     FILE *filename = fopen("multithreadtest.bin", "w+");
//     thread_data_t *data = (thread_data_t *)arg;
//     if (data == NULL) {
//         printf("Error: data is null\n");
//         pthread_exit(NULL);
//     }
//     int wrote;
//     wrote = write_to_file(filename, data->source, data->max, data->size, data->path);
//     if (wrote == 1) { //TODO: check for potential errors in write_to_file, return 1 in those cases 
//         printf("Error writing to file\n");
//         pthread_exit(NULL);
//     }
//     free(data);
//     fclose(filename);
//     pthread_exit(NULL);
// }

int main(int args, char *argv[]) {
    pthread_t thread, computethread1, computethread2;
    pthread_mutex_init(&read_mutex, NULL);
    pthread_cond_init(&read_not_empty, NULL);
    pthread_cond_init(&read_not_full, NULL);

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
    pthread_mutex_lock(&read_mutex);
    done = true;
    pthread_cond_broadcast(&read_not_empty);
    pthread_mutex_unlock(&read_mutex);

    // wait for consumer thread to finish
    pthread_join(computethread1, NULL);
    pthread_join(computethread2, NULL);  

    //pthread_create(&lastthread, NULL, writethread, NULL);

    //cleanup 
    pthread_mutex_destroy(&read_mutex);
    pthread_cond_destroy(&read_not_empty);
    pthread_cond_destroy(&read_not_full);
    free_graph_struct(graph);
    return 0;
}
