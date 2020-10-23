#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/resource.h>

#define PIPENAME "./Queue"

// Scheduling function that processes both FIFO and RR scheduling
int schedule(int scheduler) {
    int systime = 0;
    int quantum = 5; // quantam set to 5 'systime' values
    printf(" PID BURST SYSTIME\n");

    // Loop forever...
    while(1) {
        int pid;
        int burst;

        // Open the pipe file, OPEN has automatic lock that waits if is opened as write elsewhere
        int file = open(PIPENAME, O_CREAT | O_RDWR);
        if (file == -1) {
            printf("File open error\n");
            exit(EXIT_SUCCESS);
        }

        // Read two lines from the file with first being PID and second being burst time
        read(file, &pid, sizeof(int));
        read(file, &burst, sizeof(int));

        // If Round Robin...
        if (scheduler == 1) {
            int timer = 0;
            // Count through the processes burst time and if it hits 0, print it
            while(timer != quantum) {
                systime++;
                burst--;
                if (burst == 0) {
                    printf("%4d %5d %7d\n", pid, burst, systime);
                    close(file);
                    break;
                }
                timer++;
            }
            // If it's burst time isn't 0, write it and its updated burst time back to end of file
            write(file, &pid, sizeof(int));
            write(file, &burst, sizeof(int));
            close(file);
        // If FCFS...
        } else if (scheduler == 2) {
            close(file);
            int timer = 0;
            // Incremenet systime with the burst time of the process
            while (timer != burst) {
                systime++;
                timer++;
            }
            printf("%4d %5d %7d\n", pid, burst, systime);
        } else {
            printf("No scheduler type selected. Should not have gotten this far\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    mkfifo(PIPENAME, 0666); // Make a special "FIFO" file
    int scheduler;
    int n = 0;

    // If process argument is invalid
    if (argv[1] != NULL) {
        n = atoi(argv[1]);
    } else if (n < 1) {
        printf("Invalid number of processes given\n");
    } else {
        printf("Not enough commands, please use ./[executable] [number of processes] [scheduler type]\n -f - FCFS\n -r - Round Robin\n");
        exit(EXIT_SUCCESS);
    }

    // If type of scheduler provided is invalid
    if (argv[2] == NULL) {
        printf("Not enough commands, please use ./[executable] [number of processes] [scheduler type]\n -f - FCFS\n -r - Round Robin\n");
        exit(EXIT_FAILURE);
    } else {
        if (strcmp(argv[2], "-r") == 0) {
            scheduler = 1;
        }
        if (strcmp(argv[2], "-f") == 0) {
            scheduler = 2;
        }
    }

    // Loop and create the needed amount of processes
    for (int i = 0; i < n; ++i) {
        char *file = "./DummyProcess";
        char processnum[5];
        sprintf(processnum, "%d", i);
        char *modifier = "-s";

        // Build args array to pass to dummy process
        char *args[5];
        args[0] = "./DummyProcess";
        args[1] = argv[1];
        args[2] = modifier;
        args[3] = processnum;
        args[4] = NULL;

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
    // Begin scheduling
    schedule(scheduler);
}