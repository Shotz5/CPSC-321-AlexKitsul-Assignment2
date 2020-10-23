#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/resource.h>

#define PIPENAME "./Queue"

int main(int argc, char *argv[]) {
    int scheduler = 0;
    int half = 0;
    int processnum;
    int file;

    // If provided 4 args, will be either Problem 3 or Problem 4
    if (argc == 4) {
        // If Problem4
        if (strcmp(argv[2], "-s") == 0) {
            processnum = atoi(argv[3]);
            scheduler = 1;
        // If Problem3
        } else if (strcmp(argv[2], "-h") == 0) {
            processnum = atoi(argv[3]);
            half = 1;
        // No modifier given, something wrong
        } else {
            printf("Invalid modifier supplied\n Options: -s - Execute with scheduler\n    -h - Execute with half priority split\n");
        }
    // If too many parameters, halt
    } else if (argc > 4) {
        printf("Too many params supplied\n Options: -s - Execute with scheduler\n    -h - Execute with half priority split\n");
        exit(EXIT_SUCCESS);
    // If Problem1
    } else if (argc == 3) {
        processnum = atoi(argv[2]);
    // If error in parameter pass occurs
    } else if (argc <= 2) {
        printf("Not enough params given. Need ./[exeutable] [# of processes] [modifier]\n Modifiers: -s - Execute with scheduler\n    -h - Execute with half priority split\n");
        exit(EXIT_FAILURE);
    }    

    // Set nice values if Problem3
    if (half == 1) {
        if (processnum % 2 == 0) {
            nice(-15);
        } else {
            nice(15);
        }
    }

    // Loop forever printing pid/niceness or writing to file
    while(1) {
        int pid = getpid();
        // If using a scheduling algo
        if (scheduler == 1) {
            // Use nanosecond clock for seeding random burst time
            struct timespec time;
            clock_gettime(CLOCK_MONOTONIC, &time);
            srand(time.tv_nsec);
            int burst = ((rand() % 10) + 1);

            // Open pipe file
            file = open(PIPENAME, O_CREAT | O_WRONLY);
            if (file == -1) {
                printf("Unable to open file\n");
                exit(EXIT_SUCCESS);
            }

            // Write PID and burst time to file, then halt for 1 second
            write(file, &pid, sizeof(pid));
            write(file, &burst, sizeof(burst));
            close(file);
            sleep(1);
        } else {
            printf("PID: %d / Nice: %d\n", getpid(), nice(0));
            sleep(1);
        }
    }
}