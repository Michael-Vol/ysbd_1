#ifndef UTIL_H
#define UTIL_H

#define RECORD_SIZE 94      // the size of each record in bytes
#define NB_POINTER_POS 500  // the position of the pointer to the next block
#define NUM_ENTRIES_POS 508 // the position of the integer that holds the number of records in the block

typedef struct
{
  int id;
  char name[15];
  char surname[25];
  char address[50];
} Record;

Record *createRecord(int id, char name[15], char surname[25], char address[50]);

int getNumEntries(void *block);                     // returns the number of entries of a block
void *getNextBlockP(void *block);                   // returns the pointer of the next block
void *jumpToNextEntry(void *block);                 // returns the next entry
int increaseNumEntries(void *block);                // increases the number of entries at the end of the block by 1
int idIsUnique(void *firstBlock, int id);           // return 0 if the id is not unique and 1 if it is unique
void *findEntryWithId(void *firstBlock, int id);    // returns the entry with this id
int findBlockNumber(void *firstBlock, int id);      // reuturns the block index that this id is in
int findEntriesToPrint(void *firstBlock, void *id); // finds and prints the entry with this id
int printEntry(Record *entry);                      // prints the entry given
void initBlock(void *block);                        // initiallize the block

#endif