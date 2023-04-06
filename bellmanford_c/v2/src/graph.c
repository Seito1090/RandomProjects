#include "../include/graph.h"

/*
Function get_file_info
-------------------------------------

Function that returns the pointer to the graph_t structure of the graph that will be worked on
This structure contains, the number of edges and nodes that the graph but also the branches that link the nodes

Input:
----------
char* file_name : the name/location of the file

Output:
----------
graph_t * graph = the mentionned structure
NULL + perror message if there was an error
*/
graph_t * get_file_info(char* file_name){ 
    FILE * file = fopen(file_name, "rb"); if (file == NULL) {perror("Could not open the graph file");return NULL;} //Opens file and checks if it opened correctly
    // Create the structure in which the graph will be stored + checks
    graph_t* graph = malloc(sizeof(graph_t));
    if (graph == NULL) {fclose(file);perror("Could not allocate memory for the graph, free memory and try again");return NULL;};
    graph->file_infos = malloc(sizeof(file_data_t));
    if (graph->file_infos == NULL) {fclose(file);free(graph->graph_data);free(graph);
        perror("Could not allocate memory for file_infos, free memory and try again");return NULL;}
    /* 
    This works on Linux/ MacOs but not on windows 
    TODO: make sure that it works on all things without issues probably just something to do with the header files, like declare what system it is and the use the corresponding headers 
    */
    // int32_t* nodes, edges;
    // fread(&nodes, sizeof(int32_t), 1, file);
    // fread(&edges, sizeof(int32_t), 1, file);
    // graph->nodes_edges->nb_nodes = be32toh(nodes);
    // graph->nodes_edges->nb_edges  = be32toh(edges);
    
    /* Reading basic info done */ 

    /* Failsafe method but slower ? */
    
    unsigned char buffer[8]; // Buffer for the bits in the file + check
    if (fread(buffer, 1, 8, file) != 8) {fclose(file); return NULL;} 
    // Big-edian -> host
    int32_t nb_nodes = ((int32_t)buffer[0] << 24) | ((int32_t)buffer[1] << 16) | ((int32_t)buffer[2] << 8) | buffer[3];
    int32_t nb_edges = ((int32_t)buffer[4] << 24) | ((int32_t)buffer[5] << 16) | ((int32_t)buffer[6] << 8) | buffer[7];

    /* If the graph is empty we do not need to contiue */
    if (nb_nodes <= 0 || nb_edges <= 0){perror("Graph vide ou mal construit"); return NULL;}
    
    //------------------------------------------------------

    /* Reading the graph branches */
    
    graph->graph_data = malloc(nb_edges * sizeof(branch_t));
    if (graph->graph_data == NULL) {fclose(file);free(graph);
        perror("Could not allocate memory for graph_data, free memory and try again");return NULL;}
    
    graph->graph_data->cost = malloc(nb_edges * sizeof(int32_t));
    if (graph->graph_data->cost == NULL) {fclose(file);free(graph);
        perror("Could not allocate memory for graph_data structure, free memory and try again");return NULL;}

    graph->graph_data->node_from = malloc(nb_edges * sizeof(uint32_t));
    if (graph->graph_data->node_from == NULL) {fclose(file);free(graph);
        perror("Could not allocate memory for graph_data structure, free memory and try again");return NULL;}
    
    graph->graph_data->node_to = malloc(nb_edges * sizeof(uint32_t));
    if (graph->graph_data->node_to == NULL) {fclose(file);free(graph);
        perror("Could not allocate memory for graph_data structure, free memory and try again");return NULL;}
    
    for (int i = 0; i < nb_edges; i++){
    unsigned char buffer1[4]; // Buffer + checks for branches
    if (fread(buffer1, 1, 4, file) != 4) {fclose(file);free(graph->graph_data);return NULL;}
    graph->graph_data->node_from[i] = ((uint32_t)buffer1[0] << 24) | ((uint32_t)buffer1[1] << 16) | ((uint32_t)buffer1[2] << 8) | buffer1[3];

    if (fread(buffer1, 1, 4, file) != 4) {fclose(file);free(graph->graph_data);return NULL;}
    graph->graph_data->node_to[i] = ((uint32_t)buffer1[0] << 24) | ((uint32_t)buffer1[1] << 16) | ((uint32_t)buffer1[2] << 8) | buffer1[3];

    if (fread(buffer1, 1, 4, file) != 4) {fclose(file);free(graph->graph_data);return NULL;}
    graph->graph_data->cost[i] = ((int32_t)buffer1[0] << 24) | ((int32_t)buffer1[1] << 16) | ((int32_t)buffer1[2] << 8) | buffer1[3];
}

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
    free(graph->graph_data->cost);
    free(graph->graph_data->node_from);
    free(graph->graph_data->node_to);
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
uint32_t nb_nodes : number of nodes in the graph
uint32_t nb_edges : number of edges in the graph
branch_t * links : array of size nb_edges containing the links of the graph
uint32_t s : source node
bool verbose : boolean to print the result or not

Output:
----------
ford_t * ford_links : structure that keeps track of distances between the source and each other node, the path between the source and the other nodes 
NULL + perror message if there was an error
*/
ford_t* bellman_ford(uint32_t nb_nodes, uint32_t nb_edges, branch_t * links, uint32_t s, bool verbose){
    // Initialization
    ford_t* ford_links = (ford_t*)malloc(sizeof(ford_t)); 
    if (ford_links == NULL){perror("Erreur d'allocation de mémoire pour la structure de bellman_ford"); return NULL;}
    int32_t* distances = (int32_t*)malloc(sizeof(int32_t) * nb_nodes);
    if (distances == NULL){perror("Erreur d'allocation de mémoire pour la structure de bellman_ford"); return NULL;}
    int32_t* paths = (int32_t*)malloc(sizeof(int32_t) * nb_nodes);
    if (paths == NULL){perror("Erreur d'allocation de mémoire pour la structure de bellman_ford"); return NULL;}

    for (int i = 0; i < nb_nodes; i++){
        distances[i] = INT32_MAX;
        paths[i] = -1;
    }
    distances[s] = 0;
    
    // Main loop for belman ford
    for (int i = 0; i < nb_nodes-1; i++) {
        bool no_update = false; // Allows us to terminate the loop if we see that it has no reason to continue 
        for (int j = 0; j < nb_edges; j++) {
            uint32_t node_from = links->node_from[j];
            uint32_t node_to = links->node_to[j];
            int32_t cost = links->cost[j];
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

    // Detection of negative cycle
    for (int k = 0; k < nb_edges; k+=3){
        uint32_t node_from = links->node_from[k];
        uint32_t node_to = links->node_to[k];
        int32_t cost = links->cost[k];
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
int32_t nb_nodes : the number of nodes in the graph
int32_t * dist : pointer to the array that stores the distances from each node to each node
uint32_t soucre : the source node 

Output:
----------
mcost_t * max : the structure that stores the node and cost
NULL + perror message if there was an error
*/
mcost_t * get_max(int32_t nb_nodes, int32_t * dist, uint32_t source){
    mcost_t * max = (mcost_t *)malloc(sizeof(mcost_t));
    if (max == NULL){perror("Could not allocate memory in the get_max function");return NULL;}
    int64_t max_cost = (uint64_t)dist[source];
    uint32_t max_node = source;
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

/*
Function get_path
-------------------------------------
This function returns the path from the source node to the destination node given the path array that is provided

Input:
----------
uint32_t dest : the destination node 
uint32_t source : the source node 
int32_t * path : pointer to the array that stores the paths from each node to each node
int32_t * size : pointer that keeps track of the lenght of the path  

Output:
----------
mcost_t * max : the structure that stores the node and cost
NULL + perror message if there was an error
*/
int32_t* get_path(uint32_t dest, uint32_t source, int32_t* path, int32_t* size) {
    int32_t* the_path = (int32_t*) calloc(*size, sizeof(int32_t));
    if (the_path == NULL) {
        perror("Could not allocate memory in the get_path function");
        return NULL;
    }

    uint32_t i = dest;
    uint32_t the_path_indx = 0;

    // Create the path array starting from the destination back to the source
    while (i != source) {
        the_path[the_path_indx++] = i;
        i = path[i];
    }

    the_path[the_path_indx++] = source;
    *size = the_path_indx;

    // Reverse the array to return the path to take in the right order, source -> destination 
    for (uint32_t j = 0; j < the_path_indx / 2; j++) {
        uint32_t temp = the_path[j];
        the_path[j] = the_path[the_path_indx - j - 1];
        the_path[the_path_indx - j - 1] = temp;
    }

    return the_path;
}


/*
Function free_path
-------------------------------------

Function used to free the memory allocated to the path array
*/
void free_path(int32_t * path){
    free(path);
}
//---------------------------------------------------------------------------------------------------------


int main(int args, char ** argv){
    clock_t start, end;
    double execution_time;
    start = clock();
    bool verbose = true;
    char * file_name = "graph.bin";
    graph_t * graph = get_file_info(file_name);
    if (graph == NULL){printf("get file info failed\n");return 1;}
    for (uint32_t source = 0; source < graph->file_infos->nb_nodes; source++){
        ford_t * result = bellman_ford(graph->file_infos->nb_nodes, graph->file_infos->nb_edges, graph->graph_data, source, verbose);
        if (result == NULL){printf("bellmand failed\n");return 1;}
        printf("source node : %u\nDistances : [", source);
        for (int i = 0; i < graph->file_infos->nb_nodes; i++){
            printf(" %d", result->dist[i]);
        }
        mcost_t * max = get_max(graph->file_infos->nb_nodes, result->dist, source);
        if (max == NULL){printf("get max failed\n");return 1;}
        int32_t size = graph->file_infos->nb_nodes;
        int32_t * path = get_path(max->node, source, result->path, &size);
        if (path == NULL){printf("get path failed\n");return 1;}
        printf("]\n    Destination : %u\n    Cost : %ld\n    Number of nodes : %d\n    Path: ", max->node, max->cost, size);
        for (int j = 0; j < size; j++){
            printf(" %d ",path[j]);
        }
        printf("\n");
        free_path(path);
        free_max_strct(max);
        free_ford_struct(result);
    }
    free_graph_struct(graph);
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Execution time : %f\n", execution_time);
    return 0;
}