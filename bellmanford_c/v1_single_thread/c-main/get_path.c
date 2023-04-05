#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "get_max.c"

int* get_path(int dest, int source, float* path, int nb_nodes){
    int max_r_size = nb_nodes; // maximum possible size of r
    int* r = (int*)malloc((max_r_size + 1) * sizeof(int)); // allocate memory for r
    r[0] = 1; // set initial length of r
    r[1] = dest; // set initial value of r
    int i = dest;
    while (i != source) {
        i = path[i];
        if (r[0] == max_r_size) { // if r is full, double its size
            max_r_size *= 2;
            r = (int*)realloc(r, (max_r_size + 1) * sizeof(int));
        }
        r[0]++;
        r[r[0]] = i;
    }
    // reverse the order of elements in r and add the length of the path
    for (int j = 1; j <= r[0] / 2; j++) {
        int tmp = r[j];
        r[j] = r[r[0] - j + 1];
        r[r[0] - j + 1] = tmp;
    }
    r[r[0] + 1] = r[0];
    return r;
}

int main(int argc,char** argv){
    clock_t start, end;
    double execution_time;
    start = clock();

    char* file_name = "graph.bin";
    unsigned int** links = read_graph(file_name);
    unsigned int nb_nodes = links[0][0];
    unsigned int nb_edges = links[0][1];

    char buf[1024];
    char output[1024];
    char* out_ptr = output;
    char* buf_ptr = buf;

    for (int source = 0; source < nb_nodes; source++){
        int len = sprintf(buf_ptr, "Source : %d\nDistances : [ ", source);
        buf_ptr += len;
        float** result = belman_ford(nb_nodes, nb_edges, links, nb_edges, source, false);
        for (int i = 0; i < nb_nodes; i++) {
            len = sprintf(buf_ptr, "%d ", (int)result[0][i]);
            buf_ptr += len;
        }
        len = sprintf(buf_ptr, "]\n");
        buf_ptr += len;
        float* max = get_max(nb_nodes, result[0], source);
        int cost = (int)max[0];
        int node = (int)max[1];
        int* path = get_path(node, source, result[1], nb_nodes);
        int number_of_nodes = path[0];
        len = sprintf(out_ptr, "  Destination: %d\n  Cost: %d\n  Number of nodes in the path: %d\n  Path to take : [",
                node, cost, number_of_nodes);
        out_ptr += len;
        for (int i = 1; i < number_of_nodes+1; i++){
            len = sprintf(out_ptr, " %d", path[i]);
            out_ptr += len;
        }
        len = sprintf(out_ptr, "]\n--------------------------------------------------\n");
        out_ptr += len;
        free(result);
    }
    // Print the output buffer all at once
    printf("%s", buf);
    printf("%s", output);

    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Execution time : %f\n", execution_time);

    return 0;

}