#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#define NUMBERSTRINGSIZE 12

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

    // TODO: numberProc < 1000

    // create the pipes
    int i;
    for (i = 1; i <= numberProc; i++) {
        char pipeName[12]; 
        sprintf(pipeName, "pipe%dto%d", i, (i % numberProc) + 1);
        if (mkfifo(pipeName, 0666) == -1) {
            fprintf(stderr, "erro ao criar pipe %s \n", pipeName);
            return EXIT_FAILURE;
        }
    }

    // create the processes
    for (i = 1; i < numberProc; i++) { //i=2
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "erro ao criar processo %d \n", i);
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            // child process 3
            // open the pipes 2->3
            sprintf(pipeName, "pipe%dto%d", i, (i % numberProc) + 1);   // pipe to read
            FILE *pipe = fopen(pipeName, "r");
            if (pipe == NULL) {
                fprintf(stderr, "erro ao abrir pipe %s \n", pipeName);
                return EXIT_FAILURE;
            }
            char pipeName2[12];
            sprintf(pipeName2, "pipe%dto%d", i, (i + 1) % numberProc);  // pipe to write
            FILE *pipe2 = fopen(pipeName2, "w");
            if (pipe2 == NULL) {
                fprintf(stderr, "erro ao abrir pipe %s \n", pipeName2);
                return EXIT_FAILURE;
            }

            int number;
            char stringnumber[NUMBERSTRINGSIZE];
            // read(pipe, &number, sizeof(int));
            read(pipe, stringnumber, NUMBERSTRINGSIZE);
            
            number = atoi(stringnumber);
            number++;
            sprintf(stringnumber, "%d", number);
            
            int random = random() % 100;  // 0 - 99
            if (random < probability) {
                printf("[p%d] lock on token (val = %d)\n", i, number);
                sleep(time);
                printf("[p%d] unlock token\n");
            }
            
            if (write(pipe2, stringnumber, strlen(stringnumber)) < 0) {
                fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            fclose(pipe);
            fclose(pipe2);
        }
        
    }
    // pid_t processIDs[numberProc];
    while(true){
        int i;
        i % numberProc + 1;
        // lock based on pid

        i++;
    }


    // wait for all the processes to finish
    for (i = 1; i < numberProc; i++) {
        wait(NULL);
    }

    // delete the pipes


    // 1- Criar pipes e conectar
    // 2- Criar processos
    // 3- Enviar token
        // 2.1- Calcular probabilidade

    return EXIT_SUCCESS;
}
