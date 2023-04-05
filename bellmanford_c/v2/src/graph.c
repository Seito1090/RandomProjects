#include "../include/graph.h"

/*
Function get_file_info
-------------------------------------

Function that returns the pointer to the graph_t structure of the graph that will be worked on
This structure contains, the number of edges and nodes that the graph but also the branches that link the nodes

Input:
----------
char* file_name : the name/location of the file
graph_t* graph : pointer to the graph structure that stores the information about the graph 

Output:
----------
graph_t * graph = the mentionned structure
NULL : if there was any error 
*/
graph_t * get_file_info(char* file_name){ 
    /* Doing not the same as in the python code */
    FILE * file = fopen(file_name, "rb"); if (file == NULL) {perror("Erreur d'ouverture du fichier");return NULL;} //Opens file and checks if it opened correctly
    //create the structure in which the graph will be stored + checks
    graph_t* graph = malloc(sizeof(graph_t));
    if (graph == NULL) {fclose(file);perror("Could not allocate memory for the graph, free memory and try again");return NULL;};
    graph->graph_data = malloc(sizeof(int32_t *));
    if (graph->graph_data == NULL) {fclose(file);free(graph);
        perror("Could not allocate memory for nodes_edges, free memory and try again");
        return NULL;
    }
    graph->file_infos = malloc(sizeof(file_data_t));
    if (graph->file_infos == NULL) {fclose(file);free(graph->graph_data);free(graph);
        perror("Could not allocate memory for file_infos, free memory and try again");
        return NULL;
    }
    /* 
    This works on Linux/ MacOs but not on windows 
    TODO: make sure that it works on all things without issues
    */
    // int32_t* nodes, edges;
    // fread(&nodes, sizeof(int32_t), 1, file);
    // fread(&edges, sizeof(int32_t), 1, file);
    // graph->nodes_edges->nb_nodes = be32toh(nodes);
    // graph->nodes_edges->nb_edges  = be32toh(edges);
    /* Reading basic info done */ 

    /* Failsafe method but slower ? */
    
    unsigned char buffer[8]; // buffer for the bits in the file + check
    if (fread(buffer, 1, 8, file) != 8) {fclose(file); return NULL;} 
    //big-edian -> host
    int32_t nb_nodes = ((int32_t)buffer[0] << 24) | ((int32_t)buffer[1] << 16) | ((int32_t)buffer[2] << 8) | buffer[3];
    int32_t nb_edges = ((int32_t)buffer[4] << 24) | ((int32_t)buffer[5] << 16) | ((int32_t)buffer[6] << 8) | buffer[7];

    /* If the graph is empty we do not need to contiue */
    if (nb_nodes == 0){perror("Graph vide ou mal construit"); return NULL;}
    
    //------------------------------------------------------

    /* Reading the graph branches */
    
    int32_t* binary_data = (int32_t*)malloc(nb_edges * 3 * sizeof(int32_t));
    if (binary_data == NULL) {fclose(file);return NULL;}
    for (int i = 0; i < 3*nb_edges; i++){
        unsigned char buffer1[4]; //buffer + checks for branches
        if (fread(buffer1, 1, 4, file) != 4) {fclose(file);free(binary_data);return NULL;}
        binary_data[i] = ((int32_t)buffer1[0] << 24) | ((int32_t)buffer1[1] << 16) | ((int32_t)buffer1[2] << 8) | buffer1[3];
    }
    graph->graph_data = binary_data;
    fclose(file);
    graph->file_infos->nb_nodes = nb_nodes;
    graph->file_infos->nb_edges = nb_edges;
    return graph;
}
/*
Function free_graph_struct
-------------------------------------

Function used to free the memory allocated to the graph structure that was used
*/
void free_graph_struct(graph_t* graph){
    free(graph->file_infos);
    free(graph->graph_data);
    free(graph);
}
//---------------------------------------------------------------------------------------------------------

/*
Function bellman_ford
-------------------------------------
This function implements the bellman ford algorithm, it finds the shortest path between 2 nodes in a graph.
This algorithm also works when negative values are introduced for the cost between 2 nodes.

Input:
----------
int32_t nb_nodes : number of nodes in the graph
int32_t nb_edges : number of edges in the graph
int32_t ** links : array of size nb_edges containing the links of the graph
int s : source node
bool verbose : boolean to print the result or not

Output:
----------
ford_t * ford_links : structure that keeps track of distances between the source and each other node, the path between the source and the other nodes 
*/
ford_t* bellman_ford(int32_t nb_nodes, int32_t nb_edges, int32_t * links, int s, bool verbose){
    //Initialization
    ford_t* ford_links = malloc(sizeof(ford_t)); if (ford_links == NULL){perror("Erreur d'allocation de mémoire pour la structure de bellman_ford"); return NULL;};
    int32_t* distances = (int32_t*)malloc(sizeof(int32_t) * nb_nodes);
    int32_t* paths = (int32_t*)malloc(sizeof(int32_t) * nb_nodes);
    for (int i = 0; i < nb_nodes; i++){
        distances[i] = INT32_MAX;
        paths[i] = -1;
    }
    distances[s] = 0;
    
    //Main loop for belman ford
    for (int i = 0; i < nb_nodes-1; i++) {
        bool no_update = false;
        for (int j = 0; j < nb_edges*3; j+=3) {
            int32_t node_from = links[j];
            int32_t node_to = links[j+1];
            int32_t cost = links[j+2];
            if (distances[node_from] != INT32_MAX && distances[node_to] > distances[node_from] + cost) {
                distances[node_to] = distances[node_from] + cost;
                paths[node_to] = node_from;
                no_update = true;
            }
        }
        if (!no_update) {
            break;
        }
    }

    //Detection of negative cycle
    for (int k = 0; k < nb_edges*3; k+=3){
        int32_t node_from = links[k];
        int32_t node_to = links[k+1];
        int32_t cost = links[k+2];
        if (distances[node_from] != INT32_MAX && distances[node_to] > distances[node_from] + cost){
            if (verbose){
                printf("Negative cycle detected");
            }
            // Reset distancesances and pathss only for nodes that are reachable from nodes involved in negative cycle
            int start_node = node_to;
            while (start_node != node_from) {
                distances[start_node] = INT32_MAX;
                paths[start_node] = -2;
                start_node = paths[start_node];
            }
            distances[node_from] = 0;
            paths[node_from] = -2;
        }
    }
    ford_links->dist = distances;
    ford_links->path = paths;
    return ford_links;
}

/*
Function free_ford_struct
-------------------------------------

Function used to free the memory allocated to the ford structure that was used
*/
void free_ford_struct(ford_t* ford){
    free(ford->dist);
    free(ford->path);
    free(ford);
}
//---------------------------------------------------------------------------------------------------------

/*
Function get_max
-------------------------------------
This function returns us the most "expensive node" to reach as a structure that stores the node and the cost

Input:
----------
int nb_nodes : the number of nodes in the graph
int32_t * dist : pointer to the array that stores the distances from each node to each node
int soucre : the source node 

Output:
----------
mcost_t * max : the structure that stores the node and cost
*/
mcost_t * get_max(int nb_nodes, int32_t * dist, int source){
    mcost_t * max = malloc(sizeof(mcost_t));
    int32_t max_cost = dist[source];
    int max_node = source;
    for (int node_idx = 0; node_idx < nb_nodes; node_idx++){
        if (node_idx != source && dist[node_idx] != INT32_MAX && dist[node_idx] >= max_cost){
            if (dist[node_idx] == max_cost && max_node < node_idx){
                continue;
            }
            max_cost = dist[node_idx];
            max_node = node_idx;
        }
    }
    max->cost = max_cost;
    max->node = max_node;
    return max;
}

/*
Function free_max_struct
-------------------------------------

Function used to free the memory allocated to the mcost structure that was used
*/
void free_max_strct(mcost_t * mcost){
    free(mcost);
}
//---------------------------------------------------------------------------------------------------------


int main(int args, char ** argv){
    bool verbose;
    int source = 0;
    clock_t start, end;
    double execution_time;
    start = clock();
    char * file_name = "graph.bin";
    graph_t * graph = get_file_info(file_name);
    printf("nodes : %u, edges : %u\n", graph->file_infos->nb_nodes, graph->file_infos->nb_edges);
    ford_t * result = bellman_ford(graph->file_infos->nb_nodes, graph->file_infos->nb_edges, graph->graph_data, source, true);
    mcost_t * max = get_max(graph->file_infos->nb_nodes, result->dist, source);
    printf("node : %d, cost : %d \n", max->node, max->cost);
    free_max_strct(max);
    free_ford_struct(result);
    free_graph_struct(graph);
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Execution time : %f\n", execution_time);
    return 0;
}