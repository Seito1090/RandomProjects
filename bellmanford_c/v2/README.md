# Skeleton

Skeleton of the code in the C language.
graph->graph_data = (branches_t *)malloc(nb_edges * sizeof(branches_t));
    if (graph->graph_data == NULL) {fclose(file);free(graph);
        perror("Could not allocate memory for nodes_edges, free memory and try again");return NULL;}
    graph->graph_data->node_from = (uint32_t *)malloc(nb_edges* sizeof(uint32_t));
    if (graph->graph_data->node_from == NULL) {fclose(file);return NULL;}
    graph->graph_data->node_to = (uint32_t *)malloc(nb_edges* sizeof(uint32_t));
    if (graph->graph_data->node_to == NULL) {fclose(file);return NULL;}
    graph->graph_data->cost = (int32_t *)malloc(nb_edges* sizeof(int32_t));
    if (graph->graph_data->cost == NULL) {fclose(file);return NULL;}
    for (int i = 0; i < nb_edges; i++){
        unsigned char buffer1[4]; // Buffer + checks for branches
        if (fread(buffer1, 1, 4, file) != 4) {fclose(file);free(graph->graph_data);return NULL;}
        uint32_t temp = ((uint32_t)buffer1[0] << 24) | ((uint32_t)buffer1[1] << 16) | ((uint32_t)buffer1[2] << 8) | buffer1[3];
        if (i%3 == 2) { //checking if we are on the cost part in this format : [from to cost]
            if (temp & 0x80000000) {
                graph->graph_data->cost[i] = (int32_t)(~temp + 1);
            } else {
                graph->graph_data->cost[i] = (int32_t)temp;
            }
        } else if (i % 3 == 1){
            graph->graph_data->node_to[i] = temp;
        } else if (i % 3 == 0){
            graph->graph_data->node_from[i] = temp;
        } else {
            perror("Invalid file format"); return NULL;
        }
        
    }
    fclose(file);
    graph->file_infos->nb_nodes = nb_nodes;
    graph->file_infos->nb_edges = nb_edges;
    for (int i = 0; i < nb_edges; i++){
        printf("from node : %d, to node : %d, cost : %d\n", graph->graph_data->node_from[i], graph->graph_data->node_to[i], graph->graph_data->cost[i]);
    }
    return graph;