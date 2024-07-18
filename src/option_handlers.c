//
// Created by Kok_PC on 17.7.2024 г..
//

#include <stdio.h>
#include "../include/option_handlers.h"

#include <stdlib.h>
#include <time.h>
#include <cjson/cJSON.h>

#include "../include/http_handler.h"
#include "../include/utils.h"
#include "../include/notes.h"

typedef struct tm Time;

void displayCurrentDay() {
    char *date = __DATE__;
    printf("----------- Current date: %s -----------", date);
}

void displayAllDaysOfMonth() {
    time_t currentTime = time(NULL);
    Time *localTime = localtime(&currentTime);
    int month = localTime->tm_mon + 1;
    char *monthStr = monthName(month);
    int year = 1900 + localTime->tm_year;

    int daysCount = daysInMonth(month, year);
    for (int i = 0; i < daysCount; i++) {
        printf("----------- %s %d %d -----------\n", monthStr, i + 1, year);
    }
}

void addNote() {
    Note note = createNote();

    FILE *fp = fopen("../docs/notes.bin", "rb");
    FILE *fpTemp = fopen("../docs/temp.bin", "wb");
    if (!fpTemp) {
        printf("Failed to open file!");
        exit(1);
    }

    int notesCount = 0;
    Note *notesArr = malloc(sizeof(Note) * notesCount);
    Note currNote;

    if (fp) {
        // Copy all data from the original file to the notes array
        while (fread(&currNote, sizeof(Note), 1, fp)) {
            notesCount++;
            Note *tempArr = realloc(notesArr, sizeof(Note) * notesCount);
            if (!tempArr) {
                printf("Memory reallocation failed!\n");
                free(notesArr);
                fclose(fp);
                fclose(fpTemp);
                exit(1);
            }
            notesArr = tempArr;
            notesArr[notesCount - 1] = currNote;
        }
    }

    // allocate one additional space in the array and add the new note
    notesArr = realloc(notesArr, sizeof(Note) * ++notesCount);
    notesArr[notesCount - 1] = note;

    fwrite(notesArr, sizeof(Note), notesCount, fpTemp);
    if (fp) fclose(fp);
    fclose(fpTemp);

    if (fp && remove("../docs/notes.bin") != 0) {
        perror("An error occurred! Please try again! REMOVE_FAILED");
        exit(1);
    }

    if (rename("../docs/temp.bin", "../docs/notes.bin") != 0) {
        perror("An error occurred! Please try again! RENAME_FAILED");
        exit(1);
    }
}

void displayNotesForDate(const Date date) {
    FILE *fp = fopen("../docs/notes.bin", "rb");
    if (!fp) {
        perror("An error occurred. Please try again! FILE_NOT_FOUND");
        exit(1);
    }

    int notesCount = 0;
    Note currNote;
    Note *notes = malloc(sizeof(Note) * notesCount);

    while (fread(&currNote, sizeof(Note), 1, fp)) {
        if (datecmp(date, currNote.date) == 0) {
            Note *tempArr = realloc(notes, sizeof(Note) * ++notesCount);
            if (!tempArr) {
                perror("Failed to allocate memory!");
                free(notes);
                fclose(fp);
                exit(1);
            }
            notes = tempArr;
            notes[notesCount - 1] = currNote;
        }
    }

    if (notesCount > 0) {
        printf("----------- NOTES -----------\n");
        for (int i = 0; i < notesCount; i++) {
            printf("#%d:\n", i + 1);
            printf("%s\n", notes[i].text);
        }
    } else printf("----------- There are no notes available for the selected date! -----------");

    free(notes);
}

void displayLiveDate() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id", 1);
    cJSON_AddStringToObject(json, "name", "John Smith");

    char *json_str = cJSON_Print(json);

    char *response = malloc(1);
    if (make_request("https://jsonplaceholder.typicode.com/posts/1", PUT, json_str, &response) != 0) {
        perror("Failed to fetch live date!");
        return;
    }
    cJSON *json_response = cJSON_Parse(response);
    if (json == NULL) {
        perror("Failed to parse response JSON!");
        return;
    }
    cJSON *id = cJSON_GetObjectItemCaseSensitive(json_response, "id");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(json_response, "name");
    printf("%d\n", id->valueint);
    printf("%s\n", name->valuestring);
}
