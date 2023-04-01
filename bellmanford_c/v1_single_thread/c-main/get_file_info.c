#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

unsigned int* get_file_info(char* file_name){ 
    /*
    Function that returns an array containing the number of nodes and the number of edges as the 2 first 
    values while also having the rest of the binary data contained in the file
    Input :
    char* file_name : the name/location of the file
    
    Output:
    unsigned int* toReturn = an array with both the number of nodes and number of edges + the graph data 
    NULL : if there was any error */
    /* Doing not the same as in the python code */
    FILE* file = fopen(file_name, "rb");
    if (file == NULL) {
        return NULL; // failed to open file
    }

    unsigned char buffer[8];
    if (fread(buffer, 1, 8, file) != 8) {
        fclose(file);
        return NULL; // failed to read 8 bytes from file
    }

    unsigned int num_nodes = ((unsigned int)buffer[0] << 24) | ((unsigned int)buffer[1] << 16) | ((unsigned int)buffer[2] << 8) | buffer[3];
    unsigned int num_edges = ((unsigned int)buffer[4] << 24) | ((unsigned int)buffer[5] << 16) | ((unsigned int)buffer[6] << 8) | buffer[7];

    unsigned int* binary_data = (unsigned int*)malloc(num_edges * 3 * sizeof(unsigned int));
    if (binary_data == NULL) {
        fclose(file);
        return NULL; // failed to allocate memory
    }
    for (int i = 0; i < 3*num_edges; i++){
        unsigned char buffer1[4];   
        if (fread(buffer1, 1, 4, file) != 4) {
            fclose(file);
            free(binary_data);
            return NULL; // failed to read 4 bytes from file
        }
        binary_data[i] = ((unsigned int)buffer1[0] << 24) | ((unsigned int)buffer1[1] << 16) | ((unsigned int)buffer1[2] << 8) | buffer1[3];
    }

    fclose(file);
    unsigned int* toReturn = (unsigned int*)malloc(sizeof(unsigned int) * (3*num_edges + 2));
    if (toReturn == NULL) {
        free(binary_data);
        return NULL; // failed to allocate memory
    }

    toReturn[0] = num_nodes;
    toReturn[1] = num_edges;
    for (int i = 0; i < 3*num_edges; i++){
        toReturn[i+2] = binary_data[i];
    }
    free(binary_data);
    return toReturn;
}

// int main(int argc,char** argv){
//     clock_t start, end;
//     double execution_time;
//     start = clock();
//     char* file_name = "graph.bin";
//     unsigned int* binary_data = get_file_info(file_name);
//     if (binary_data != NULL) {
//         printf("Read integers: %u, %u\n", binary_data[0], binary_data[1]);
//         free(binary_data);
//     } else {
//         printf("Failed to read integers from file\n");
//     }
//     end = clock();
//     execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
//     printf("Execution time: %f seconds \n", execution_time);
//     return 0;
// }