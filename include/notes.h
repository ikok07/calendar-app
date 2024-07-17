//
// Created by Kok_PC on 17.7.2024 г..
//

#ifndef NOTES_H
#define NOTES_H
#include "utils.h"

typedef struct note {
    char text[256];
    Date date;
} Note;

Note createNote();

#endif //NOTES_H
