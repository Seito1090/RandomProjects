#include "../include/graph.h"
#include "../include/mtgraph.h"
#include <semaphore.h>

uint8_t BUFFERSIZE;
uint8_t NTHREADS;

//test
sem_t read_sem, compute_sem, read_done_sem, write_sem, compute_done_sem;

//Declaration of shared buffers and everything related to them
uint32_t *readbuffer;
int readhead = 0;
int readtail = 0;

thread_data_t *writebuffer;
int writehead = 0;
int writetail = 0;

pthread_mutex_t read_done_mutex;
pthread_mutex_t compute_done_mutex;
pthread_cond_t broad;

//Declaration of global varaibles that store the graph and the state of the actions (done or not)
//When created and filled, graph will be free to access for all threads (no need to lock it since it is read only)
graph_t *graph; //graph is global so that each thread can access it easily
bool verbose = false;
bool read_done = false;
bool compute_done = false;
bool show = false;
bool error;
int writecount = 0;
int computecount = 0;

/*
Function readthread 
---------------------------

Function executed by the first threadn it reads the graph stored in the file and fills the graph_t structure

Input:
----------
void* arg : the name of the file to be read, it is a char* that was casted  to void* in order to pass it to a thread

Output:
----------
None
*/
void *readthread(void *arg) {
    //Get the opened file from the argument
    FILE *file = (FILE *) arg;
    
    //Initialize graph and checks if it went well, stop if it went wrong
    graph = (graph_t *)get_file_info(file); 
    if (graph == NULL) {
        printf("Error reading graph from file\n");
        error = true;
        pthread_exit(NULL);
    }

    if (verbose) {
        printf("Nb nodes: %d\nNb edges:%d\n", graph->file_infos->nb_nodes, graph->file_infos->nb_edges);
    }

    printf("readthread done\n");

    for (uint32_t source = 0; source < graph->file_infos->nb_nodes; source++) {
        sem_wait(&read_sem);

        readbuffer[readhead] = source;
        // printf("put source %d in readbuffer\n", source);
        readhead = (readhead + 1) % BUFFERSIZE;

        sem_post(&compute_sem);
        sem_post(&read_done_sem); // signal that write to buffer is done
    }
    pthread_mutex_lock(&read_done_mutex);
    read_done = true;
    pthread_mutex_unlock(&read_done_mutex);
    pthread_exit(NULL);
}


/*
Function computers
---------------------------

Function executed by the computer threads, it computes the best path to the given node

Input:
----------
None, it takes its source node from the readbuffer

Output:
----------
None, it writes the result in the writebuffer
*/
void *computers(void *arg) {
    uint32_t source;
    thread_data_t *data = (thread_data_t *)malloc(sizeof(thread_data_t));
    if (data == NULL) {
        printf("Error: Failed to allocate thread data.\n");
        error = true;
        pthread_exit(NULL);
    }

    while (true) {

        pthread_mutex_lock(&compute_done_mutex);
        pthread_mutex_lock(&read_done_mutex);
        if (read_done){
            if (writecount == graph->file_infos->nb_nodes) {
                printf("computecount = %d\n", computecount);
                pthread_mutex_unlock(&read_done_mutex);
                pthread_mutex_unlock(&compute_done_mutex);
                free(data);
                printf("terminated a compute thread\n");
                pthread_exit(NULL);
            }
        }
        pthread_mutex_unlock(&read_done_mutex);
        pthread_mutex_unlock(&compute_done_mutex);
        sem_wait(&compute_sem);

        sem_wait(&read_done_sem); // wait for write to buffer to be done

        source = readbuffer[readtail];
        readtail = (readtail + 1) % BUFFERSIZE;

        sem_post(&read_sem); // signal that buffer has a free slot

        if (graph != NULL) {
            // printf("took source %d\n", source);
            ford_t *ford = (ford_t *)bellman_ford(graph->file_infos->nb_nodes, graph->file_infos->nb_edges, graph->graph_data, source, verbose);
            mcost_t * max = (mcost_t *)get_max(graph->file_infos->nb_nodes, ford->dist, source);
            int32_t size = graph->file_infos->nb_nodes;
            int32_t * path = (int32_t *)get_path(max->node, source, ford->path, &size);
            data->source = source; data->max = max; data->size = size; data->path = path; data->ford = ford;
            printf("computed source %d\n", source);

            sem_wait(&write_sem);

            writebuffer[writehead] = *data;
            // printf("source %d written to write buffer \n", source);
            writehead = (writehead + 1) % BUFFERSIZE;

            sem_post(&write_sem);
            sem_post(&compute_done_sem);
        }
        printf("waiting here\n");
        pthread_mutex_lock(&compute_done_mutex);
        computecount++;
        printf("computecount = %d\n", computecount);
        if (writecount == graph->file_infos->nb_nodes){
            pthread_mutex_unlock(&compute_done_mutex);
            printf("terminated a compute thread normally\n");
            break;
        }
        pthread_mutex_unlock(&compute_done_mutex);

    }
    printf("hard exit \n");
    pthread_exit(NULL);
}


/*
Function writethread
---------------------------

Function executed by the write thread, it writes the data structure in the writebuffer to the file

Input:
----------
None, it takes its data from the writebuffer

Output:
----------
None, it writes the data to the file
*/
void *writethread(void *arg){
    thread_data_t *data; 
    // bool first_pass = true; int wrote; 

    while (true){
        sem_wait(&write_sem);

        sem_wait(&compute_done_sem);

        data = &writebuffer[writetail];
        writetail = (writetail + 1) % BUFFERSIZE;

        sem_post(&write_sem);

        printf("took source %d from write buffer\n", data->source);
        pthread_mutex_lock(&compute_done_mutex);
        writecount++;
        if (writecount == graph->file_infos->nb_nodes){
            printf("writecount = %d\n", writecount);
            printf("computer_done set\n");
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&compute_done_mutex);
    }
    pthread_exit(NULL);
}
    // //Initialization of the variables used in the function and getting the file from the arguments
    // FILE *file = (FILE*)arg;
    
    // //While the compute threads are not done, it writes the data structure in the writebuffer to the file
    // while (true) {

    //     //Waits for the writebuffer to be accessible and reads the data structure
    //     pthread_mutex_lock(&write_mutex);
    //     if(write_done){
    //         if (writecount < graph->file_infos->nb_nodes){
    //             printf("Error: not all data was written to the file A\n");
    //             // printf("writetail and writehead: %d %d\n", writetail, writehead);
    //             // thread_data_t * data1 = &writebuffer[writetail];
    //             // thread_data_t * data2 = &writebuffer[writehead];
    //             // thread_data_t * data3 = &writebuffer[(writehead + 1) % BUFFERSIZE];
    //             // thread_data_t * data4 = &writebuffer[(writehead + 2) % BUFFERSIZE];
    //             // thread_data_t * data5 = &writebuffer[(writehead + 3) % BUFFERSIZE];
    //             // printf("sources : \n data 1 : %d \n data 2 : %d \n data 3 : %d \n data 4 : %d \n data 5 : %d \n", data1->source, data2->source, data3->source, data4->source, data5->source);
    //             // data = &writebuffer[0];
    //             // if (show){

    //             //     //Shows the graph computed data in the terminal
    //             //     printf("Source node : %d\nDistances : [ ", data->source);
    //             //     for (int i = 0; i < graph->file_infos->nb_nodes; i++) {
    //             //         printf("%d ", data->ford->dist[i]);
    //             //     }
    //             //     printf("]\n    Destination : %u\n    Cost : %ld\n    Number of nodes : %d\n    Path : [", data->max->node, data->max->cost, data->size);
    //             //     for (int i = 0; i < data->size; i++) {
    //             //         printf(" %d", data->path[i]);
    //             //     }
    //             //     printf(" ]\n");
    //             //     writecount++;
    //             // } else {
    //             //     wrote = write_to_file(file, data->source, data->max, data->size, data->path);
    //             //     if (wrote == 1) {
    //             //         printf("Error writing to file\n");
    //             //         error = true;
    //             //         pthread_exit(NULL);
    //             //     }
    //             // }
    //             pthread_mutex_unlock(&write_mutex);
    //             pthread_exit(NULL);
    //         }   
    //     }
    //     while (writehead == writetail) {
    //         pthread_cond_wait(&write_not_empty, &write_mutex);
    //         if (write_done) {
    //             if (writecount < graph->file_infos->nb_nodes){
    //                 printf("Error: not all data was written to the file B\n");
    //                 printf("writetail and writehead: %d %d\n", writetail, writehead);
    //             }
    //             pthread_mutex_unlock(&write_mutex);
    //             printf("Super exit\n");
    //             pthread_exit(NULL);
    //         }
    //     }
    //     data = &writebuffer[writetail];
    //     printf("came here and got sourece %d\n", data->source);
    //     writetail = (writetail + 1) % BUFFERSIZE;
    //     pthread_cond_signal(&write_not_full);
    //     pthread_mutex_unlock(&write_mutex);


    //     //Checks if the data is null, if it is, exits because cannot write anything/ there was a bug
    //     if (data == NULL) {
    //         printf("Error: data is null\n");
    //         error = true;
    //         pthread_exit(NULL);
    //     }

    //     //Checks if it has to show or save the output, does accordingly
    //     if (show){

    //         //Shows the graph computed data in the terminal
    //         printf("Source node : %d\nDistances : [ ", data->source);
    //         for (int i = 0; i < graph->file_infos->nb_nodes; i++) {
    //             printf("%d ", data->ford->dist[i]);
    //         }
    //         printf("]\n    Destination : %u\n    Cost : %ld\n    Number of nodes : %d\n    Path : [", data->max->node, data->max->cost, data->size);
    //         for (int i = 0; i < data->size; i++) {
    //             printf(" %d", data->path[i]);
    //         }
    //         printf(" ]\n");
    //         writecount++;
    //     } else {

    //         //Writes the graph computed data to the file (checks for first pass to write node info to the file)
    //         if (first_pass) {
    //             uint32_t nb_nodes_be = htonl(graph->file_infos->nb_nodes);
    //             if (fwrite(&nb_nodes_be, sizeof(uint32_t), 1, file) != 1) {
    //                 printf("Error: Failed to write source node.\n");
    //                 error = true;
    //                 pthread_exit(NULL);
    //             }
    //             first_pass = false;
    //         }
    //         wrote = write_to_file(file, data->source, data->max, data->size, data->path);
    //         if (wrote == 1) {
    //             printf("Error writing to file\n");
    //             error = true;
    //             pthread_exit(NULL);
    //         }
    //         writecount++;
    //     }
    //     free_ford_struct(data->ford);
    //     free_max_struct(data->max);
    //     free_path(data->path);

    //     //Checks if it has done everything, if it has, it frees the data and exits (in a mutex to avoid a race condition)
    //     pthread_mutex_lock(&write_mutex);
    //     if(writecount == graph->file_infos->nb_nodes){
    //             pthread_mutex_unlock(&write_mutex);
    //             printf("emergencey exit \n");
    //             pthread_exit(NULL);
    //     }
    //     pthread_mutex_unlock(&write_mutex);
    // }
 

/*
Function submain
---------------------------

Function executed by the main thread, it executes the other threads and manages the buffers

Input:
----------
sub_data_t *sub_data : the data structure containing the arguments of the program

Output:
----------
int : 0 if everything went well, 1 if there was an error
*/
int submain(sub_data_t *sub_data) {
    
    //Initialization of the information needed for the function
    //It is passed by the main function in the sub_data structure
    int grph;
    error = false;
    verbose = sub_data->verbose;
    NTHREADS = sub_data->nb_threads;
    BUFFERSIZE = NTHREADS+1;

    if (verbose){printf("NTHREADS : %d\n", NTHREADS);}
    if (verbose){printf("BUFFERSIZE : %d\n", BUFFERSIZE);}

    readbuffer = malloc(sizeof(uint32_t) * BUFFERSIZE);
    writebuffer = malloc(sizeof(thread_data_t) * BUFFERSIZE);
    if (sub_data->print){
        show = true;
    }

    //Initializes the threads, mutexes and conditions
    pthread_t thread[NTHREADS];
    pthread_t reader, writer;
    sem_init(&read_sem, 0, BUFFERSIZE);
    sem_init(&compute_sem, 0, BUFFERSIZE);
    sem_init(&write_sem, 0, BUFFERSIZE);
    pthread_mutex_init(&read_done_mutex, NULL);
    pthread_mutex_init(&compute_done_mutex, NULL);
    // pthread_cond_init(&broad, NULL);


    //start read thread
    grph = pthread_create(&reader, NULL, readthread, (void *) sub_data->inputfile);
    if (grph != 0) {
        printf("Error creating thread\n");
        return 1;
    }

    //start computer threads
    for (int thrd = 0; thrd < NTHREADS; thrd++) {
        pthread_create(&thread[thrd], NULL, computers, NULL);
    }

    //start write thread
    pthread_create(&writer, NULL, writethread, (void *) sub_data->outputfile);

    
    //when the read thread is done, set read_done to true and signal it to the computer threads
    pthread_join(reader, NULL);
  

    pthread_join(writer, NULL);
    printf("read_done = %d\n", read_done);
     sem_destroy(&read_sem);
    sem_destroy(&compute_sem);
    sem_destroy(&write_sem);

    pthread_mutex_destroy(&read_done_mutex);
    pthread_mutex_destroy(&compute_done_mutex);
    // pthread_cond_destroy(&broad);

    free_graph_struct(graph);
    free(readbuffer);
    free(writebuffer);
    
    if (error) {return 1;}
    return 0;
}   
    


