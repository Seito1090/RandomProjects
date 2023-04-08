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
//TODO: Add tests for cases where the graph is not connected

void test_default(void){
    printf("Default test start\n");
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
    printf("Default test passed\n");
}

void test_negative_cost(void){
    printf("Negative cost test start\n");
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

    printf("Negative graph tests passed\n");
}

void test_empty(void){
    FILE * empty_test = fopen("tests/graph_bin/empty.bin", "rb");
    if (empty_test == NULL){printf("empty_test get file info"); exit(1);}

    graph_t * empty_graph = get_file_info(empty_test);

    CU_ASSERT_EQUAL(empty_graph, NULL);
    
}

void test_corrupted(void){
    FILE * corrupted_test = fopen("tests/graph_bin/corrupted.bin", "rb");
    if (corrupted_test == NULL){printf("corrupted_test get file info"); exit(1);}
    
    graph_t * corrupted_graph = get_file_info(corrupted_test);

    CU_ASSERT_EQUAL(corrupted_graph, NULL);
    

}

void test_only_zeros(void){
    printf("Test only zeros start\n");
    FILE * zero_test = fopen("tests/graph_bin/zeros.bin", "rb");
    if (zero_test == NULL){
        printf("Error opening file zero_test");
        exit(1);
    }
    graph_t * zero_graph = get_file_info(zero_test);

    CU_ASSERT_EQUAL(zero_graph, NULL);
    
    printf("Test only zeros passed\n");
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
    if ((NULL == CU_add_test(pSuite, "test of default graph", test_default)) ||
        (NULL == CU_add_test(pSuite, "test of graph with negative cost", test_negative_cost)) ||
        (NULL == CU_add_test(pSuite, "test of empty graph", test_empty)) ||
        (NULL == CU_add_test(pSuite, "test of corrupted graph", test_corrupted)) ||
        (NULL == CU_add_test(pSuite, "test of full zeros graph", test_only_zeros)) 
        ){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
}