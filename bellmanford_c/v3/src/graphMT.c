#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


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
    if (graph == NULL) {
        printf("Error reading graph from file\n");
        pthread_exit(NULL);
    }
    printf("Number of nodes: %d\n", graph->file_infos->nb_nodes);
    pthread_exit(NULL);
}

void *computers(void *arg) {
    uint32_t source = *(uint32_t *)arg;
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
    free_path(path);
    free_max_struct(max);
    free_ford_struct(ford);
    pthread_exit(NULL);
}

void *writethread(void *arg){
    char *filename = (char *)arg[];
    pthread_exit(NULL);
}

int main(int args, char *argv[]) {
    char *filename = "tests/graph_bin/default.bin";
    int grph, rep;
    pthread_t thread[3];
    grph = pthread_create(&thread[0], NULL, readthread, (void *) filename);
    if (grph != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    pthread_join(thread[0], NULL);
    uint32_t source = 0;
    rep = pthread_create(&thread[1], NULL, computers, (void *)&source);
    if (rep != 0) {
        printf("Error creating thread\n");
        return 1;
    }
    pthread_join(thread[1], NULL);

    free_graph_struct(graph);
    return 0;
}
