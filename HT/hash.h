#ifndef HASH_H
#define HASH_H

#include "BF/BF.h"

typedef struct
{
    int fileDesc;
    char attrType;
    char *attrName;
    int attrLength;
    long int numBuckets;
} HT_info;
#endif