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
    BF_PrintError("Error Creating File");
    return -1;
  }
  if (bfs = BF_OpenFile(fileName) < 0) {
    BF_PrintError("Error opening file");
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
  firstBlockContent->info = createHPInfo(attrType, attrName, attrLength);
  strcpy(firstBlockContent->typeOfFile, "HP");
  memcpy(firstBlock, firstBlockContent, sizeof(firstBlockInfo));
  if (BF_WriteBlock(bfs, 0) < 0) {
    BF_PrintError("Error writing first block back");
    return -1;
  }
  return 0;
}

HP_info *HP_OpenFile(char *fileName) {
}

int HP_CloseFile(HP_info *header_info) {
}

int HP_InsertEntry(HP_info header_info, Record record) {
}

int HP_DeleteEntry(HP_info header_info, void *value) {
}

int HP_GetAllEntries(HP_info header_info, void *value) {
}

HP_info *createHPInfo(char attrType, char *attrName, int attrLength) {
  HP_info *hpInfo = malloc(sizeof(hpInfo));
  hpInfo->attrType = attrType;
  hpInfo->attrName = attrName;
  hpInfo->attrLength = attrLength;
  return hpInfo;
}