#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>


#ifndef LEPL1503__GRAPH__
#define LEPL1503__GRAPH__

typedef struct bin_data{
    uint32_t * graph_data;  
} bin_data_t;

typedef struct file_data{
    uint32_t nb_edges;
    uint32_t nb_nodes;
} file_data_t;

typedef struct graph{
    // TODO: completez cette structure.
    file_data_t * nodes_edges; 
    bin_data_t * graph_info;
} graph_t;



// TODO: declarez les fonctions liees a graph_t.
uint32_t* get_file_info(char* file_name, graph_t* graph);

#endif // LEPL1503__GRAPH__