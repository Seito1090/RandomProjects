#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "get_file_info.c"

unsigned int** read_graph(char* file_name){
    /*
    Function that reads the graph from the file and returns it as a matrix.
    Input :
    char* file_name : the name/location of the file
    Output:
    unsigned int** links : the graph as a matrix WITH nb of nodes and edges at the very end
    */
    unsigned int* binary_data = get_file_info(file_name);
    unsigned int** links = (unsigned int**)malloc((binary_data[1] + 1)* sizeof(unsigned int*));
    unsigned int* infos = (unsigned int*)malloc(2 * sizeof(unsigned int));
    infos[0] = binary_data[0];
    infos[1] = binary_data[1];
    links[0] = infos;
    for (int i = 0; i < binary_data[1]*3; i+=3) {
        unsigned int* edge = (unsigned int*)malloc(3 * sizeof(unsigned int));
        edge[0] = binary_data[i+2];
        edge[1] = binary_data[i+3];
        edge[2] = binary_data[i+4];
        links[i/3+1] = edge;
    }
    return links;
}

// int main(int argc,char** argv){
//     char* file_name = "graph.bin";
//     unsigned int** links = read_graph(file_name);
//     unsigned int nb_nodes = links[11][0];
//     unsigned int nb_edges = links[11][1];
//     for (int i = 0; i < nb_edges; i++){
//         printf("%u %u %u\n", links[i][0], links[i][1], links[i][2]);
//     }
//     return 0;
// }