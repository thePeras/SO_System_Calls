#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include<string.h>

int main (int argc, char* argv[]){
    pid_t pid;

    for (int i = 1; i < argc; i++){

        char command[1024];
        char* current_file = argv[i]; 
        char* file_name[1024];

        char pandoc[] = "pandoc "; 
        char output[] = " -o ";
        char epub[] = ".epub";

        strcat(command, pandoc);
        strcat(command, output);
        strncpy(file_name, current_file, strlen(current_file)-4);
        strcat(command, epub);

        if((pid = fork()) == - 1){
            fprintf(stderr, "%s: can´t fork command: %s\n", pandoc , sterror(errno));
            continue;
        }

        else if(pid == 0){
           fprintf("[pid%d] converting %s...",getpid(),current_file);
           execlp("pandoc", command,NULL);
           fprintf(stderr, "%s: can´t fork command: %s\n", pandoc , sterror(errno));
           exit(EXIT_FAILURE);

        }









    }





}

