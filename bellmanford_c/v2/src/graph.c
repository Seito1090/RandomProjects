#include "../include/graph.h"


uint32_t * get_file_info(char* file_name, graph_t* graph){ 
    /*
    Function that returns an array containing the number of nodes and the number of edges as the 2 first 
    values while also having the rest of the binary data contained in the file
    Input :
    char* file_name : the name/location of the file
    graph_t* graph : pointer to the graph structure that stores the information about the graph 
    
    Output:
    uint32_t* toReturn = an array with both the number of nodes and number of edges + the graph data 
    NULL : if there was any error 
    */
    /* Doing not the same as in the python code */

    FILE * file = fopen(file_name, "rb"); if (file == NULL) {perror("Erreur d'ouverture du fichier");return NULL;} //Opens file and checks if it opened correctly
    uint32_t * file_info = (uint32_t *)malloc(2*sizeof(uint32_t)); if (file_info == NULL) {perror("Erreur d'allocation de memoire");return NULL;} //Allocates memory for the basic infos
    uint32_t nodes, edges;
    fread(&nodes, sizeof(uint32_t), 1, file);
    fread(&edges, sizeof(uint32_t), 1, file);
    
    graph->nodes_edges->nb_nodes = be32toh(nodes);
    graph->nodes_edges->nb_edges = be32toh(edges);
    printf(" hey ");
    /* Reading basic info done */ 

    /* Reading the graph branches */
    
    uint32_t* binary_data = (uint32_t*)malloc(graph->nodes_edges->nb_edges * 3 * sizeof(uint32_t));
    if (binary_data == NULL) {
        fclose(file);
        return NULL; // failed to allocate memory
    }
    for (int i = 0; i < 3*graph->nodes_edges->nb_edges; i++){
        unsigned char buffer1[4];   
        if (fread(buffer1, 1, 4, file) != 4) {
            fclose(file);
            free(binary_data);
            return NULL; // failed to read 4 bytes from file
        }
        binary_data[i] = ((uint32_t)buffer1[0] << 24) | ((uint32_t)buffer1[1] << 16) | ((uint32_t)buffer1[2] << 8) | buffer1[3];
    }

    fclose(file);
    return binary_data;
}

int main( int args, char ** argv){
    char * file_name = "graph.bin";
    graph_t* graph;
    uint32_t * extracted_data = get_file_info(file_name, graph);
    printf("hey2");
    return 0;
}