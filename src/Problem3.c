#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/resource.h>

int main(int argc, char *argv[]) {
    int n = 0;

    if (argv[1] != NULL) {
        n = atoi(argv[1]);
    } else {
        printf("Invalid number of processes given\n");
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < n; i++) {
        char *file = "./DummyProcess";
        char processnum[5];
        sprintf(processnum, "%d", i);
        char *modifier = "-h";

        char *args[5];
        args[0] = "./DummyProcess";
        args[1] = argv[1];
        args[2] = modifier;
        args[3] = processnum;
        args[4] = NULL;

        if (fork() == 0) {
            execvp(args[0], args);
            exit(EXIT_SUCCESS);
        }
    }

    while (n > 0) {
        wait(NULL);
        n--;
    }
}
