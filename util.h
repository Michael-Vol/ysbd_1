#ifndef UTIL_H
#define UTIL_H

#define RECORD_SIZE 94

typedef struct
{
    int id;
    char name[15];
    char surname[25];
    char address[50];
} Record;

int getNumEntries(void *block);
void *getNextBlockP(void *block);
void *jumpToNextEntry(void *block);
int increaseNumEntries(void *block);
int idIsUnique(void *firstBlock, int id);
void *findEntryWithId(void *firstBlock, int id);
int findBlockNumber(void *firstBlock, int id);
int findEntriesToPrint(void *firstBlock, int id);
int printEntry(Record *entry);
void initBlock(void *block);

#endif