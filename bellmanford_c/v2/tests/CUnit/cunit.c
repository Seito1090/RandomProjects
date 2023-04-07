/*
Here are contained the tests to make sure the functions impelemented in the graph.c file are working properly.
*/
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/graph.h"

// Test Suite setup and cleanup functions:
//TODO: Add tests for cases where the graph is not connected

void test_get_file_info(void){
    //Default graph 
    FILE * default_test = fopen("tests/CUnit/default.bin", "rb");
    if (default_test == NULL){
        printf("Error opening file");
        exit(1);
    }
    graph_t * default_graph = get_file_info(default_test);
    CU_ASSERT_EQUAL(default_graph->file_infos->nb_edges, 10);
    CU_ASSERT_EQUAL(default_graph->file_infos->nb_nodes, 5);
    free_graph_struct(default_graph);
    fclose(default_test);

    //Empty graph
    FILE * empty_test = fopen("tests/CUnit/empty.bin", "rb");
    if (empty_test == NULL){
        printf("Error opening file");
        exit(1);
    }
    graph_t * empty_graph = get_file_info(empty_test);
    CU_ASSERT_EQUAL(empty_graph, NULL);
    free_graph_struct(empty_graph);
    fclose(empty_test);
    
    //Corrupted graph
    FILE * corrupted_test = fopen("tests/CUnit/corrupted.bin", "rb");
    if (corrupted_test == NULL){
        printf("Error opening file");
        exit(1);
    }
    graph_t * corrupted_graph = get_file_info(corrupted_test);
    CU_ASSERT_EQUAL(corrupted_graph, NULL);
    free_graph_struct(corrupted_graph);
    fclose(corrupted_test);
    
    //Graph with negative cost
    FILE * neg_test = fopen("tests/CUnit/graph_neg.bin", "rb");
    if (neg_test == NULL){
        printf("Error opening file");
        exit(1);
    }
    graph_t * neg_graph = get_file_info(neg_test);
    CU_ASSERT_EQUAL(neg_graph->file_infos->nb_edges, 4);
    CU_ASSERT_EQUAL(neg_graph->file_infos->nb_nodes, 3);
    free_graph_struct(neg_graph);
    fclose(neg_test);
    
    //Graph with only zeros
    FILE * zero_test = fopen("tests/CUnit/graph_0.bin", "rb");
    if (zero_test == NULL){
        printf("Error opening file");
        exit(1);
    }
    graph_t * zero_graph = get_file_info(zero_test);
    CU_ASSERT_EQUAL(zero_graph, NULL);
    free_graph_struct(zero_graph);
    fclose(zero_test);
    
}

void test_bellman_ford(void){
    //Default graph 
    FILE * default_test = fopen("tests/CUnit/default.bin", "rb");
    if (default_test == NULL){exit(1);}
    graph_t * default_graph = get_file_info(default_test);
    if (default_graph == NULL) {exit(1);}
    ford_t * result = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 0, false);
    if (result == NULL){exit(1);}
    //Asserts Equals here for distances and paths 
    free_ford_struct(result);
    ford_t * result = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 0, false);
    if (result == NULL){exit(1);}
    //Asserts Equals here for distances and paths
    free_ford_struct(result);
    free_graph_struct(default_graph);
    fclose(default_test);

    //Graph with negative cost
    FILE * neg_test = fopen("tests/CUnit/graph_neg.bin", "rb");
    if (neg_test == NULL){exit(1);}
    graph_t * neg_graph = get_file_info(neg_test);
    if (neg_graph == NULL) {exit(1);}
    ford_t * result = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 1, false);
    if (result == NULL){exit(1);}
    //Asserts Equals here for distances and paths 
    free_ford_struct(result);
    ford_t * result = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 1, false);
    if (result == NULL){exit(1);}
    //Asserts Equals here for distances and paths
    free_ford_struct(result);
    free_graph_struct(neg_graph);
    fclose(neg_test);
}

void test_get_max(void){

}

void test_get_path(void){

}

int main(){
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Suite_1", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if ((NULL == CU_add_test(pSuite, "test of get_file_info()", test_get_file_info)) ||
        (NULL == CU_add_test(pSuite, "test of bellman_ford()", test_bellman_ford)) //||
        // (NULL == CU_add_test(pSuite, "test of get_max()", test_get_max)) ||
        // (NULL == CU_add_test(pSuite, "test of get_path()", test_get_path))
        ){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
}