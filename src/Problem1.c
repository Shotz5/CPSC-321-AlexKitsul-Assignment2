#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/resource.h>

int main(int argc, char *argv[]) {
    int n = 0;

    if (argv[1] != NULL) {
        n = atoi(argv[1]);
    } else if (n < 1) {
        printf("Invalid number of processes given\n");
    } else {
        printf("Not enough commands, please use ./[executable] [number of processes]\n");
        exit(EXIT_SUCCESS);
    }

    // Loop and create the needed amount of processes
    for (int i = 0; i < n; i++) {
        char *file = "./DummyProcess";
        char processnum[5];
        sprintf(processnum, "%d", i);

        // Build args array to pass to dummy process
        char *args[4];
        args[0] = "./DummyProcess";
        args[1] = argv[1];
        args[2] = processnum;
        args[3] = NULL;

        // Fork and execute if child, repeat loop if parent
        int forking = fork();
        if (forking > 0) {
            continue;
        } else if (forking == 0) {
            execvp(args[0], args);
            exit(EXIT_SUCCESS);
        } else {
            printf("Errrrrror\n");
            exit(EXIT_FAILURE);
        }
    }

    wait(NULL);
}
