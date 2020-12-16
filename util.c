#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Record *createRecord(int id, char name[15], char surname[25], char address[50]) {
  Record *record = malloc(sizeof(Record));
  record->id = id;
  strcpy(record->address, address);
  strcpy(record->surname, surname);
  strcpy(record->name, name);
  return record;
}

int getNumEntries(void *block) {
  void *blockPointer = block;
  int *numEntriesP = blockPointer + NUM_ENTRIES_POS;
  return *numEntriesP;
}
void *getNextBlockP(void *block) {
  void *blockPointer = block;
  void *NextBlockP = blockPointer + NB_POINTER_POS;
  return NextBlockP;
}
void *jumpToNextEntry(void *block) {
  void *blockPointer = block;
  void *NextRecordP = blockPointer + RECORD_SIZE;
  return NextRecordP;
}
int increaseNumEntries(void *block) {
  void *blockPointer = block;
  int *numEntriesP = blockPointer + NUM_ENTRIES_POS;
  int newEntries = *(numEntriesP)++;
  return newEntries;
}

void initBlock(void *block) {
  void *blockPointer = block;
  void *NextBlockP = blockPointer + NB_POINTER_POS;
  int *numEntriesP = blockPointer + NUM_ENTRIES_POS;
  void *nullptr = NULL;
  // memcpy(NextBlockP, nullptr, 8);
  *numEntriesP = 0;
}

int idIsUnique(void *firstBlock, int id) {
  void *currentBlock = firstBlock;
  while (currentBlock != NULL) {
    int numEntries = getNumEntries(currentBlock);
    Record *currentEntry = (Record *)currentBlock;
    for (int i = 0; i < numEntries; i++) {
      if (currentEntry->id == id) {
        return 0;
      }
      currentEntry = jumpToNextEntry(currentBlock);
    }
    currentBlock = getNextBlockP(currentBlock);
  }
  return 1;
}

void *findEntryWithId(void *firstBlock, int id) {
  void *currentBlock = firstBlock;
  while (currentBlock != NULL) {
    int numEntries = getNumEntries(currentBlock);
    Record *currentEntry = (Record *)currentBlock;
    for (int i = 0; i < numEntries; i++) {
      if (currentEntry->id == id) {
        return currentEntry;
      }
      currentEntry = jumpToNextEntry(currentBlock);
    }
    currentBlock = getNextBlockP(currentBlock);
  }
  return NULL;
}
int findBlockNumber(void *firstBlock, int id) {
  void *currentBlock = firstBlock;
  int blockCounter = 0;
  while (currentBlock != NULL) {
    int numEntries = getNumEntries(currentBlock);
    Record *currentEntry = (Record *)currentBlock;
    for (int i = 0; i < numEntries; i++) {
      if (currentEntry->id == id) {
        return blockCounter;
      }
      currentEntry = jumpToNextEntry(currentBlock);
    }
    currentBlock = getNextBlockP(currentBlock);
    blockCounter++;
  }
  return -1;
}

int printEntry(Record *entry) {
  printf("Id: %d \n", entry->id);
  printf("Name: %s \n", entry->name);
  printf("Surname: %s \n", entry->surname);
  printf("Address: %s \n\n", entry->address);
  return 0;
}

int findEntriesToPrint(void *firstBlock, void *id) {
  void *currentBlock = firstBlock;
  int blockCounter = 0;
  while (currentBlock != NULL) {
    int numEntries = getNumEntries(currentBlock);
    Record *currentEntry = (Record *)currentBlock;
    for (int i = 0; i < numEntries; i++) {
      if (id == NULL) {
        printEntry(currentEntry);
      } else if (currentEntry->id == *((int *)id)) {
        printEntry(currentEntry);
        return blockCounter;
      }
      currentEntry = jumpToNextEntry(currentBlock);
    }
    blockCounter++;
    currentBlock = getNextBlockP(currentBlock);
  }
  return 0;
}