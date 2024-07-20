//
// Created by Kok_PC on 17.7.2024 г..
//

#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

#define single_day_secs 60 * 60 * 24

typedef struct date {
    int day;
    int month;
    int year;
} Date;

void checkIfAllocated(void *arg);
void insertString(char ***strArr, const char *str, size_t strArrSize);

errno_t splitString(char ***destStr, const char *str);
bool yearIsLeap(int year);
int daysInMonth(int month, int year);
char *monthName(int month);
int datecmp(Date d1, Date d2);
void loadEnvFile();
void log_error();

#endif //UTILS_H
