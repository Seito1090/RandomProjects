#ifndef LEPL1503__GRAPH__
#define LEPL1503__GRAPH__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

typedef struct branch{
    uint32_t * node_from;
    uint32_t * node_to;
    int32_t * cost;
} branch_t;

typedef struct file_data{
    uint32_t nb_edges;
    uint32_t nb_nodes;
} file_data_t;

typedef struct graph{
    file_data_t * file_infos; 
    branch_t * graph_data;
} graph_t;

typedef struct ford{
    int32_t * dist;
    int32_t * path;
} ford_t;

typedef struct mcost{
    int64_t cost;
    uint32_t node;
} mcost_t;


graph_t * get_file_info(FILE * file);

void free_graph_struct(graph_t* graph);

ford_t* bellman_ford(uint32_t nb_nodes, uint32_t nb_edges, branch_t* links, uint32_t source, bool verbose);

void free_ford_struct(ford_t * ford);

mcost_t * get_max(int32_t nb_nodes, int32_t * dist, uint32_t source);

void free_max_struct(mcost_t * mcost);

int32_t * get_path(uint32_t dest, uint32_t source, int32_t* path, int32_t* size);

void free_path(int32_t * path);

#endif // LEPL1503__GRAPH__