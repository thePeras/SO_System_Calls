#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

// gcc -Wall txt2epub.c -o txt2epub

int main (int argc, char* argv[]){
    // Example:
    // argv = { "txt2epub", "file1.txt", "file1.txt" } -> argc = 3
    // zip_arguments = { "zip", "ebooks.epub", "file1.epub", "file1.txt", NULL }

    pid_t pid;
    char* zip_arguments[argc+2];
    zip_arguments[0] = "zip";
    zip_arguments[1] = "ebooks.zip";
    zip_arguments[argc+1] = NULL;

    for (int i = 1; i < argc; i++){ 
        // argv[i] is the .txt file we want to convert
        // i want a string with the name of the .epub file
        char* epub_name = malloc(strlen(argv[i]) + 1);
        strcpy(epub_name, argv[i]);
        epub_name[strlen(argv[i]) - 4] = '\0';
        strcat(epub_name, ".epub");

        zip_arguments[i+1] = epub_name;

        char* txt_name = malloc(strlen(argv[i]) + 1);
        strcpy(txt_name, argv[i]);

        // TODO: create command: pandoc txt_name(.txt) -o epub_name(.epub)
        char* arguments[] = { "pandoc", txt_name, "-o", epub_name, NULL };

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
    }
    
    if((pid = fork()) == - 1){
        fprintf(stderr, "zip: can't fork command: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        printf("[pid%d] creating ebooks.zip..\n", getpid()); 
        // execute command
        execvp(zip_arguments[0], zip_arguments);
        fprintf(stderr, "zip: can't execute command: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else{
        wait(NULL);
    }

    return EXIT_SUCCESS;
}

