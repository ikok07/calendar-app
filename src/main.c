#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/background_log.h"
#include "../include/option_handlers.h"
#include "../include/utils.h"


void askUserForOption(int *option) {
    printf("Select option:\n");
    printf("1. Display current day\n");
    printf("2. Display all the days of the current month\n");
    printf("3. Add note\n");
    printf("4. Display note\n");
    printf("5. Fetch live date\n");
    printf("6. Exit\n");
    scanf_s("%d", option, sizeof(int));
}

void executeCommand(const int option) {
    switch (option) {
        case 1:
            displayCurrentDay();
            break;
        case 2:
            displayAllDaysOfMonth();
            break;
        case 3:
            addNote();
            break;
        case 4:
            Date date;
            printf("Note's day:\n");
            scanf_s("%d", &date.day, sizeof(int));
            printf("Note's month:\n");
            scanf_s("%d", &date.month, sizeof(int));
            printf("Note's year:\n");
            scanf_s("%d", &date.year, sizeof(int));
            displayNotesForDate(date);
            break;
        case 5:
            displayLiveDate();
            break;
        case 6:
            exit(0);
        default:
            printf("Invalid option selected. Please try again!\n");
    }
    printf("\n");
}

FILE *log_file;

int main(const int argc, const char **argv) {
    int option = 0;
    bool loggingEnabled = false;
    printf("The Callendar App!\n");
    if (argc > 1) {
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "--log")) loggingEnabled = true;
        }
    }

    if (loggingEnabled && start_background_log(log_file) != 0) return 1;

    while (option >= 0) {
        askUserForOption(&option);
        executeCommand(option);
    }

    fclose(log_file);
    return 0;
}
