#ifndef HEAP_H
#define HEAP_H

#include "../BF/BF.h"
#define RECORD_SIZE 94

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

typedef struct
{
  int fileDesc;
  char attrType;
  char *attrName;
  int attrLength;
} HP_info;
typedef struct {
  char typeOfFile[2];
  char *filename;
  char attrType;
  char *attrName;
  int attrLength;
} firstBlockInfo;

typedef struct {
  Record records[BLOCK_SIZE / RECORD_SIZE];
  int numOfRecords;
  Block *nextBlock;
} Block;

#endif