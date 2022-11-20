#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

// gcc -Wall txt2epub.c -o txt2epub

int main (int argc, char* argv[]){
    printf("RUNNING\n");
    pid_t pid;
    for (int i = 1; i < argc; i++){ 

        // argv[i] is the .txt file we want to convert
        // i want a string with the name of the .epub file
        char* epub_name = malloc(strlen(argv[i]) + 1);
        strcpy(epub_name, argv[i]);
        epub_name[strlen(argv[i]) - 4] = '\0';
        strcat(epub_name, ".epub");

        //TODO: enchament: set metadata correctly

        // create command: pandoc argv[i](.txt) -o epub_name(.epub) 
        char* txt_name = malloc(strlen(argv[i]) + 1);
        strcpy(txt_name, argv[i]);

        char* arguments[1024] = {"pandoc", txt_name, "-o", epub_name, NULL };

        
        if((pid = fork()) == - 1){
            fprintf(stderr, "pandoc: can't fork command: %s\n", strerror(errno));
            continue;
        }
        else if(pid == 0){
            printf("[pid%d] converting %s..\n", getpid(), txt_name); 
            // execute command
            execvp(arguments[0], arguments);
            fprintf(stderr, "pandoc: can't execute command: %s\n", strerror(errno));
            continue;
        }
        else{
            wait(NULL);
        }
        
        // TODO: zip <zipname> <fiule1> <file2> ...
        
    return EXIT_SUCCESS;
    }
}

