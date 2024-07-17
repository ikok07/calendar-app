//
// Created by Kok_PC on 17.7.2024 г..
//

#include <stdio.h>
#include "../include/notes.h"

Note createNote() {
    Note note;
    printf("Note content:\n");
    scanf("\n");
    scanf("%[^\n]%*c", note.text);
    printf("Note's date:\n");
    printf("Day:\n");
    scanf_s("%d", &note.date.day, sizeof(int));
    printf("Month:\n");
    scanf_s("%d", &note.date.month, sizeof(int));
    printf("Year:\n");
    scanf_s("%d", &note.date.year, sizeof(int));
    return note;
}
