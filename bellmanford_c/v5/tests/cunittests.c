/*
Here are contained the tests to make sure the functions impelemented in the graph.c file are working properly.
*/
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"

// Test Suite setup and cleanup functions:

/*
Function test_default
--------------------------------
Tests the code with the default graph, 5 nodes, 10 links, seed 42
*/
void test_default(void){
    /*Tests the code with the default graph, 5 nodes, 10 links, seed 42*/
    FILE * default_file = fopen("tests/graph_bin/default.bin", "rb");
    if (default_file == NULL){printf("default_test get file info"); exit(1);}

    graph_t * default_graph = get_file_info(default_file);
    
    CU_ASSERT_EQUAL(default_graph->file_infos->nb_edges, 10);
    CU_ASSERT_EQUAL(default_graph->file_infos->nb_nodes, 5);

    ford_t * result_node_0 = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 0, false);
    if (result_node_0 == NULL){printf("res def 1");exit(1);}
    
    //Asserts Equals here for node 0
    CU_ASSERT_EQUAL(result_node_0->dist[0], 0);
    CU_ASSERT_EQUAL(result_node_0->dist[1], 4);
    CU_ASSERT_EQUAL(result_node_0->dist[2], 4);
    CU_ASSERT_EQUAL(result_node_0->dist[3], 14);
    CU_ASSERT_EQUAL(result_node_0->dist[4], 7);

    free_ford_struct(result_node_0);

    ford_t * result_node_4 = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 4, false);
    if (result_node_4 == NULL){printf("res def 2");exit(1);}
    
    //Asserts Equals for node 4
    CU_ASSERT_EQUAL(result_node_4->dist[0], 9);
    CU_ASSERT_EQUAL(result_node_4->dist[1], 13);
    CU_ASSERT_EQUAL(result_node_4->dist[2], 13);
    CU_ASSERT_EQUAL(result_node_4->dist[3], 23);
    CU_ASSERT_EQUAL(result_node_4->dist[4], 0);

    free_ford_struct(result_node_4);

    ford_t * result_node_1 = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 1, false);
    if (result_node_1 == NULL){printf("res def 5");exit(1);}

    mcost_t * max_node_1= get_max(default_graph->file_infos->nb_nodes,result_node_1->dist, 1);
    if (max_node_1== NULL){printf("max def 1");exit(1);}

    //Default graph Asserts
    CU_ASSERT_EQUAL(max_node_1->cost, 10);
    CU_ASSERT_EQUAL(max_node_1->node, 3); 

    //Default graph Free
    free_max_struct(max_node_1);
    free_ford_struct(result_node_1);

    ford_t * result_node_2 = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 2, false);
    if (result_node_2 == NULL){printf("res def 8");exit(1);}
    
    mcost_t * max_node_2 = get_max(default_graph->file_infos->nb_nodes,result_node_2->dist, 1);
    if (max_node_2 == NULL){printf("max def 4");exit(1);}
    
    int32_t size_node_2 = default_graph->file_infos->nb_nodes;
    int32_t * path_node_2 = get_path(max_node_2->node, 2, result_node_2->path, &size_node_2);
    
    //Default graph Asserts
    CU_ASSERT_EQUAL(size_node_2, 4);
    CU_ASSERT_EQUAL(path_node_2[0], 2);
    CU_ASSERT_EQUAL(path_node_2[1], 0); 
    CU_ASSERT_EQUAL(path_node_2[2], 1);
    CU_ASSERT_EQUAL(path_node_2[3], 3);

    //Default graph Free
    free_path(path_node_2);
    free_max_struct(max_node_2);
    free_ford_struct(result_node_2);
    free_graph_struct(default_graph);
}

/*
Function test_negative_cost
--------------------------------
Tests the code with the negative cost graph, 3 nodes, 4 links, seed 42
*/
void test_negative_cost(void){
    FILE * neg_test = fopen("tests/graph_bin/neg_cost.bin", "rb");
    if (neg_test == NULL){
        printf("Error opening file");
        exit(1);
    }
    graph_t * neg_graph = get_file_info(neg_test);

    CU_ASSERT_EQUAL(neg_graph->file_infos->nb_edges, 4);
    CU_ASSERT_EQUAL(neg_graph->file_infos->nb_nodes, 3);

    ford_t * result_neg_node_2 = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 2, false);
    if (result_neg_node_2 == NULL){printf("res def 4");exit(1);}

    //Asserts Equals here for node 2
    CU_ASSERT_EQUAL(result_neg_node_2->dist[0], 13);
    CU_ASSERT_EQUAL(result_neg_node_2->dist[1], 9);
    CU_ASSERT_EQUAL(result_neg_node_2->dist[2], 0);
    
    ford_t * result_neg_node_0 = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 0, false);
    if (result_neg_node_0 == NULL){printf("res def 3");exit(1);}

    //Asserts Equals here for node 0
    CU_ASSERT_EQUAL(result_neg_node_0->dist[0], 0);
    CU_ASSERT_EQUAL(result_neg_node_0->dist[1], 8);
    CU_ASSERT_EQUAL(result_neg_node_0->dist[2], -1);

    mcost_t * max_neg_node_0 = get_max(neg_graph->file_infos->nb_nodes,result_neg_node_0->dist, 0);
    if (max_neg_node_0 == NULL){printf("max def 2");exit(1);}

    CU_ASSERT_EQUAL(max_neg_node_0->cost, 8);
    CU_ASSERT_EQUAL(max_neg_node_0->node, 1); 

    mcost_t * max_neg_node_2 = get_max(neg_graph->file_infos->nb_nodes,result_neg_node_2->dist, 2);
    if (max_neg_node_2 == NULL){printf("max def 3");exit(1);}

    CU_ASSERT_EQUAL(max_neg_node_2->cost, 13);
    CU_ASSERT_EQUAL(max_neg_node_2->node, 0); 
    
    int32_t size_neg_node_0 = neg_graph->file_infos->nb_nodes;
    int32_t * path_neg_node_0 = get_path(max_neg_node_0->node, 0, result_neg_node_0->path, &size_neg_node_0);
    if (path_neg_node_0 == NULL){printf("path_neg_node_0\n");exit(1);}

    CU_ASSERT_EQUAL(size_neg_node_0, 3);
    CU_ASSERT_EQUAL(path_neg_node_0[0], 0);
    CU_ASSERT_EQUAL(path_neg_node_0[1], 2); 
    CU_ASSERT_EQUAL(path_neg_node_0[2], 1);

    int32_t size_neg_node_2 = neg_graph->file_infos->nb_nodes;
    int32_t * path_neg_node_2 = get_path(max_neg_node_2->node, 2, result_neg_node_2->path, &size_neg_node_2);
    if (path_neg_node_2 == NULL){printf("path_def_3\n");exit(1);}

    CU_ASSERT_EQUAL(size_neg_node_2, 3);
    CU_ASSERT_EQUAL(path_neg_node_2[0], 2);
    CU_ASSERT_EQUAL(path_neg_node_2[1], 1); 
    CU_ASSERT_EQUAL(path_neg_node_2[2], 0);

    //Free Negative graph
    free_path(path_neg_node_2);
    free_path(path_neg_node_0);
    free_max_struct(max_neg_node_0);
    free_max_struct(max_neg_node_2);
    free_ford_struct(result_neg_node_2);
    free_ford_struct(result_neg_node_0);
    free_graph_struct(neg_graph);
}

/*
Function test_empty
--------------------------------
Tests the code with an edge case, an empty graph
*/
void test_empty(void){
    FILE * empty_test = fopen("tests/graph_bin/empty.bin", "rb");
    if (empty_test == NULL){printf("empty_test get file info"); exit(1);}

    graph_t * empty_graph = get_file_info(empty_test);

    CU_ASSERT_EQUAL(empty_graph, NULL);
    
}

/*
Function test_corrupted
--------------------------------
Tests the code with an edge case, a corrupted graph
*/
void test_corrupted(void){
    FILE * corrupted_test = fopen("tests/graph_bin/corrupted.bin", "rb");
    if (corrupted_test == NULL){printf("corrupted_test get file info"); exit(1);}
    
    graph_t * corrupted_graph = get_file_info(corrupted_test);

    CU_ASSERT_EQUAL(corrupted_graph, NULL);
}

/*
Function test_only_zeros
--------------------------------
Tests the code with an edge case, a graph with only zeros
*/
void test_only_zeros(void){
    FILE * zero_test = fopen("tests/graph_bin/zeros.bin", "rb");
    if (zero_test == NULL){
        printf("Error opening file zero_test");
        exit(1);
    }
    graph_t * zero_graph = get_file_info(zero_test);

    CU_ASSERT_EQUAL(zero_graph, NULL);
}

/*
Function check_binary_files_equal
--------------------------------
This function checks if two binary files are equal

Input:
---------
const char* filename1: The name of the first file
const char* filename2: The name of the second file

Output:
---------
int: 0 if the files are equal, 1 otherwise
*/
int check_binary_files_equal(const char* filename1, const char* filename2) {
    FILE* file1 = fopen(filename1, "rb");
    if (!file1) {
        printf("Error: Failed to open file %s\n", filename1);
        return 1;
    }

    FILE* file2 = fopen(filename2, "rb");
    if (!file2) {
        printf("Error: Failed to open file %s\n", filename2);
        fclose(file1);
        return 1;
    }

    int is_equal = 1;
    uint8_t byte1, byte2;
    while (1) {
        byte1 = fgetc(file1);
        byte2 = fgetc(file2);
        if (byte1 != byte2) {
            is_equal = 0;
            break;
        }
        if (feof(file1) && feof(file2)) {
            break;
        }
        if (feof(file1) || feof(file2)) {
            is_equal = 0;
            break;
        }
    }

    if (!is_equal) {
        printf("The files are not equal\n");
        fclose(file1);
        fclose(file2);
        return 1;
    }

    fclose(file1);
    fclose(file2);

    printf("The files are equal\n");
    return 0;
}

/*
Function test_check_binary_files_equal
--------------------------------
This function tests cases for the check_binary_files_equal function
*/
void test_check_binary_files_equal(void) {
    CU_ASSERT_EQUAL(check_binary_files_equal("tests/output_tests/default_py.bin", "tests/output_tests/default_c.bin"), 0);
    CU_ASSERT_EQUAL(check_binary_files_equal("tests/output_tests/neg_py.bin", "tests/output_tests/neg_c.bin"), 0);
    CU_ASSERT_EQUAL(check_binary_files_equal("tests/output_tests/wrong_py.bin", "tests/output_tests/default_c.bin"), 1);
    CU_ASSERT_EQUAL(check_binary_files_equal("tests/output_tests/non_existent_file.bin", "tests/output_tests/default_c.bin"), 1);
}

/*
Function test_get_file_info
--------------------------------
This function tests if the get_file_info function works correctly
*/
void test_get_file_info(void){
    CU_ASSERT_EQUAL(get_file_info(NULL), NULL);
    FILE * file = fopen("tests/graph_bin/default.bin", "rb");
    graph_t * graph_check = malloc(sizeof(graph_t));
    graph_check->file_infos = malloc(sizeof(file_data_t));
    graph_check->file_infos->nb_nodes = 5;
    graph_check->file_infos->nb_edges = 10;
    graph_check->graph_data = (branch_t*)malloc(10*sizeof(branch_t));
    graph_check->graph_data->cost = (int32_t*)malloc(10*sizeof(int32_t));
    graph_check->graph_data->node_from = (uint32_t*)malloc(10*sizeof(uint32_t));
    graph_check->graph_data->node_to = (uint32_t*)malloc(10*sizeof(uint32_t));
    graph_check->graph_data->node_from[0] = 0 ;graph_check->graph_data->node_to[0] = 2 ; graph_check->graph_data->cost[0] = 4;
    graph_check->graph_data->node_from[1] = 0 ;graph_check->graph_data->node_to[1] = 4 ; graph_check->graph_data->cost[1] = 7;
    graph_check->graph_data->node_from[2] = 0 ;graph_check->graph_data->node_to[2] = 1 ; graph_check->graph_data->cost[2] = 4;
    graph_check->graph_data->node_from[3] = 1 ;graph_check->graph_data->node_to[3] = 0 ; graph_check->graph_data->cost[3] = 9;
    graph_check->graph_data->node_from[4] = 1 ;graph_check->graph_data->node_to[4] = 4 ; graph_check->graph_data->cost[4] = 7;
    graph_check->graph_data->node_from[5] = 1 ;graph_check->graph_data->node_to[5] = 3 ; graph_check->graph_data->cost[5] = 10;
    graph_check->graph_data->node_from[6] = 1 ;graph_check->graph_data->node_to[6] = 2 ; graph_check->graph_data->cost[6] = 2;
    graph_check->graph_data->node_from[7] = 2 ;graph_check->graph_data->node_to[7] = 0 ; graph_check->graph_data->cost[7] = 3;
    graph_check->graph_data->node_from[8] = 3 ;graph_check->graph_data->node_to[8] = 2 ; graph_check->graph_data->cost[8] = 5;
    graph_check->graph_data->node_from[9] = 4 ;graph_check->graph_data->node_to[9] = 0 ; graph_check->graph_data->cost[9] = 9;
    
    graph_t * graph = get_file_info(file);
    CU_ASSERT_EQUAL(graph->file_infos->nb_nodes, graph_check->file_infos->nb_nodes);
    CU_ASSERT_EQUAL(graph->file_infos->nb_edges, graph_check->file_infos->nb_edges);
    for (int i = 0; i < graph->file_infos->nb_edges; i++){
        CU_ASSERT_EQUAL(graph->graph_data->node_from[i], graph_check->graph_data->node_from[i]);
        CU_ASSERT_EQUAL(graph->graph_data->node_to[i], graph_check->graph_data->node_to[i]);
        CU_ASSERT_EQUAL(graph->graph_data->cost[i], graph_check->graph_data->cost[i]);
    }
    free_graph_struct(graph);
    free_graph_struct(graph_check);
}

/*
Function test_bellman_ford
--------------------------------
This function tests if the bellman_ford function works correctly
*/
void test_bellman_ford(void){
    FILE * file = fopen("tests/graph_bin/default.bin", "rb");
    graph_t * graph = get_file_info(file);

    int32_t* dist_check = (int32_t*)malloc(sizeof(int32_t) * graph->file_infos->nb_nodes);
    int32_t* paths_check = (int32_t*)malloc(sizeof(int32_t) * graph->file_infos->nb_nodes);

    dist_check[0] = 0; dist_check[1] = 4; dist_check[2] = 4; dist_check[3] = 14; dist_check[4] = 7;
    paths_check[0] = -1; paths_check[1] = 0; paths_check[2] = 0; paths_check[3] = 1; paths_check[4] = 0;

    ford_t * ford = bellman_ford(graph->file_infos->nb_nodes, graph->file_infos->nb_edges, graph->graph_data, 0, false);
    for (int i = 0; i < graph->file_infos->nb_nodes; i++){
        CU_ASSERT_EQUAL(ford->dist[i], dist_check[i]);
        CU_ASSERT_EQUAL(ford->path[i], paths_check[i]);
    }
    free_graph_struct(graph);
    free_ford_struct(ford);
    free(dist_check);
    free(paths_check);
}

/*
Function test_get_max
--------------------------------
This function tests if the get_max function works correctly
*/
void test_get_max(void){
    int32_t* dist_check = (int32_t*)malloc(sizeof(int32_t) * 5);
    dist_check[0] = 0; dist_check[1] = 4; dist_check[2] = 4; dist_check[3] = 14; dist_check[4] = 7;
    mcost_t * mcost = get_max(5, dist_check, 0);
    CU_ASSERT_EQUAL(mcost->cost, 14);
    CU_ASSERT_EQUAL(mcost->node, 3);
    free(dist_check);
    free_max_struct(mcost);
}

/*
Function test_get_path
--------------------------------
This function tests if the get_path function works correctly
*/
void test_get_path(void){
    int32_t* paths_check = (int32_t*)malloc(sizeof(int32_t) * 5);
    paths_check[0] = -1; paths_check[1] = 0; paths_check[2] = 0; paths_check[3] = 1; paths_check[4] = 0;
    int32_t size = 5;
    int32_t * path = get_path(3, 0, paths_check, &size);
    CU_ASSERT_EQUAL(path[0], 0);
    CU_ASSERT_EQUAL(path[1], 1);
    CU_ASSERT_EQUAL(path[2], 3);
    free_path(path);
    free(paths_check);
}

int main(int argc, char **argv){
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Suite_1", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (argc > 1){
        printf("Test de correspondance d'un graph personalise\n"); 
        check_binary_files_equal(argv[1], argv[2]); 
    } else {
        if ((NULL == CU_add_test(pSuite, "test of default graph", test_default)) ||
            (NULL == CU_add_test(pSuite, "test of graph with negative cost", test_negative_cost)) ||
            (NULL == CU_add_test(pSuite, "test of empty graph", test_empty)) ||
            (NULL == CU_add_test(pSuite, "test of corrupted graph", test_corrupted)) ||
            (NULL == CU_add_test(pSuite, "test of full zeros graph", test_only_zeros)) || 
            (NULL == CU_add_test(pSuite, "test of binary output files", test_check_binary_files_equal))||
            (NULL == CU_add_test(pSuite, "test of get_file_info", test_get_file_info)) ||
            (NULL == CU_add_test(pSuite, "test of bellman_ford", test_bellman_ford)) ||
            (NULL == CU_add_test(pSuite, "test of get_max", test_get_max)) ||
            (NULL == CU_add_test(pSuite, "test of get_path", test_get_path))
            ){
            CU_cleanup_registry();
            return CU_get_error();
        }
        CU_basic_run_tests();
        CU_basic_show_failures(CU_get_failure_list());
    }
    CU_cleanup_registry();
}