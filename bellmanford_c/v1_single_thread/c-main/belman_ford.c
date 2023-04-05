#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "read_graph.c"

// #define INFINITY 1000000
float** belman_ford(unsigned int nb_nodes, unsigned int nb_edges, unsigned int** links, int size_links, int s, bool verbose){
    /*
    This function implements the belman ford algorithm
    Input :
        - nb_nodes : number of nodes in the graph
        - nb_edges : number of edges in the graph
        - links : array of size nb_edges containing the links of the graph
        - s : source node
        - verbose : boolean to print the result or not
    Output :
        - dist : array of size nb_nodes containing the shortest path from s to each node
        - path : array of size nb_nodes containing the previous node in the shortest path from s to each node
        both stored in the same array to save memory
    */
    //Initialization
    float* dist = (float*)malloc(sizeof(float) * nb_nodes);
    float* path = (float*)malloc(sizeof(float) * nb_nodes);
    for (int i = 0; i < nb_nodes; i++){
        dist[i] = INFINITY;
        path[i] = -1;
    }
    dist[s] = 0;
    
    //Main loop for belman ford
    for (int i = 0; i < nb_nodes - 1; i++){
        for (int j = 0; j < nb_edges; j++){
            int node_from = links[j][0];
            int node_to = links[j][1];
            int cost = links[j][2];
            if (dist[node_from] != INFINITY && dist[node_to] > dist[node_from] + cost){
                dist[node_to] = dist[node_from] + cost;
                path[node_to] = node_from;
            }
        }
    }

    //Detection of negative cycle
    for (int k = 0; k < nb_edges; k++){
        int node_from = links[k][0];
        int node_to = links[k][1];
        int cost = links[k][2];
        if (dist[node_from] != INFINITY && dist[node_to] > dist[node_from] + cost){
            if (verbose){
                printf("Negative cycle detected");
            }
            // Reset distances and paths only for nodes that are reachable from nodes involved in negative cycle
            int start_node = node_to;
            while (start_node != node_from) {
                dist[start_node] = INFINITY;
                path[start_node] = -2;
                start_node = path[start_node];
            }
            dist[node_from] = 0;
            path[node_from] = -2;
        }
    }
    float** toReturn = (float**)malloc(sizeof(float*) * 2);
    toReturn[0] = dist;
    toReturn[1] = path;
    return toReturn;
}

// int main(int argc,char** argv){
//     char* file_name = "graph.bin";
//     unsigned int** links = read_graph(file_name);
//     unsigned int nb_nodes = links[0][0];
//     unsigned int nb_edges = links[0][1];
//     float** result = belman_ford(nb_nodes, nb_edges, links, nb_edges, 1, true);
//     for (int i = 0; i < nb_nodes; i++){
//         printf("%f %f\n", result[0][i], result[1][i]);
//     }
//     return 0;
// }