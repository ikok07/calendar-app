//
// Created by Kok_PC on 17.7.2024 г..
//

#ifndef OPTION_HANDLERS_H
#define OPTION_HANDLERS_H
#include "utils.h"

void displayCurrentDay();
void displayAllDaysOfMonth();
void addNote();
void displayNotesForDate(Date date);
void displayLiveDateForCoordinates(int lat, int lng);
void displayFullDatesFromTimestamps(int num_days, ...);

#endif //OPTION_HANDLERS_H
