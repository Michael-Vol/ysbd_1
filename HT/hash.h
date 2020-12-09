typedef struct
{
    int fileDesc;
    char attrType;
    char *attrName;
    int attrLength;
    long int numBuckets;
} HT_info;