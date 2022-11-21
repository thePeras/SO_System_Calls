#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

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
    float probability = atof(argv[2]);
    int time = atoi(argv[3]);
    if (numberProc == 0) {
        fprintf(stderr, "numero de processos invalido \n");
        return EXIT_FAILURE;
    }
    if (probability == 0) {
        fprintf(stderr, "probabilidade invalida \n");
        return EXIT_FAILURE;
    }
    int probability_int = (int)(probability * 100);
    if (time == 0) {
        fprintf(stderr, "tempo invalido \n");
    }

    // TODO: numberProc < 1000

    // create the pipes
    for (int i = 1; i <= numberProc; i++) {
        char pipeName[12]; 
        sprintf(pipeName, "pipe%dto%d", i, (i % numberProc) + 1);
        if (mkfifo(pipeName, 0666) == -1) {
            fprintf(stderr, "erro ao criar pipe %s \n", pipeName);
            return EXIT_FAILURE;
        }
    }
    int token = 0;
    int pipe = open("pipe1to2", O_WRONLY | O_NONBLOCK);
    if (pipe < 0) {
        fprintf(stderr, "erro ao abrir pipe pipe1to2 \n");
        return EXIT_FAILURE;
    }
    write(pipe, &token, sizeof(token));
    // create the processes

    for (int i = 1; i < numberProc; i++) { //i=2
        printf("66\n");
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "erro ao criar processo %d \n", i);
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            // child process 3
            // open the pipes 2->3
            char pipeName[12];
            sprintf(pipeName, "pipe%dto%d", i, (i % numberProc) + 1);   // pipe to read
            int pipe = open(pipeName, O_RDONLY);
            if (pipe < 0) {
                fprintf(stderr, "erro ao abrir pipe %s \n", pipeName);
                return EXIT_FAILURE;
            }
            char pipeName2[12];
            sprintf(pipeName2, "pipe%dto%d", i, (i + 1) % numberProc);  // pipe to write
            int pipe2 = open(pipeName2, O_WRONLY);
            if (pipe2 < 0) {
                fprintf(stderr, "erro ao abrir pipe %s \n", pipeName2);
                return EXIT_FAILURE;
            }

            int number, nbytes;
            char stringnumber[NUMBERSTRINGSIZE];
            while((nbytes = read(pipe, stringnumber, NUMBERSTRINGSIZE)) > 0) {
                number = atoi(stringnumber);
                printf("Processo %d recebeu o numero %d \n", i, number);
                number++;
                sprintf(stringnumber, "%d", number);
                // write(pipe2, &number, sizeof(int));
                write(pipe2, stringnumber, NUMBERSTRINGSIZE);
                printf("Processo %d enviou o numero %d \n", i, number);
                if (number % probability_int == 0) {
                    sleep(time);
                }
            }
            
            float rand = random() % 100;  // 0 - 99
            if (rand < probability_int) {
                printf("[p%d] lock on token (val = %d)\n", i, number);
                sleep(time);
                printf("[p%d] unlock token\n", i);
            }
            
            if (write(pipe2, stringnumber, strlen(stringnumber)) < 0) {
                fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            close(pipe);
            close(pipe2);
        } 
    }

    // wait for all the processes to finish
    for (int i = 1; i < numberProc; i++) {
        wait(NULL);
    }

    // delete the pipes


    // 1- Criar pipes e conectar
    // 2- Criar processos
    // 3- Enviar token
        // 2.1- Calcular probabilidade

    return EXIT_SUCCESS;
}
