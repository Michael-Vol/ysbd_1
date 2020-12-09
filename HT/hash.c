#include "hash.h"

int HT_CreateIndex(char *fileName, char attrType, char *attrName, int attrLength, int buckets)
{
}

HT_info *HT_OpenIndex(char *fileName)
{
}

int HT_CloseIndex(HT_info *header_info)
{
}

int HT_InsertEntry(HT_info header_info, Record record)
{
}

int HT_DeleteEntry(HT_info header_info, void *value)
{
}

int HT_GetAllEntries(HT_info header_info, void *value)
{
}

int HashStatistics(char *filename)
{
}