#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned int* get_file_info(char* file_name){ 
    /*
    Function that returns an array containing the number of nodes and the number of edges as the 2 first 
    values while also having the rest of the binary data contained in the file
    Input :
    char* file_name : the name/location of the file
    
    Output:
    unsigned int* toReturn = an array with both the number of nodes and number of edges
    NULL : if there was any error */
    /* Doing not the same as in the python code */
    FILE* file = fopen(file_name, "rb");if (file == NULL) {/*printf("Failed to open file\n")*/;return NULL; /* failed to open file */}
    
    /*Getting the nodes and edges infos*/

    unsigned char buffer[8];
    if (fread(buffer, 1, 8, file) != 8) {
        fclose(file);
        /*printf("Failed to read 8 bytes from file\n");*/
        return NULL; // failed to read 8 bytes from file
    }
    unsigned int* infos = (unsigned int*)malloc(2 * sizeof(unsigned int));
    if (infos == NULL) {
        /*printf("Failed to allocate memory\n");*/
        return NULL; // failed to allocate memory
    }
    infos[0] = ((unsigned int)buffer[0] << 24) | ((unsigned int)buffer[1] << 16) | ((unsigned int)buffer[2] << 8) | buffer[3];
    infos[1] = ((unsigned int)buffer[4] << 24) | ((unsigned int)buffer[5] << 16) | ((unsigned int)buffer[6] << 8) | buffer[7];
    
    /*Once done we can create an array to store the rest of the graph (now we know the lenght of the array we have to create)*/
    unsigned char* binary_data = (unsigned char*)malloc(infos[1] * sizeof(unsigned int));
    if (binary_data == NULL) {
        // printf("Failed to allocate memory\n");
        return NULL; // failed to allocate memory
    }
    unsigned char buffer1[4];
    for (int i = 0; i < 3*infos[1]; i++){
        if (fread(buffer1, 1, 4, file) != 4) {
            fclose(file);
            //printf("Failed to read 4 bytes from file\n");
            return NULL; // failed to read 4 bytes from file
        }
        binary_data[i] = ((unsigned int)buffer1[0] << 24) | ((unsigned int)buffer1[1] << 16) | ((unsigned int)buffer1[2] << 8) | buffer1[3];
        //printf("%u ", binary_data[i]);
    }
    fclose(file);
    unsigned int* toReturn = (unsigned int*)malloc(sizeof(unsigned int) * (3*infos[1] + 2));
    if (toReturn == NULL) {
        //printf("Failed to allocate memory\n");
        return NULL; // failed to allocate memory
    }
    toReturn[0] = infos[0];
    toReturn[1] = infos[1];
    for (int i = 0; i < 3*infos[1]; i++){
        toReturn[i+2] = binary_data[i];
    }
    free(infos);
    free(binary_data);
    return toReturn;
}

// int main(int argc,char** argv){
//     char* file_name = "graph.bin";
//     unsigned int* binary_data = get_file_info(file_name);
//     if (binary_data != NULL) {
//         printf("Read integers: %u, %u\n", binary_data[0], binary_data[1]);
//         free(binary_data);
//     } else {
//         printf("Failed to read integers from file\n");
//     }
//     return 0;
// }