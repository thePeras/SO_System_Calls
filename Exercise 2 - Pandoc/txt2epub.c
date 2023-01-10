#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main (int argc, char* argv[]){
    pid_t pid;
    if (argc == 1) {
        fprintf(stderr, "Faltam argumentos\nUsage: %s <file1.txt> [... file_n.txt] \n", argv[0]);
        return EXIT_FAILURE;
    }
    
    char **zip_arguments = (char **)malloc((argc + 2) * sizeof(char *));
    zip_arguments[0] = (char *)malloc(3 * sizeof(char));
    strcpy(zip_arguments[0], "zip");
    zip_arguments[1] = (char *)malloc(10 * sizeof(char));
    strcpy(zip_arguments[1], "ebooks.zip");

    for(int i = 1; i<argc; i++){
        zip_arguments[i+1] = malloc(strlen(argv[i]) + 1);
        stpncpy(zip_arguments[i+1], argv[i], strlen(argv[i]) - 4);
        strcat(zip_arguments[i+1], ".epub");
    }

    for (int i = 1; i < argc; i++){ 
        if((pid = fork()) == - 1){
            fprintf(stderr, "pandoc: Não conseguiu dar fork: %s\n", strerror(errno));
            continue;
        }
        else if(pid == 0){
            char* title = malloc(strlen(argv[i]) + 7);
            strcpy(title, "title=");
            strncat(title, argv[i], strlen(argv[i]) - 4);
            title[strlen(argv[i]) + 6] = '\0';

            char* txt_name = malloc(strlen(argv[i]) - 1);
            stpncpy(txt_name, argv[i], strlen(argv[i]) - 4);
            strcat(txt_name, ".txt");

            char* pandoc_arguments[] = { "pandoc", txt_name, "-o", zip_arguments[i+1], "--metadata", title, NULL };

            printf("[pid%d] converting %s..\n", getpid(), txt_name); 
            execvp(pandoc_arguments[0], pandoc_arguments);
            fprintf(stderr, "pandoc: Comando não executado: %s\n", strerror(errno));
            continue; 
        }
    }

    for(int i = 0; i < argc; i++){
        wait(NULL);
    }
    
    printf("A criar ebooks.zip..\n"); 
    for (int i = 0; i < argc+1; i++){
        printf("%s ", zip_arguments[i]);
    }
    execvp(zip_arguments[0], zip_arguments);

    // This only runs if execvp fails
    fprintf(stderr, "zip: Comando não executado: %s\n", strerror(errno));
    return EXIT_FAILURE;
}

