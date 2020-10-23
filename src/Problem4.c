#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/resource.h>

#define PIPENAME "./Queue"

int schedule(int scheduler) {
    int systime = 0;
    int quantum = 5;

    while(1) {
        int pid;
        int burst;

        int file = open(PIPENAME, O_CREAT | O_RDWR);
        if (file == -1) {
            printf("File open error\n");
        }

        read(file, &pid, sizeof(int));
        read(file, &burst, sizeof(int));

        if (scheduler == 1) {
            int timer = 0;
            while(timer != quantum) {
                systime++;
                burst--;
                if (burst == 0) {
                    printf("%d %d %d\n", pid, burst, systime);
                    close(file);
                    break;
                }
                timer++;
            }
            write(file, &pid, sizeof(int));
            write(file, &burst, sizeof(int));
            close(file);
        } else if (scheduler == 2) {
            close(file);
            int timer = 0;
            while (timer != burst) {
                systime++;
                timer++;
            }
            printf("%d %d %d\n", pid, burst, systime);
        } else {
            printf("No scheduler type selected. Should not have gotten this far\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    mkfifo(PIPENAME, 0666);
    int scheduler;
    int n = 0;

    if (argv[1] != NULL) {
        n = atoi(argv[1]);
    } else {
        printf("Invalid number of processes given\n");
        exit(EXIT_SUCCESS);
    }

    if (argv[2] == NULL) {
        printf("Not enough commands, please use ./[executable] [number of processes] [scheduler type]\n -f - FCFS\n -r - Round Robin\n");
        exit(EXIT_FAILURE);
    } else {
        if (strcmp(argv[2], "robin") == 0) {
            scheduler = 1;
        }
        if (strcmp(argv[2], "fcfs") == 0) {
            scheduler = 2;
        }
    }

    for (int i = 0; i < n; ++i) {
        char *file = "./DummyProcess";
        char processnum[5];
        sprintf(processnum, "%d", i);
        char *modifier = "-s";

        char *args[5];
        args[0] = "./DummyProcess";
        args[1] = argv[1];
        args[2] = modifier;
        args[3] = processnum;
        args[4] = NULL;

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
    schedule(scheduler);
}