#ifndef HEAP_H
#define HEAP_H

#include "../BF/BF.h"
#include "util.h"

int HP_CreateFile(char *fileName, char attrType, char *attrName, int attrLength);
HP_info *HP_OpenFile(char *fileName);
int HP_CloseFile(HP_info *header_info);
int HP_InsertEntry(HP_info header_info, Record record);
int HP_DeleteEntry(HP_info header_info, void *value);
int HP_GetAllEntries(HP_info header_info, void *value);

typedef struct
{
	int fileDesc;
	char attrType;
	char *attrName;
	int attrLength;
} HP_info;

typedef struct
{
	char typeOfFile[2];
	char *filename;
	char attrType;
	char *attrName;
	int attrLength;
} firstBlockInfo;

typedef struct // το κανουμε κατι αυτοο?????
{
	Record records[BLOCK_SIZE / RECORD_SIZE];
	int numOfRecords;
	Block *nextBlock;
} Block;

#endif