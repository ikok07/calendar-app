//
// Created by Kok_PC on 18.7.2024 г..
//

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "../include/background_log.h"


void *log_dates(FILE *log_file) {
    while (true) {
        time_t currTime = time(NULL);
        struct tm *local_time = localtime(&currTime);
        char timestamp[256];
        strftime(timestamp, 256, "%x@%H:%M:%S", local_time);
        fprintf(log_file, "Log: %s\n", timestamp);
        sleep(1);
    }

    return NULL;
}

int start_background_log(FILE *log_file) {
    log_file = fopen("../docs/logs.txt", "w");
    if (!log_file) {
        printf("Error opening log file!\n");
        return 1;
    }

    pthread_t thread;
    int return_value = pthread_create(&thread, NULL, log_dates, log_file);
    if (return_value != 0) return 1;
    return_value = pthread_detach(thread);
    if (return_value != 0) return 1;
    return 0;
}
