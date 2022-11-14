#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    
    if(argc < 4) {
        fprintf(stderr, "faltam argumentos \nUsage: %s <number_processes> <probability_to_sleep> <time_to_sleep> \n", argv[0]);
        return EXIT_FAILURE;
    }
    if(argc > 4) {
        fprintf(stderr, "argumentos a mais \nUsage: %s <number_processes> <probability_to_sleep> <time_to_sleep> \n", argv[0]);
        return EXIT_FAILURE;
    }
    int numberProc = atoi(argv[1]);
    int probability = atoi(argv[2]);
    int time = atoi(argv[3]);
    if (numberProc == 0) {
        fprintf(stderr, "numero de processos invalido \n");
        return EXIT_FAILURE;
    }
    if (probability == 0) {
        fprintf(stderr, "probabilidade invalida \n");
        return EXIT_FAILURE;
    }
    if (time == 0) {
        fprintf(stderr, "tempo invalido \n");
    }
    return EXIT_SUCCESS;
}