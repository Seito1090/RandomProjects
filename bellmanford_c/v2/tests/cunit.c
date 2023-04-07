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

void test_get_file_info(void){
    //Default graph 
    FILE * default_test = fopen("tests/graph_bin/default.bin", "rb");
    if (default_test == NULL){printf("default_test get file info"); exit(1);}
    graph_t * default_graph = get_file_info(default_test);

    CU_ASSERT_EQUAL(default_graph->file_infos->nb_edges, 10);
    CU_ASSERT_EQUAL(default_graph->file_infos->nb_nodes, 5);

    free_graph_struct(default_graph);
    fclose(default_test);
    printf("finished test_get_file_info");

    //Empty graph
    FILE * empty_test = fopen("tests/graph_bin/empty.bin", "rb");
    if (empty_test == NULL){printf("empty_test get file info"); exit(1);}
    graph_t * empty_graph = get_file_info(empty_test);

    CU_ASSERT_EQUAL(empty_graph, NULL);
    
    free_graph_struct(empty_graph);
    fclose(empty_test);
    
    //Corrupted graph
    FILE * corrupted_test = fopen("tests/graph_bin/corrupted.bin", "rb");
    if (corrupted_test == NULL){printf("corrupted_test get file info"); exit(1);}
    graph_t * corrupted_graph = get_file_info(corrupted_test);

    CU_ASSERT_EQUAL(corrupted_graph, NULL);
    
    free_graph_struct(corrupted_graph);
    fclose(corrupted_test);
    
    //Graph with negative cost
    FILE * neg_test = fopen("tests/graph_bin/graph_neg.bin", "rb");
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
    FILE * zero_test = fopen("tests/graph_bin/graph_0.bin", "rb");
    if (zero_test == NULL){
        printf("Error opening file zero_test");
        exit(1);
    }
    graph_t * zero_graph = get_file_info(zero_test);
    CU_ASSERT_EQUAL(zero_graph, NULL);
    free_graph_struct(zero_graph);
    fclose(zero_test);
}

void test_bellman_ford(void){
    //Default graph 
    FILE * default_test = fopen("tests/graph_bin/default.bin", "rb");
    if (default_test == NULL){printf("def test bf");exit(1);}

    graph_t * default_graph = get_file_info(default_test);
    if (default_graph == NULL) {printf("def g bf");exit(1);}

    ford_t * result_def_1 = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 0, false);
    if (result_def_1 == NULL){printf("res def 1");exit(1);}
    
    //Asserts Equals here for node 0
    CU_ASSERT_EQUAL(result_def_1->dist[0], 0);
    CU_ASSERT_EQUAL(result_def_1->dist[1], 4);
    CU_ASSERT_EQUAL(result_def_1->dist[2], 4);
    CU_ASSERT_EQUAL(result_def_1->dist[3], 14);
    CU_ASSERT_EQUAL(result_def_1->dist[4], 7);

    free_ford_struct(result_def_1);

    ford_t * result_def_2 = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 4, false);
    if (result_def_2 == NULL){printf("res def 2");exit(1);}
    
    //Asserts Equals for node 4
    CU_ASSERT_EQUAL(result_def_2->dist[0], 9);
    CU_ASSERT_EQUAL(result_def_2->dist[1], 13);
    CU_ASSERT_EQUAL(result_def_2->dist[2], 13);
    CU_ASSERT_EQUAL(result_def_2->dist[3], 23);
    CU_ASSERT_EQUAL(result_def_2->dist[4], 0);

    free_ford_struct(result_def_2);
    free_graph_struct(default_graph);
    fclose(default_test);


    //Graph with negative cost
    FILE * neg_test = fopen("tests/graph_bin/graph_neg.bin", "rb");
    if (neg_test == NULL){printf("neg test bf");exit(1);}

    graph_t * neg_graph = get_file_info(neg_test);
    if (neg_graph == NULL) {printf("neg graph bf");exit(1);}

    ford_t * result_def_3 = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 0, false);
    if (result_def_3 == NULL){printf("res def 3");exit(1);}

    //Asserts Equals here for node 0
    CU_ASSERT_EQUAL(result_def_3->dist[0], 0);
    CU_ASSERT_EQUAL(result_def_3->dist[1], 8);
    CU_ASSERT_EQUAL(result_def_3->dist[2], -1);

    free_ford_struct(result_def_3);

    ford_t * result_def_4 = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 2, false);
    if (result_def_4 == NULL){printf("res def 4");exit(1);}

    //Asserts Equals here for node 2
    CU_ASSERT_EQUAL(result_def_4->dist[0], 13);
    CU_ASSERT_EQUAL(result_def_4->dist[1], 9);
    CU_ASSERT_EQUAL(result_def_4->dist[2], 0);
   
    free_ford_struct(result_def_4);
    free_graph_struct(neg_graph);
    fclose(neg_test);
}

void test_get_max(void){
    //Default graph Setup
    FILE * default_test = fopen("tests/graph_bin/default.bin", "rb");
    if (default_test == NULL){printf("def test max");exit(1);}

    graph_t * default_graph = get_file_info(default_test);
    if (default_graph == NULL) {printf("def g max");exit(1);}

    ford_t * result_def_5 = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 1, false);
    if (result_def_5 == NULL){printf("res def 5");exit(1);}

    mcost_t * max_def_1= get_max(default_graph->file_infos->nb_nodes,result_def_5->dist, default_graph->file_infos->nb_nodes);
    if (max_def_1== NULL){printf("max def 1");exit(1);}

    //Default graph Asserts
    CU_ASSERT_EQUAL(max_def_1->cost, 10);
    CU_ASSERT_EQUAL(max_def_1->node, 3); 

    //Default graph Free
    free_max_struct(max_def_1);
    free_ford_struct(result_def_5);
    free_graph_struct(default_graph);
    fclose(default_test);


    //Graph with negative cost
    FILE * neg_test = fopen("tests/graph_bin/graph_neg.bin", "rb");
    if (neg_test == NULL){printf("neg t max");exit(1);}
    
    graph_t * neg_graph = get_file_info(neg_test);
    if (neg_graph == NULL) {printf("neg g max");exit(1);}
    
    ford_t * result_def_6 = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 0, false);
    if (result_def_6 == NULL){printf("res def 6");exit(1);}
    
    mcost_t * max_def_2 = get_max(neg_graph->file_infos->nb_nodes,result_def_6->dist, neg_graph->file_infos->nb_nodes);
    if (max_def_2 == NULL){printf("max def 2");exit(1);}

    
    CU_ASSERT_EQUAL(max_def_2->cost, 8);
    CU_ASSERT_EQUAL(max_def_2->node, 1); 

    free_max_struct(max_def_2);
    free_ford_struct(result_def_6);
    
    ford_t * result_def_7 = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 2, false);
    if (result_def_7 == NULL){printf("res def 7");exit(1);}
    
    mcost_t * max_def_3 = get_max(neg_graph->file_infos->nb_nodes,result_def_7->dist, neg_graph->file_infos->nb_nodes);
    if (max_def_3 == NULL){printf("max def 3");exit(1);}


    CU_ASSERT_EQUAL(max_def_3->cost, 13);
    CU_ASSERT_EQUAL(max_def_3->node, 0); 

    free_max_struct(max_def_3);
    free_ford_struct(result_def_7);
    free_graph_struct(neg_graph);
    fclose(neg_test);
}

void test_get_path(void){
    //Default graph Setup
    FILE * default_test = fopen("tests/graph_bin/default.bin", "rb");
    if (default_test == NULL){exit(1);}
    
    graph_t * default_graph = get_file_info(default_test);
    if (default_graph == NULL) {exit(1);}
    
    ford_t * result_def_8 = bellman_ford(default_graph->file_infos->nb_nodes, default_graph->file_infos->nb_edges, default_graph->graph_data, 1, false);
    if (result_def_8 == NULL){printf("res def 8");exit(1);}
    
    mcost_t * max_def_4 = get_max(default_graph->file_infos->nb_nodes,result_def_8->dist, default_graph->file_infos->nb_nodes);
    if (max_def_4 == NULL){printf("max def 4");exit(1);}
    
    int32_t size_def1 = default_graph->file_infos->nb_nodes;
    int32_t * path_def_1 = get_path(max_def_4->node, 1, result_def_8->path, &size_def1);
    if (path_def_1 == NULL){printf("path_def_1\n");exit(1);}

    //Default graph Asserts
    CU_ASSERT_EQUAL(size_def1, 2);
    CU_ASSERT_EQUAL(path_def_1[0], 1);
    CU_ASSERT_EQUAL(path_def_1[1], 3); 

    //Default graph Free
    free_path(path_def_1);
    free_max_struct(max_def_4);
    free_ford_struct(result_def_8);
    free_graph_struct(default_graph);
    fclose(default_test);


    //Graph with negative cost
    FILE * neg_test = fopen("tests/graph_bin/graph_neg.bin", "rb");
    if (neg_test == NULL){exit(1);}
    
    graph_t * neg_graph = get_file_info(neg_test);
    if (neg_graph == NULL) {exit(1);}
    
    ford_t * result_def_9 = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 0, false);
    if (result_def_9 == NULL){printf("res def 9");exit(1);}
    
    mcost_t * max_def_5 = get_max(neg_graph->file_infos->nb_nodes,result_def_9->dist, neg_graph->file_infos->nb_nodes);
    if (max_def_5 == NULL){printf("max def 5");exit(1);}
    
    int32_t size_def2 = default_graph->file_infos->nb_nodes;
    int32_t * path_def_2 = get_path(max_def_5->node, 0, result_def_9->path, &size_def2);
    if (path_def_2 == NULL){printf("path_def_2\n");exit(1);}


    CU_ASSERT_EQUAL(size_def2, 3);
    CU_ASSERT_EQUAL(path_def_2[0], 0);
    CU_ASSERT_EQUAL(path_def_2[1], 2); 
    CU_ASSERT_EQUAL(path_def_2[2], 1);

    free_path(path_def_2);
    free_max_struct(max_def_5);
    free_ford_struct(result_def_9);
    
    ford_t * result_def_10 = bellman_ford(neg_graph->file_infos->nb_nodes, neg_graph->file_infos->nb_edges, neg_graph->graph_data, 2, false);
    if (result_def_10 == NULL){printf("res def 10");exit(1);}
    
    mcost_t * max_def_6 = get_max(neg_graph->file_infos->nb_nodes,result_def_10->dist, neg_graph->file_infos->nb_nodes);
    if (max_def_6 == NULL){printf("max_def_6");exit(1);}
    
    int32_t size_def3 = default_graph->file_infos->nb_nodes;
    int32_t * path_def_3 = get_path(max_def_6->node, 2, result_def_10->path, &size_def3);
    if (path_def_3 == NULL){printf("path_def_3\n");exit(1);}

    
    CU_ASSERT_EQUAL(size_def3, 3);
    CU_ASSERT_EQUAL(path_def_3[0], 2);
    CU_ASSERT_EQUAL(path_def_3[1], 1); 
    CU_ASSERT_EQUAL(path_def_3[2], 0);

    free_path(path_def_3);
    free_max_struct(max_def_6);
    free_ford_struct(result_def_10);
    free_graph_struct(neg_graph);
    fclose(neg_test);
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
        (NULL == CU_add_test(pSuite, "test of bellman_ford()", test_bellman_ford)) ||
        (NULL == CU_add_test(pSuite, "test of get_max()", test_get_max)) ||
        (NULL == CU_add_test(pSuite, "test of get_path()", test_get_path))
        ){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_run_tests();
    printf("hey");
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
}