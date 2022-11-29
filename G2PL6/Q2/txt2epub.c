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
    char* zip_arguments[argc+2];
    zip_arguments[0] = "zip";
    zip_arguments[1] = "ebooks.zip";
    zip_arguments[argc+1] = NULL;

    for (int i = 1; i < argc; i++){ 
        char* epub_name = malloc(strlen(argv[i]) + 1);
        strcpy(epub_name, argv[i]);
        epub_name[strlen(argv[i]) - 4] = '\0';
        strcat(epub_name, ".epub");

        char* title = malloc(strlen(argv[i]) + 7);
        strcpy(title, "title=");
        strcat(title, argv[i]);
        title[strlen(argv[i]) + 6] = '\0';

        zip_arguments[i+1] = epub_name;


        char* txt_name = malloc(strlen(argv[i]) + 1);
        strcpy(txt_name, argv[i]);

        char* pandoc_arguments[] = { "pandoc", txt_name, "-o", epub_name, "--metadata", title, NULL };

        if((pid = fork()) == - 1){
            fprintf(stderr, "pandoc: Não conseguiu dar fork: %s\n", strerror(errno));
            continue;
        }
        else if(pid == 0){
            printf("[pid%d] converting %s..\n", getpid(), txt_name); 
            execvp(pandoc_arguments[0], pandoc_arguments);
            fprintf(stderr, "pandoc: Comando não executado: %s\n", strerror(errno));
            continue; 
        }
        else{
            wait(NULL);
        }
    }
    
    if((pid = fork()) == - 1){
        fprintf(stderr, "zip: Não conseguiu dar fork: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        printf("[pid%d] a criar ebooks.zip..\n", getpid()); 
        execvp(zip_arguments[0], zip_arguments);
        fprintf(stderr, "zip: Comando não executado: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else{
        wait(NULL);
    }

    return EXIT_SUCCESS;
}

