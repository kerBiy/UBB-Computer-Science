#pragma once
#include "entity.h"

typedef struct
{
    Participant items[101];
    int size;
} List;

List createList();

void addParticipant(List *list, Participant participant);
int findIndex(List *list, const char *firstName, const char *lastName);
int deleteParticipant(List *list, const char *firstName, const char *lastName);
int updateParticipant(List *list, const char *firstName, const char *lastName, int newScore);
// void filterParticipantsByScore(List list, int scoreThreshold);
// void filterParticipantsByName(List list, const char *namePrefix);
// void sortParticipantsByName(List list, int ascending);
// void sortParticipantsByScore(List list, int ascending);
// void printParticipants(List list);

