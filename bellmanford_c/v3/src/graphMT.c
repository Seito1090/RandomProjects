#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFERSIZE 6
#define NTHREADS 5

uint32_t readbuffer[BUFFERSIZE];
int readhead = 0;
int readtail = 0;

pthread_mutex_t read_done_mutex;
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
    // printf("Number of nodes: %d\n", graph->file_infos->nb_nodes);

    //Fill the buffer with the source nodes and signal the other threads
    for (uint32_t src = 0; src < graph->file_infos->nb_nodes; src++) {
        pthread_mutex_lock(&read_mutex);
        while ((readhead + 1) % BUFFERSIZE == readtail) {
            pthread_cond_wait(&read_not_full, &read_mutex);
        }
        readbuffer[readhead] = src;
        // printf("Read thread: %d\n", src);
        readhead = (readhead + 1) % BUFFERSIZE;
        pthread_cond_broadcast(&read_not_empty);
        pthread_mutex_unlock(&read_mutex);

    }
    pthread_exit(NULL);
}

//computers done = false / still need to adapt it for the second part 
void *computers(void *arg) {
    uint32_t source;
    bool *read_done = (bool *)arg;
    pthread_mutex_lock(&read_done_mutex);
    if (*read_done){
        pthread_mutex_unlock(&read_done_mutex);
        printf("Computers: read thread done\n");
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&read_done_mutex);
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
            // printf("Computers: waiting for read thread : %d\n", *read_done);
            pthread_mutex_lock(&read_done_mutex);
            if (*read_done) {
                pthread_mutex_unlock(&read_done_mutex);
                pthread_mutex_unlock(&read_mutex);
                printf("Computers: read thread done\n");
                free(data);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&read_done_mutex);
        }
        source = readbuffer[readtail];
        readtail = (readtail + 1) % BUFFERSIZE;
        pthread_cond_broadcast(&read_not_full);
        pthread_mutex_unlock(&read_mutex);


        if (graph == NULL) {
            printf("Error: graph is null\n");
            free(data);
            pthread_exit(NULL);
        }

        ford_t *ford = (ford_t *)bellman_ford(graph->file_infos->nb_nodes, graph->file_infos->nb_edges, graph->graph_data, source, verbose);
        mcost_t * max = (mcost_t *)get_max(graph->file_infos->nb_nodes, ford->dist, source);
        int32_t size = graph->file_infos->nb_nodes;
        int32_t * path = (int32_t *)get_path(max->node, source, ford->path, &size);
        data->source = source; data->max = max; data->size = size; data->path = path;
        free_ford_struct(ford);
        pthread_mutex_lock(&write_mutex);
        while ((writehead + 1) % BUFFERSIZE == writetail) {
            pthread_cond_wait(&write_not_full, &write_mutex);
        }
        pthread_mutex_lock(&read_done_mutex);
        if (*read_done && readhead == readtail) {
            pthread_mutex_unlock(&read_done_mutex);
            pthread_mutex_unlock(&write_mutex);
            printf("Computers: read thread done\n");
            free(data);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&read_done_mutex);
        writebuffer[writehead] = *data;
        writehead = (writehead + 1) % BUFFERSIZE;
        pthread_cond_broadcast(&write_not_empty);
        pthread_mutex_unlock(&write_mutex);
        pthread_mutex_lock(&read_done_mutex);
        if (*read_done) {
            // free_max_struct(max);
            // free_path(path);  
            pthread_mutex_unlock(&read_done_mutex);
            free(data);         
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&read_done_mutex);
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
    bool first_pass = true;
    // printf("Write thread entry\n");
    while (true) {
        pthread_mutex_lock(&write_mutex);
        while (writehead == writetail) {
            pthread_cond_wait(&write_not_empty, &write_mutex);
            // printf("Write thread: waiting and done is : %d\n", *write_done);
            if (*write_done) {
                pthread_mutex_unlock(&write_mutex);
                printf("Exitting write thread\n");
                fclose(filename);
                pthread_exit(NULL);
            }
        }
        data = &writebuffer[writetail];
        // printf("Write thread: \nSource: %u\nMax cost: %ld\nMax node: %u\nSize: %d\n", data->source, data->cost, data->node, data->size);
        writetail = (writetail + 1) % BUFFERSIZE;
        pthread_cond_signal(&write_not_full);
        pthread_mutex_unlock(&write_mutex);


        if (data == NULL) {
            printf("Error: data is null\n");
            pthread_exit(NULL);
        }
        if (first_pass) {
            uint32_t nb_nodes_be = htonl(graph->file_infos->nb_nodes);
            if (fwrite(&nb_nodes_be, sizeof(uint32_t), 1, filename) != 1) {
                printf("Error: Failed to write source node.\n");
                pthread_exit(NULL);
            }
            first_pass = false;
        }
        // printf("Write Thread\n");
        // for (int i = 0; i < data->size; i++) {
        //     printf("path[%d]%d \n", i, data->path[i]);
        // }
        // printf("Writing to file, \nsource : %d\ncost : %ld\nnode : %d\nsize : %d\n", data->source, data->max->cost, data->max->node, data->size);
        wrote = write_to_file(filename, data->source, data->max, data->size, data->path);
        if (wrote == 1) { //TODO: check for potential errors in write_to_file, return 1 in those cases 
            printf("Error writing to file\n");
            pthread_exit(NULL);
        }
        free_max_struct(data->max);
        free_path(data->path);
        // printf("Write thread done source : %d\n", data->source);  
        // printf("Write source %d done\n", data->source);
        if(*write_done){
            free_max_struct(data->max);
            free_path(data->path);
            fclose(filename);
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

// void *doublethread(void * arg){
//     //We just split the bellmanford into two threads
//     //We do this because writing and reading from a file is quite quick compared to the 
//     //actual computation of the bellmanford algorithm
// }
int main(int args, char *argv[]) {
    if (NTHREADS <= 0) {
        printf("Error: Invalid number of threads\n");
        return 1;
    } else if (NTHREADS == 1) {
        // do singe thread
        printf("Error: Invalid number of threads\n");
        return 1;
    } else if (NTHREADS == 2) {
        // do double thread
        printf("Error: Invalid number of threads\n");
        return 1;
    } else {
    pthread_t thread[NTHREADS];
    pthread_t reader, writer;
    pthread_mutex_init(&read_done_mutex, NULL);
    pthread_mutex_init(&read_mutex, NULL);
    pthread_cond_init(&read_not_empty, NULL);
    pthread_cond_init(&read_not_full, NULL);
    pthread_mutex_init(&write_mutex, NULL);
    pthread_cond_init(&write_not_empty, NULL);
    pthread_cond_init(&write_not_full, NULL);

    //start producer thread
    char *filename = "tests/graph_bin/default.bin";
    int grph, write;
    grph = pthread_create(&reader, NULL, readthread, (void *) filename);
    if (grph != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    // start consumer thread
    bool read_done = false;
    for (int thrd = 0; thrd < NTHREADS; thrd++) {
        pthread_create(&thread[thrd], NULL, computers, (void *)&read_done);
    }
    bool write_done = false;
    write = pthread_create(&writer, NULL, writethread, (void *)&write_done);
    if (write != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    pthread_join(reader, NULL);
    // wait for threads to finish 
    printf("Done reading, thread 0 joined\n");
    // signal consumer thread to finish
    pthread_mutex_lock(&read_mutex);
    read_done = true;
    pthread_cond_broadcast(&read_not_empty);
    pthread_mutex_unlock(&read_mutex);

    // wait for consumer thread to finish
    for (int thrd = 0; thrd < NTHREADS; thrd++) {
        pthread_join(thread[thrd], NULL);
        // printf("Thread %d joined\n", thrd);
    }

    // signal last thread to finish
    pthread_mutex_lock(&write_mutex);
    write_done = true;
    pthread_cond_broadcast(&write_not_empty);
    pthread_mutex_unlock(&write_mutex);
    // printf("Done1\n");
    printf("Done computers \n");
    // wait for last thread to finish
    // printf("Waiting for last thread %d to join\n", thrd);
    pthread_join(writer, NULL);
    printf("Done last thread joined\n");
    //cleanup 
    pthread_mutex_destroy(&read_done_mutex);
    pthread_mutex_destroy(&read_mutex);
    pthread_cond_destroy(&read_not_empty);
    pthread_cond_destroy(&read_not_full);
    pthread_mutex_destroy(&write_mutex);
    pthread_cond_destroy(&write_not_empty);
    pthread_cond_destroy(&write_not_full);
    free_graph_struct(graph);
    }
    return 0;
}
