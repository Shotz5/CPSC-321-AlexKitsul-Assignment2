#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/resource.h>

#define PIPENAME "./Queue"

int main(int argc, char *argv[]) {
    int scheduler = 0;
    int half = 0;
    int processnum;
    int file;

    if (argc == 4) {
        if (strcmp(argv[2], "-s") == 0) {
            processnum = atoi(argv[3]);
            scheduler = 1;
        } else if (strcmp(argv[2], "-h") == 0) {
            processnum = atoi(argv[3]);
            half = 1;
        } else {
            printf("Invalid modifier supplied\n Options: -s - Execute with scheduler\n    -h - Execute with half priority split\n");
        }
    } else if (argc > 4) {
        printf("Too many params supplied\n Options: -s - Execute with scheduler\n    -h - Execute with half priority split\n");
        exit(EXIT_SUCCESS);
    } else if (argc == 3) {
        processnum = atoi(argv[2]);
    } else if (argc <= 2) {
        printf("Not enough params given. Need ./[exeutable] [# of processes] [modifier]\n Modifiers: -s - Execute with scheduler\n    -h - Execute with half priority split\n");
        exit(EXIT_FAILURE);
    }    

    if (half == 1) {
        if (processnum % 2 == 0) {
            nice(-15);
        } else {
            nice(15);
        }
    }

    while(1) {
        struct timespec time;
        clock_gettime(CLOCK_MONOTONIC, &time);
        srand(time.tv_nsec);

        int pid = getpid();
        int burst = ((rand() % 10) + 1);
        if (scheduler == 1) {
            file = open(PIPENAME, O_CREAT | O_WRONLY);
            if (file == -1) {
                printf("Unable to open file\n");
                exit(EXIT_SUCCESS);
            }
            write(file, &pid, sizeof(pid));
            write(file, &burst, sizeof(burst));
            close(file);
            sleep(1);
        } else {
            printf("%d %10d\n", getpid(), nice(0));
            sleep(1);
        }
    }
}