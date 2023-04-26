#include "../include/graph.h"
#define NTHREADS 4

int inputbuffer[NTHREADS-2];
int outputbuffer[NTHREADS-2];
int inputbuffer_index = 0;
int outputbuffer_index = 0;

int inputbuffer_empty(){
    return inputbuffer_index == 0;
}
int inputbuffer_full(){
    return inputbuffer_index == NTHREADS-2;
}
void pushToInputBuffer(int value){
    inputbuffer[inputbuffer_index] = value;
    inputbuffer_index++;
}
int popFromInputBuffer(){
    int value = inputbuffer[inputbuffer_index];
    inputbuffer_index--;
    return value;
}
int outputbuffer_empty(){
    return outputbuffer_index == 0;
}
int outputbuffer_full(){
    return outputbuffer_index == NTHREADS-2;
}
void pushToOutputBuffer(int value){
    outputbuffer[outputbuffer_index] = value;
    outputbuffer_index++;
}
int popFromOutputBuffer(){
    int value = outputbuffer[outputbuffer_index];
    outputbuffer_index--;
    return value;
}
void* graphproducer(void *arg){
    /*This function reads the file, produces the graph structure and fills the input buffer for other threads to read from*/
}
void* graphcomputer(void *arg){
    /*This function reads from the input buffer, computes the shortest path and fills the output buffer for the last thread to read from*/
}
void* graphwriter(void *arg){
    /*This function reads from the output buffer and writes the result to the file*/
}
int main(int args, char *argv[]) {
    /*First we need 2 buffers, one for the readind and one for the writing threads, ATTENTION PLEASE IT WORKS ONLY FOR NTHREADS > 2*/
    pthread_t threads[NTHREADS];
    int arg[NTHREADS];
    int err;

    return 0;
}