#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "read_graph.c"

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
int32_t** bellman_ford(int32_t nb_nodes, int32_t nb_edges, int32_t** links, int s, bool verbose){
    //Initialization
    int32_t* dist = (int32_t*)malloc(sizeof(int32_t) * nb_nodes);
    int32_t* path = (int32_t*)malloc(sizeof(int32_t) * nb_nodes);
    for (int i = 0; i < nb_nodes; i++){
        dist[i] = INT32_MAX;
        path[i] = -1;
    }
    dist[s] = 0;
    
    //Main loop for belman ford
    for (int i = 0; i < nb_nodes - 1; i++){
        for (int j = 0; j < nb_edges; j++){
            int32_t node_from = (int32_t)links[j][0];
            int32_t node_to = (int32_t)links[j][1];
            int32_t cost = (int32_t)links[j][2];
            if (dist[node_from] != INT32_MAX && dist[node_to] > dist[node_from] + cost){
                dist[node_to] = dist[node_from] + cost;
                path[node_to] = node_from;
            }
        }
    }

    //Detection of negative cycle
    for (int k = 0; k < nb_edges; k++){
        int32_t node_from = (int32_t)links[k][0];
        int32_t node_to = (int32_t)links[k][1];
        int32_t cost = (int32_t)links[k][2];
        if (dist[node_from] != INT32_MAX && dist[node_to] > dist[node_from] + cost){
            if (verbose){
                printf("Negative cycle detected");
            }
            // Reset distances and paths only for nodes that are reachable from nodes involved in negative cycle
            int start_node = node_to;
            while (start_node != node_from) {
                dist[start_node] = INT32_MAX;
                path[start_node] = -2;
                start_node = path[start_node];
            }
            dist[node_from] = 0;
            path[node_from] = -2;
        }
    }
    int32_t** toReturn = (int32_t**)malloc(sizeof(int32_t*) * 2);
    toReturn[0] = (int32_t*)dist;
    toReturn[1] = (int32_t*)path;
    return toReturn;
}

int main(int argc,char** argv){
    char* file_name = "graph.bin";
    unsigned int** links = read_graph(file_name);
    unsigned int nb_nodes = links[0][0];
    unsigned int nb_edges = links[0][1];
    int32_t** result = bellman_ford(nb_nodes, nb_edges, links, 0, true);
    for (int i = 0; i < nb_nodes; i++){
        printf("%d %d\n", result[0][i], result[1][i]);
    }
    return 0;
}