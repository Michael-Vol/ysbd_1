#include "heap.h"
#include "../BF/BF.h"
#include "../record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int HP_CreateFile(char *fileName, char attrType, char *attrName, int attrLength, int buckets) {
  int bfs;
  void *firstBlock;
  BF_Init();
  if (BF_CreateFile(fileName) < 0) {
    BF_PrintError("Error Creating heap file");
    return -1;
  }
  if (bfs = BF_OpenFile(fileName) < 0) {
    BF_PrintError("Error opening heap file");
    return -1;
  }
  if (BF_AllocateBlock(bfs) < 0) {
    BF_PrintError("Error Allocating First Block");
    return -1;
  }
  if (BF_ReadBlock(bfs, 0, &firstBlock) < 0) {
    BF_PrintError("Error getting First Block");
    return -1;
  }
  firstBlockInfo *firstBlockContent = malloc(sizeof(firstBlockInfo));
  strcpy(firstBlockContent->typeOfFile, "HP");
  strcpy(firstBlockContent->filename, fileName);
  strcpy(firstBlockContent->attrName, attrName);
  firstBlockContent->attrType = attrType;
  firstBlockContent->attrLength = attrLength;
  memcpy(firstBlock, firstBlockContent, sizeof(*firstBlockContent));
  if (BF_WriteBlock(bfs, 0) < 0) {
    BF_PrintError("Error writing to first block");
    return -1;
  }
  return 0;
}

HP_info *HP_OpenFile(char *fileName) {
  int bfs;
  firstBlockInfo *firstBlock;
  if (bfs = BF_OpenFile(fileName) < 0) {
    BF_PrintError("Error opening heap file");
    return NULL;
  }
  if (BF_ReadBlock(bfs, 0, &firstBlock) < 0) {
    BF_PrintError("Error opening heap file");
    return NULL;
  }
  HP_info *hpinfo = malloc(sizeof(HP_info));
  hpinfo->fileDesc = bfs;
  hpinfo->attrType = firstBlock->attrType;
  hpinfo->attrLength = firstBlock->attrLength;
  strcpy(hpinfo->attrName, firstBlock->attrName);

  return hpinfo;
}

int HP_CloseFile(HP_info *header_info) {
  if (BF_CloseFile(header_info->fileDesc) < 0) {
    BF_PrintError("Error closing heap file");
    return -1;
  };
  free(header_info);
  return 0;
}

int HP_InsertEntry(HP_info header_info, Record record) {
  int bfs;
  void *block;
  int fileDesc = header_info.fileDesc;
  if (bfs = BF_OpenFile(fileDesc) < 0) {
    BF_PrintError("Error opening heap file");
    return -1;
  }
  if (BF_ReadBlock(fileDesc, 1, &block) < 0) {
    BF_PrintError("Error reading heap block");
    return -1;
  }
  if (!idIsUnique(block, record.id)) {
    return -1;
  }

  int numBlocks = BF_GetBlockCounter(bfs);
  if (numBlocks == 1) {
    if (BF_AllocateBlock(fileDesc) < 0) {
      BF_PrintError("Error getting block counter");
      return -1;
    }
    initBlock(block);
    memcpy(block, &record, sizeof(record));
    increaseNumEntries(block);
    if (BF_WriteBlock(bfs, 1) < 0) {
      BF_PrintError("Error writing to block");
      return -1;
    }
    return numBlocks;
  } else {
    if (BF_ReadBlock(fileDesc, numBlocks - 1, block) < 0) {
      BF_PrintError("Error reading heap block");
      return -1;
    }
    int numEntries = getNumEntries(block);
    if (numEntries < BLOCK_SIZE / RECORD_SIZE) {
      void *newRecordP = (char *)block + (numEntries * RECORD_SIZE) + 1;
      memcpy(newRecordP, &record, sizeof(record));
      increaseNumEntries(block);

      if (BF_WriteBlock(bfs, numBlocks - 1) < 0) {
        BF_PrintError("Error writing to block");
        return -1;
      }
      return numBlocks - 1;
    } else {
      if (BF_AllocateBlock(fileDesc) < 0) {
        BF_PrintError("Error getting block counter");
        return -1;
      }
      if (BF_ReadBlock(fileDesc, numBlocks, &block) < 0) {
        BF_PrintError("Error reading heap block");
        return -1;
      }
      initBlock(block);
      memcpy(block, &record, sizeof(record));
      increaseNumEntries(block);
      if (BF_WriteBlock(bfs, numBlocks) < 0) {
        BF_PrintError("Error writing to block");
        return -1;
      }
      void *newBlockP = block;
      if (BF_ReadBlock(fileDesc, numBlocks - 1, &block) < 0) {
        BF_PrintError("Error reading heap block");
        return -1;
      }
      void *prevBlockP = getNextBlockP(block);
      prevBlockP = newBlockP;
      if (BF_WriteBlock(bfs, numBlocks - 1) < 0) {
        BF_PrintError("Error writing to block");
        return -1;
      }
      return numBlocks;
    }
  }
}

int HP_DeleteEntry(HP_info header_info, void *value) {
  int bfs;
  void *block;
  int fileDesc = header_info.fileDesc;
  if (bfs = BF_OpenFile(fileDesc) < 0) {
    BF_PrintError("Error opening heap file");
    return -1;
  }
  if (BF_ReadBlock(fileDesc, 1, &block) < 0) {
    BF_PrintError("Error reading heap block");
    return -1;
  }
  Record *entry = (Record *)findEntryWithId(block, *((int *)(value)));
  int currentBlockIndex = findBlockNumber(block, *((int *)(value)));
  if (entry != NULL) {
    memset(entry, 0, RECORD_SIZE);
    if (BF_WriteBlock(bfs, currentBlockIndex) < 0) {
      BF_PrintError("Error writing to block");
      return -1;
    }
    return 0;
  }
  return -1;
}

int HP_GetAllEntries(HP_info header_info, void *value) {
  int bfs;
  void *block;
  int fileDesc = header_info.fileDesc;
  if (bfs = BF_OpenFile(fileDesc) < 0) {
    BF_PrintError("Error opening heap file");
    return -1;
  }
  if (BF_ReadBlock(fileDesc, 1, &block) < 0) {
    BF_PrintError("Error reading heap block");
    return -1;
  }
  int idBlockNumber = findEntriesToPrint(block, *((int *)(value)));
  if (idBlockNumber == 0) {
    return BF_GetBlockCounter(fileDesc) - 1;
  }
  return idBlockNumber;
}

int getNumEntries(void *block) {
  void *blockPointer = block;
  int *numEntriesP = blockPointer + 508;
  return *numEntriesP;
}
void *getNextBlockP(void *block) {
  void *blockPointer = block;
  void *NextBlockP = blockPointer + 500;
  return NextBlockP;
}
void *jumpToNextEntry(void *block) {
  void *blockPointer = block;
  void *NextRecordP = blockPointer + RECORD_SIZE;
  return NextRecordP;
}
int increaseNumEntries(void *block) {
  void *blockPointer = block;
  int *numEntriesP = blockPointer + 508;
  *(numEntriesP)++;
  return *numEntriesP;
}

void initBlock(void *block) {
  void *blockPointer = block;
  void *NextBlockP = blockPointer + 500;
  int *numEntriesP = blockPointer + 508;
  *((char *)NextBlockP) = NULL;
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
}

int findEntriesToPrint(void *firstBlock, int id) {
  void *currentBlock = firstBlock;
  int blockCounter = 0;
  while (currentBlock != NULL) {
    int numEntries = getNumEntries(currentBlock);
    Record *currentEntry = (Record *)currentBlock;
    for (int i = 0; i < numEntries; i++) {
      if (id == NULL) {
        printEntry(currentEntry);
      } else if (currentEntry->id == id) {
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