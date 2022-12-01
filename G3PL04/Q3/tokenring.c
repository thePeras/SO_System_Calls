#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

// Custom mod function
static int mod(int a, int b){
    int r = a % b;
    return r < 0 ? r + b : r;
}

// Global variables
pid_t parentPID;
int numberProc;

// Process variables
int process_pipe_in;
int process_pipe_out;
char* process_pipe_name;

// SIGINT Signal handler
static void close_pipes(){
    if (getpid() == parentPID) {
        for (int i = 0; i < numberProc; i++) wait(NULL);
        printf("\n");
    }
    else {
        // Closing pipes
        if (close(process_pipe_in) < 0) {
            fprintf(stderr, "Erro a fechar pipe de input. %s\n", strerror(errno));
        }
        if (close(process_pipe_out) < 0) {
            fprintf(stderr, "Erro a fechar pipe de output. %s\n", strerror(errno));
        }
        // Unlinking pipe
        if (unlink(process_pipe_name) < 0) {
            fprintf(stderr, "Erro a dar unlink ao pipe. %s\n", strerror(errno));
        }
    }

    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    parentPID = getpid();
    int token = 0;

    // Overwriting SIGINT handler
    if(signal(SIGINT, close_pipes) == SIG_ERR) {
        fprintf(stderr, "Não consegue apanhar SIGINT: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Checking arguments
    if(argc < 4) {
        fprintf(stderr, "Faltam argumentos \nUsage: %s <number_processes> <probability_to_sleep> <time_to_sleep> \n", argv[0]);
        return EXIT_FAILURE;
    }
    if(argc > 4) {
        fprintf(stderr, "Argumentos a mais \nUsage: %s <number_processes> <probability_to_sleep> <time_to_sleep> \n", argv[0]);
        return EXIT_FAILURE;
    }
    numberProc = atoi(argv[1]);
    float probability = atof(argv[2]);
    int time = atoi(argv[3]);

    if (numberProc <= 1) {
        fprintf(stderr, "Número de processos inválido.\n");
        return EXIT_FAILURE;
    }
    if (probability <= 0 || probability > 1) {
        fprintf(stderr, "Probabilidade inválida. Introduza uma probabilidade entre: ]0, 1] \n");
        return EXIT_FAILURE;
    }
    int probability_int = (int)(probability * 100);
    if (time <= 0) {
        fprintf(stderr, "Tempo inválido \n");
        return EXIT_FAILURE;
    }

    // Create pipes
    char * pipes[numberProc];
    for (int i = 1; i <= numberProc; i++) {
        char pipeName[12];
        sprintf(pipeName, "pipe%dto%d", i, mod(i, numberProc)+ 1);
        //copy the pipe name to the array
        pipes[i-1] = malloc(strlen(pipeName) + 1);
        strcpy(pipes[i-1], pipeName);
        if (mkfifo(pipeName, 0666) == -1) {
            fprintf(stderr, "Erro ao criar pipe %s: %s \n", pipeName, strerror(errno));
            return EXIT_FAILURE;
        }
    }
    
    // Creating processes
    for (int i = 0; i < numberProc; i++) { 
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Erro ao criar processo %d: %s. \n", i + 1, strerror(errno));
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            // Child process 
            srandom(getpid());

            if(i == 0){
                process_pipe_out = open(pipes[0], O_WRONLY); // pipe to write
                if (process_pipe_out < 0) {
                    fprintf(stderr, "Erro ao abrir pipe %s: %s \n", pipes[0], strerror(errno));
                    return EXIT_FAILURE;
                }
                write(process_pipe_out, (char*) &token, sizeof(int));
            }
            char* pipeName = pipes[mod(i-1, numberProc)];
            process_pipe_name = pipeName;
            process_pipe_in = open(pipeName, O_RDONLY); // pipe to read
            if (process_pipe_in < 0) {
                fprintf(stderr, "Erro ao abrir para ler pipe %s: %s \n", pipeName, strerror(errno));
                return EXIT_FAILURE;
            }
            if(i != 0){
                char* pipeName2 = pipes[i];  // pipe to write
                process_pipe_out = open(pipeName2, O_WRONLY);
                if (process_pipe_out < 0) {
                    fprintf(stderr, "Erro ao abrir para escrever pipe %s: %s \n", pipeName2, strerror(errno));
                    return EXIT_FAILURE;
                }
            }
            
            free(pipes[i]);
            
            int nbytes = 0;
            while((nbytes = read(process_pipe_in, (char*) &token, sizeof(int))) > 0) {
                token++;
                int rand = mod(random(), 100);  
                if (rand < probability_int) {
                    printf("[p%d] lock on token (val = %d)\n", i+1, token);
                    sleep(time);
                    printf("[p%d] unlock token \n", i+1);
                }

                if (write(process_pipe_out, (char*)&token, sizeof(int)) < 0) {
                    fprintf(stderr, "Erro ao escrever para o pipe: %s. \n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
    
            fprintf(stderr, "Erro ao ler do pipe: %s. \n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    
    pause();
    fprintf(stderr, "Erro ao receber/processar sinal: %s \n", strerror(errno));
    return EXIT_FAILURE;
}
