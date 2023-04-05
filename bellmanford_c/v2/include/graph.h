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
    // TODO: completez cette structure.
    file_data_t * file_infos; 
    uint32_t * graph_data;
} graph_t;



// TODO: declarez les fonctions liees a graph_t.
graph_t * get_file_info(char* file_name);

#endif // LEPL1503__GRAPH__