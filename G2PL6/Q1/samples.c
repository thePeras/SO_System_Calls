#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool offset_found(int* offsets, int i, long offset) {
    for(int j=0; j<i; j++){
        if(offsets[j] == offset) return true;
    }
    return false;
}

int main(int argc, char** argv) {
    long size;
    int numberfrags;
    int maxfragsize;

    if(argc < 4) {
        fprintf(stderr, "faltam argumentos \nUsage: %s <file> <numberfrags> <maxfragsize> \n", argv[0]);
        return EXIT_FAILURE;
    }
    if(argc > 4) {
        fprintf(stderr, "argumentos a mais \nUsage: %s <file> <numberfrags> <maxfragsize> \n", argv[0]);
        return EXIT_FAILURE;
    }

    numberfrags = atoi(argv[2]);
    if (numberfrags == 0) {
        fprintf(stderr, "numero de fragmentos invalido \n");
        return EXIT_FAILURE;
    }

    int* offsets = (int*)malloc(sizeof(int) * numberfrags);
    if (offsets == NULL) {
        fprintf(stderr, "erro ao alocar memoria \n");
        return EXIT_FAILURE;
    }

    maxfragsize = atoi(argv[3]);
    if (maxfragsize == 0) {
        fprintf(stderr, "numero de fragmentos invalido \n");
        return EXIT_FAILURE;
    }


    FILE* file = fopen(argv[1], "r");

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    if(size < maxfragsize){
        fprintf(stderr, "tamanho do ficheiro menor que o tamanho maximo de fragmentos \n");
        return EXIT_FAILURE;
    }
    
    //start the seed random
    srandom(0);

    //generate random numbers
    char* buffer = (char*)malloc(sizeof(char) * maxfragsize);

    int maxfrags = size - maxfragsize < numberfrags ? size - maxfragsize + 1 : numberfrags;

    for (int i = 0; i < maxfrags; i++) {
        long offset = random() % (size - maxfragsize + 1);

        while(offset_found(offsets, i, offset)) {
            offset = random() % (size - maxfragsize + 1);
        }

        offsets[i] = offset;

        fseek(file, offset, SEEK_SET);
        fread(buffer, sizeof(char), maxfragsize, file);
        for (int i = 0; i < maxfragsize; i++) {
            if (buffer[i] == '\n') buffer[i] = ' ';
        }
        fprintf(stdout, ">%s< \n", buffer);
    }

    if(size - maxfragsize + 1 < numberfrags) {
        fprintf(stderr, "foram apenas apresentados %ld fragmentos dos %d totais devido a falta de fragmentos possíveis \n", size - maxfragsize + 1, numberfrags);
        return EXIT_FAILURE;
    }

    free(buffer);
    free(offsets);

    return EXIT_SUCCESS;
}