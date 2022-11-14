#include <stdio.h>
#include <stdlib.h>
int main (int argc, char** argv) {

    if (argc != 3) {
        fprintf (stderr, "faltam argumentos");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}