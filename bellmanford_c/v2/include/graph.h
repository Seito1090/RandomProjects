#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#ifndef LEPL1503__GRAPH__
#define LEPL1503__GRAPH__

typedef struct file_data{
    uint32_t nb_edges;
    uint32_t nb_nodes;
} file_data_t;

typedef struct graph{
    file_data_t * file_infos; 
    uint32_t * graph_data;
} graph_t;

typedef struct ford{
    int32_t * dist;
    int32_t * path;
}ford_t;

typedef struct mcost{
    int32_t cost;
    int32_t node;
} mcost_t;


graph_t * get_file_info(char* file_name);

void free_graph_struct(graph_t* graph);

ford_t* bellman_ford(int32_t nb_nodes, int32_t nb_edges, int32_t* links,int source, bool verbose);

void free_ford_strtuct(ford_t * ford);

mcost_t * get_max(int nb_nodes, int32_t * dist, int source);

void free_max_strct(mcost_t * mcost);

int32_t * get_path(int dest, int source, int32_t* path, int32_t* size);

void free_path(int32_t * path);

#endif // LEPL1503__GRAPH__