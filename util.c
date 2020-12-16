#include "HP/heap.h"
#include "util.h"

#include <stdio.h>

int getNumEntries(void *block)
{
    void *blockPointer = block;
    int *numEntriesP = blockPointer + 508;
    return *numEntriesP;
}
void *getNextBlockP(void *block)
{
    void *blockPointer = block;
    void *NextBlockP = blockPointer + 500;
    return NextBlockP;
}
void *jumpToNextEntry(void *block)
{
    void *blockPointer = block;
    void *NextRecordP = blockPointer + RECORD_SIZE;
    return NextRecordP;
}
int increaseNumEntries(void *block)
{
    void *blockPointer = block;
    int *numEntriesP = blockPointer + 508;
    *(numEntriesP)++;
    return *numEntriesP;
}

void initBlock(void *block)
{
    void *blockPointer = block;
    void *NextBlockP = blockPointer + 500;
    int *numEntriesP = blockPointer + 508;
    *((char *)NextBlockP) = NULL;
    *numEntriesP = 0;
}

int idIsUnique(void *firstBlock, int id)
{
    void *currentBlock = firstBlock;
    while (currentBlock != NULL)
    {
        int numEntries = getNumEntries(currentBlock);
        Record *currentEntry = (Record *)currentBlock;
        for (int i = 0; i < numEntries; i++)
        {
            if (currentEntry->id == id)
            {
                return 0;
            }
            currentEntry = jumpToNextEntry(currentBlock);
        }
        currentBlock = getNextBlockP(currentBlock);
    }
    return 1;
}

void *findEntryWithId(void *firstBlock, int id)
{
    void *currentBlock = firstBlock;
    while (currentBlock != NULL)
    {
        int numEntries = getNumEntries(currentBlock);
        Record *currentEntry = (Record *)currentBlock;
        for (int i = 0; i < numEntries; i++)
        {
            if (currentEntry->id == id)
            {
                return currentEntry;
            }
            currentEntry = jumpToNextEntry(currentBlock);
        }
        currentBlock = getNextBlockP(currentBlock);
    }
    return NULL;
}
int findBlockNumber(void *firstBlock, int id)
{
    void *currentBlock = firstBlock;
    int blockCounter = 0;
    while (currentBlock != NULL)
    {
        int numEntries = getNumEntries(currentBlock);
        Record *currentEntry = (Record *)currentBlock;
        for (int i = 0; i < numEntries; i++)
        {
            if (currentEntry->id == id)
            {
                return blockCounter;
            }
            currentEntry = jumpToNextEntry(currentBlock);
        }
        currentBlock = getNextBlockP(currentBlock);
        blockCounter++;
    }
    return -1;
}

int printEntry(Record *entry)
{
    printf("Id: %d \n", entry->id);
    printf("Name: %s \n", entry->name);
    printf("Surname: %s \n", entry->surname);
    printf("Address: %s \n\n", entry->address);
}

int findEntriesToPrint(void *firstBlock, int id)
{
    void *currentBlock = firstBlock;
    int blockCounter = 0;
    while (currentBlock != NULL)
    {
        int numEntries = getNumEntries(currentBlock);
        Record *currentEntry = (Record *)currentBlock;
        for (int i = 0; i < numEntries; i++)
        {
            if (id == NULL)
            {
                printEntry(currentEntry);
            }
            else if (currentEntry->id == id)
            {
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