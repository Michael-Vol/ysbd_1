#ifndef HEAP_H
#define HEAP_H

#include "../BF/BF.h"

typedef struct
{
  int fileDesc;
  char attrType;
  char *attrName;
  int attrLength;
} HP_info;
typedef struct {
  char typeOfFile[2];
  HP_info *info;
} firstBlockInfo;

HP_info *createHPInfo(char attrType, char *attrName, int attrLength);
#endif
