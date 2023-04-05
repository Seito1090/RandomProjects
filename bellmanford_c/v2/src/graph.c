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
    graph->graph_data = malloc(sizeof(uint32_t *));
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
    // uint32_t* nodes, edges;
    // fread(&nodes, sizeof(uint32_t), 1, file);
    // fread(&edges, sizeof(uint32_t), 1, file);
    // graph->nodes_edges->nb_nodes = be32toh(nodes);
    // graph->nodes_edges->nb_edges  = be32toh(edges);
    /* Reading basic info done */ 

    /* Failsafe method but slow */
    
    unsigned char buffer[8]; // buffer for the bits in the file + check
    if (fread(buffer, 1, 8, file) != 8) {fclose(file); return NULL;} 
    //big-edian -> host
    unsigned int nb_nodes = ((unsigned int)buffer[0] << 24) | ((unsigned int)buffer[1] << 16) | ((unsigned int)buffer[2] << 8) | buffer[3];
    unsigned int nb_edges = ((unsigned int)buffer[4] << 24) | ((unsigned int)buffer[5] << 16) | ((unsigned int)buffer[6] << 8) | buffer[7];

    /* If the graph is empty we do not need to contiue */
    if (nb_nodes == 0){perror("Graph vide ou mal construit"); return NULL;}
    
    //------------------------------------------------------

    /* Reading the graph branches */
    
    uint32_t* binary_data = (uint32_t*)malloc(nb_edges * 3 * sizeof(uint32_t));
    if (binary_data == NULL) {fclose(file);return NULL;}
    for (int i = 0; i < 3*nb_edges; i++){
        unsigned char buffer1[4]; //buffer + checks for branches
        if (fread(buffer1, 1, 4, file) != 4) {fclose(file);free(binary_data);return NULL;}
        binary_data[i] = ((uint32_t)buffer1[0] << 24) | ((uint32_t)buffer1[1] << 16) | ((uint32_t)buffer1[2] << 8) | buffer1[3];
    }
    graph->graph_data = binary_data;
    fclose(file);
    graph->file_infos->nb_nodes = nb_nodes;
    graph->file_infos->nb_edges = nb_edges;
    return graph;
}

int main(int args, char ** argv){
    clock_t start, end;
    double execution_time;
    start = clock();
    char * file_name = "graph.bin";
    graph_t * graph = get_file_info(file_name);
    printf("nodes : %u, edges : %u\n", graph->file_infos->nb_nodes, graph->file_infos->nb_edges);
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Execution time : %f\n", execution_time);
    return 0;
}