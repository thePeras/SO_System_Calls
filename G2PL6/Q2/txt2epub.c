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

    for (int i = 1; i < argc; i++){ ;

        // argv[i] is the .txt file we want to convert
        // i want a string with the name of the .epub file
        char* epub_name = malloc(strlen(argv[i]) + 1);
        strcpy(epub_name, argv[i]);
        epub_name[strlen(argv[i]) - 4] = '\0';
        strcat(epub_name, ".epub");

        // create command: pandoc argv[i](.txt) -o epub_name(.epub)
        char command[1024] = "pandoc ";
        char* txt_name = malloc(strlen(argv[i]) + 1);
        strcpy(txt_name, argv[i]);
        strcat(command, txt_name);
        char output[1024] = " -o ";
        strcat(command, output);
        strcat(command, epub_name);

        printf(">> %s   |   ", command);

        if((pid = fork()) == - 1){
            fprintf(stderr, "pandoc: can't fork command: %s\n", strerror(errno));
            continue;
        }
        else if(pid == 0){
            printf("[pid%d] converting %s..\n",getpid(), txt_name); 
            // execute command
            execlp("pandoc", command, (char *)0); // this is the problem
            fprintf(stderr, "pandoc: can't execute command: %s\n", strerror(errno));
            continue;
        }
        else{
            wait(NULL);
        }

        // zip <zipname> <fiule1> <file2> ...
        
    return EXIT_SUCCESS;
    }
}

