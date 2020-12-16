#ifndef HEAP_H
#define HEAP_H

#include "../BF/BF.h"
#include "../util.h"

typedef struct
{
  int fileDesc;
  char attrType;
  char attrName[20];
  int attrLength;
  char fileName[20];
} HP_info;

int HP_CreateFile(char *fileName, char attrType, char *attrName, int attrLength);
HP_info *HP_OpenFile(char *fileName);
int HP_CloseFile(HP_info *header_info);
int HP_InsertEntry(HP_info header_info, Record record);
int HP_DeleteEntry(HP_info header_info, void *value);
int HP_GetAllEntries(HP_info header_info, void *value);

typedef struct
{
  char typeOfFile[2];
  char filename[20];
  char attrType;
  char attrName[20];
  int attrLength;
} firstBlockInfo;

#endif