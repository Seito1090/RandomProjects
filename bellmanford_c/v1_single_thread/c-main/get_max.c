#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "belman_ford.c"

float* get_max(int size_dist, float* dist, int source){
    float max_cost = dist[source];
    int max_node = source;
    for (int node_idx = 0; node_idx < size_dist; node_idx++){
        if (node_idx != source && dist[node_idx] != INFINITY && dist[node_idx] >= max_cost){
            if (dist[node_idx] == max_cost && max_node < node_idx){
                continue;
            }
            max_cost = dist[node_idx];
            max_node = node_idx;
        }
    }
    float* max = (float*)malloc(2 * sizeof(float));
    max[0] = max_cost;
    max[1] = (float) max_node;
    return max;
}

// int main(int argc,char** argv){
//     char* file_name = "graph.bin";
//     unsigned int** links = read_graph(file_name);
//     unsigned int nb_nodes = links[0][0];
//     unsigned int nb_edges = links[0][1];
//     float** result = belman_ford(nb_nodes, nb_edges, links, 11, 0, true);
//     for (int i = 0; i < nb_nodes; i++){
//         printf("%f %f\n", result[0][i], result[1][i]);
//     }
//     float* max = get_max(nb_nodes, result[0], 0);
//     int cost = (int)max[0];
//     int node = (int)max[1];
//     printf("max: %d %d\n", cost, node);
//     return 0;
// }






// int main(int argc,char** argv){
//     char* file_name = "graph.bin";
//     unsigned int** links = read_graph(file_name);
//     unsigned int nb_nodes = links[11][0];
//     unsigned int nb_edges = links[11][1];
//     int source = 0;
//     printf("source = %d\n", source);
//     float** result = belman_ford(nb_nodes, nb_edges, links, 11, source, true);
//     printf("Distances from source %d to each node : \n", source);
//     for (int i = 0; i < nb_nodes; i++){
//         printf("%d to node %d \n", (int)result[0][i], i);
//     }
//     float* max = get_max(nb_nodes, result[0], source);
//     int cost = (int)max[0];
//     int node = (int)max[1];
//     printf("Destination node = %d\nCost = %d", node, (int)max[0]);
//     int* path = get_path(node, source, result[1], node);
//     for (int i = 0; i < node; i++){
//         printf("%d \n", path[i]);
//     }
//     return 0;
// }