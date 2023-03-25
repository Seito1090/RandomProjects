#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "get_max.c"

int* get_path(int dest, int source, float* path, int nb_nodes){
    /*
    This function returns an array containing the path from the source to dest node using the path array
    Input :
        - dest : destination node
        - source : source node
        - path : array of size nb_nodes containing the previous node in the shortest path from s to each node
    Output :
        - r : array containing the path from the source to dest node
    */
    int* r = (int*)malloc(sizeof(int));
    r[0] = dest;
    int i = dest;
    int len_r = 1;
    while (i != source) {
        len_r++;
        r = (int*)realloc(r, len_r * sizeof(int));
        for (int j = len_r - 1; j > 0; j--) {
            r[j] = r[j - 1];
        }
        r[0] = path[i];
        i = path[i];
    }
    r= (int*)realloc(r, (len_r+1) * sizeof(int));
    for (int j = len_r; j > 0; j--) {
        r[j] = r[j - 1];
    }
    r[0] = len_r;
    return r;
}

int main(int argc,char** argv){
    char* file_name = "graph.bin";
    unsigned int** links = read_graph(file_name);
    // printf("nb_nodes : %d\n", links[size-1][0]);
    unsigned int nb_nodes = links[0][0];
    unsigned int nb_edges = links[0][1];
    for (int source = 0; source < nb_nodes; source++){
        printf("Source : %d\n", source);
        float** result = belman_ford(nb_nodes, nb_edges, links, nb_edges, source, true);
        printf("Distances : [");
        for (int i = 0; i < nb_nodes; i++){
            printf(" %d ", (int)result[0][i]);
        }
        printf("]\n");
        float* max = get_max(nb_nodes, result[0], source);
        int cost = (int)max[0];
        int node = (int)max[1];
        printf("    Destination: %d \n    Cost: %d\n", cost, node);
        int* path = get_path(node, source, result[1], nb_nodes);
        int number_of_nodes = path[0];
        printf("    Number of nodes in the path: %d \n", number_of_nodes);
        printf("    Path to take : [");
        for (int i = 1; i < number_of_nodes+1; i++){
            printf(" %d ", path[i]);
        }
        printf("]\n");
        printf("--------------------------------------------------\n");
        free(result);
        }
    return 0;
}