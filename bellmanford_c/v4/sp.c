#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

// Keep these includes for macos compatibility.
#include "include/portable_endian.h"
#include "include/portable_semaphore.h"

// Our own architecture independent header.
#include "include/graph.h"
#include "include/mtgraph.h"

typedef struct {
    FILE *input_file;
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;


void usage(char *prog_name) {
    fprintf(stderr, "UTILISATION:\n");
    fprintf(stderr, "    %s [OPTIONS] input_file\n", prog_name);
    fprintf(stderr,
            "    input_file: chemin vers le fichier d'instance representant le "
            "graphe a traiter.\n");
    fprintf(
        stderr,
        "    -f output_file: chemin vers le fichier qui contiendra le resultat "
        "de programme, au format specifie dans l'enonce. Defaut : stdout.\n");
    fprintf(stderr,
            "    -n n_threads: nombre de threads de calcul executant "
            "l'algorithme de plus court chemin. Defaut : 4.\n");
    fprintf(stderr,
            "    -v: autorise les messages de debug. Si ce n'est pas active, "
            "aucun message de ce type ne peut etre affiche, excepte les "
            "messages d'erreur en cas d'echec. Defaut : false.\n");
}

int parse_args(args_t *args, int argc, char *argv[]) {
    memset(args, 0, sizeof(args_t));

    // Valeurs par defaut pour les arguments.
    args->nb_threads = 4;
    args->verbose = false;
    args->output_stream = stdout;
    args->input_file = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "hn:vf:")) != -1) {
        switch (opt) {
            case 'n':
                args->nb_threads = atoi(optarg);
                if (args->nb_threads == 0) {
                    fprintf(stderr,
                            "Le nombre de threads de calcul doit etre superieur a 0 ! Recu : %s\n",
                            optarg);
                    return -1;
                }
                break;
            case 'v':
                args->verbose = true;
                break;
            case 'f':
                args->output_stream = fopen(optarg, "w+");
                if (args->output_stream == NULL) {
                    fprintf(stderr,
                            "Impossible to open the output file %s: %s\n",
                            optarg, strerror(errno));
                    return -1;
                }
                break;
            case '?':
                usage(argv[0]);
                return 1;
            case 'h':
                usage(argv[0]);
                return 1;
            default:
                usage(argv[0]);
                return -1;
        }
    }


    if (optind == argc) {
        fprintf(stderr,
                "Vous devez fournir un fichier d'instance en entree !\n");
        return -1;
    }

    if (NULL == (args->input_file = fopen(argv[optind], "r"))) {
        fprintf(stderr,
                "Impossible d'ouvrir le fichier qui contient l'instance de graphe %s: %s\n",
                argv[optind], strerror(errno));
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {

    /*Part that was given to us in the example code*/
    args_t args;
    int err = parse_args(&args, argc, argv);
    if (err == -1) {
        exit(EXIT_FAILURE);
    } else if (err == 1) {
        exit(EXIT_SUCCESS);
    }

    //Now the code 
    //Our file is already opened thanks to the parse_args function
    int passed;
    sub_data_t * sub_data = malloc(sizeof(sub_data_t));
    sub_data->nb_threads = args.nb_threads;
    sub_data->inputfile = args.input_file;
    sub_data->outputfile = args.output_stream;
    sub_data->verbose = args.verbose;
    if (args.output_stream != stdout){
        sub_data->print = false;
    } else {
        sub_data->print = true;
    }
    passed = submain(sub_data);
    if (passed != 0){
        printf("error in submain\n");
        return 1;
    }
    // printf("submain passed\n");
    free(sub_data);

    if (args.output_stream != stdout){
        // printf("closing file\n");
        fclose(args.output_stream);
    }
    return 0;
}