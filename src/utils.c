//
// Created by Kok_PC on 17.7.2024 г..
//

#include <stdio.h>
#include "../include/utils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void checkIfAllocated(void *arg) {
    if (!arg) {
        printf("Failed to allocate enough memory!\n");
        exit(1);
    }
}

void insertString(char ***strArr, const char *str, const size_t strArrSize) {
    char **newSplitted = realloc(*strArr, sizeof(char*) * strArrSize);
    checkIfAllocated(newSplitted);
    *strArr = newSplitted;
    (*strArr)[strArrSize - 1] = malloc(strlen(str) + 1);
    strcpy_s((*strArr)[strArrSize - 1], strlen(str) + 1, str);
}

errno_t splitString(char ***destStr, const char *str) {
    int length = strlen(str);
    if (length == 0) return 1;
    char **splittedString = malloc(sizeof(char*));
    checkIfAllocated(splittedString);
    int strCount = 0;
    int charCount = 1;
    char *currStr = malloc(sizeof(char));
    for (int i = 0; i < length; i++) {
        if (str[i] != ' ' && str[i] != '\0') {
            char *newStr = realloc(currStr, sizeof(char) * ++charCount);
            checkIfAllocated(newStr);
            currStr = newStr;
            currStr[charCount - 2] = str[i];
            continue;
        }
        if (charCount == 0) continue;
        currStr[charCount - 1] = '\0';
        insertString(&splittedString, currStr, ++strCount);
        charCount = 1;
        currStr = malloc(sizeof(char) * charCount);
        checkIfAllocated(currStr);
    }
    currStr[charCount - 1] = '\0';
    if (strlen(currStr) > 1) {
        insertString(&splittedString, currStr, ++strCount);
    }

    free(currStr);
    *destStr = splittedString;
    return 0;
}

bool yearIsLeap(const int year) {
    bool yearIsLeap;

    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) yearIsLeap = true;
            else yearIsLeap = false;
        } else {
            yearIsLeap = true;
        }
    } else {
        yearIsLeap = false;
    }
    return yearIsLeap;
}

int daysInMonth(int month, int year) {
    bool isLeap = yearIsLeap(year);
    int daysInMonth;

    if (isLeap && month == 2) daysInMonth = 29;
    else if (!isLeap && month == 2) daysInMonth = 28;
    else if (month < 8) {
        if (month % 2 == 0) daysInMonth = 30;
        else daysInMonth = 31;
    } else {
        if (month % 2 == 0) daysInMonth = 31;
        else daysInMonth = 30;
    }
    return daysInMonth;
}

typedef enum month {
    JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
} Month;

char *monthName(const int month) {
    switch (month) {
        case JAN: return "Jan";
        case FEB: return "Feb";
        case MAR: return "Mar";
        case APR: return "Apr";
        case MAY: return "May";
        case JUN: return "Jun";
        case JUL: return "Jul";
        case AUG: return "Aug";
        case SEP: return "Sep";
        case OCT: return "Oct";
        case NOV: return "Nov";
        case DEC: return "Dec";
        default: return "Unknown";
    }
}

int datecmp(const Date d1, const Date d2) {
    if (d1.day == d2.day && d1.month == d2.month && d1.year == d2.year) return 0;
    if (d1.year != d2.year) return d1.year < d2.year ? 1 : -1;
    if (d1.month == d2.month) return d1.month < d2.month ? 1 : -1;
    return d1.day < d2.day ? 1 : -1;
}

void loadEnvFile() {
    FILE *fp = fopen("../secrets.env", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open .env file!");
        return;
    }

    int curr_line_size = 0;
    char *curr_line = malloc(curr_line_size);
    char curr_char;
    while ((curr_char = fgetc(fp)) != EOF) {
        if (curr_char == '\n') {
            curr_line[curr_line_size] = '\0';
            if (curr_line[0] == '#') {
                curr_line_size = 0;
                curr_line = malloc(curr_line_size);
            }
            putenv(curr_line);
            curr_line_size = 0;
            curr_line = malloc(curr_line_size);
            continue;
        }
        char *temp_ptr = realloc(curr_line, ++curr_line_size);
        if (temp_ptr == NULL) {
            printf("Failed to load ENV file!");
            exit(1);
        }
        curr_line = temp_ptr;
        curr_line[curr_line_size - 1] = curr_char;
    }

    if (curr_line_size > 0) {
        curr_line[curr_line_size] = '\0';
        putenv(curr_line);
    }

    fclose(fp);
}

void log_error() {
    fprintf(stderr, "Error: %s\n", strerror(errno));
}