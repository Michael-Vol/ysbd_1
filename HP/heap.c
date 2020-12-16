#include "heap.h"
#include "../BF/BF.h"
#include "../util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int HP_CreateFile(char *fileName, char attrType, char *attrName, int attrLength)
{
	int bfs;
	void *firstBlock;
	BF_Init();
	if (BF_CreateFile(fileName) < 0)
	{
		BF_PrintError("Error Creating heap file");
		return -1;
	}
	if ((bfs = BF_OpenFile(fileName)) < 0)
	{
		BF_PrintError("Error opening heap file");
		return -1;
	}
	if (BF_AllocateBlock(bfs) < 0)
	{
		BF_PrintError("Error Allocating First Block");
		return -1;
	}
	if (BF_ReadBlock(bfs, 0, &firstBlock) < 0)
	{
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
	if (BF_WriteBlock(bfs, 0) < 0)
	{
		BF_PrintError("Error writing to first block");
		return -1;
	}
	if (BF_CloseFile(bfs) < 0)
	{
		BF_PrintError("Error closing heap file");
		return -1;
	}
	return 0;
}

HP_info *HP_OpenFile(char *fileName)
{
	int bfs;
	firstBlockInfo *firstBlock;
	if ((bfs = BF_OpenFile(fileName)) < 0)
	{
		BF_PrintError("Error opening heap file");
		return NULL;
	}
	printf("HP_OpenFile   bfs=%d\n", bfs);
	void *block;
	if (BF_ReadBlock(bfs, 0, &block) < 0)
	{
		BF_PrintError("Error opening heap file");
		return NULL;
	}
	firstBlock = block;
	HP_info *hpinfo = malloc(sizeof(HP_info));
	hpinfo->fileDesc = bfs;
	hpinfo->attrType = firstBlock->attrType;
	hpinfo->attrLength = firstBlock->attrLength;
	strcpy(hpinfo->attrName, firstBlock->attrName);
	strcpy(hpinfo->fileName, firstBlock->filename);

	return hpinfo;
}

int HP_CloseFile(HP_info *header_info)
{
	if (BF_CloseFile(header_info->fileDesc) < 0)
	{
		BF_PrintError("Error closing heap file");
		return -1;
	}
	free(header_info);
	return 0;
}

int HP_InsertEntry(HP_info header_info, Record record)
{
	void *block;
	void *firstBlock;
	int fileDesc = header_info.fileDesc;
	char *fileName = header_info.fileName;

	int numBlocks = BF_GetBlockCounter(fileDesc);
	printf("\nSTART\nfilename = %s, NumBlocks: %d \n", fileName, numBlocks);

	printf("HP_InsertEntry   filedesc=%d\n", fileDesc);
	if (BF_ReadBlock(fileDesc, 0, &firstBlock) < 0) // read the first block
	{
		BF_PrintError("Error reading heap first Block");
		return -1;
	}

	// if (numBlocks > 1) // if there are blocks in the file
	// {				   // check for uniqness

	// 	if (!idIsUnique(firstBlock, record.id))
	// 	{
	// 		printf("UNIQUE!!!\n");
	// 		return -1;
	// 	}
	// }

	if (numBlocks == 1) // if we only have the information block
	{
		if (BF_AllocateBlock(fileDesc) < 0) // allocate a new block
		{
			BF_PrintError("Error getting block counter");
			return -1;
		}

		if (BF_ReadBlock(fileDesc, 1, &block) < 0) // read the second(new) block
		{
			BF_PrintError("Error reading heap new block");
			return -1;
		}

		initBlock(block);
		// *((Record *)block) = record;
		memcpy(block, &record, sizeof(record));
		increaseNumEntries(block);
		printEntry(&block);

		setBlockP(firstBlock, &block);

		if (BF_WriteBlock(fileDesc, 1) < 0)
		{
			BF_PrintError("Error writing to block");
			return -1;
		}
		return numBlocks;
	}
	else
	{
		if (BF_ReadBlock(fileDesc, numBlocks - 1, &block) < 0)
		{
			BF_PrintError("Error reading heap last block");
			return -1;
		}

		int numEntries = getNumEntries(block);
		printf("numEntries =%d\n", numEntries);
		if (numEntries < BLOCK_SIZE / RECORD_SIZE)
		{
			void *newRecordP = (void *)block + (numEntries * RECORD_SIZE);
			*((Record *)newRecordP) = record;
			printEntry(&record);
			// memcpy(newRecordP, &record, sizeof(record));
			increaseNumEntries(block);
			if (BF_WriteBlock(fileDesc, numBlocks - 1) < 0)
			{
				BF_PrintError("Error writing to block");
				return -1;
			}
			return numBlocks - 1;
		}
		else
		{
			if (BF_AllocateBlock(fileDesc) < 0)
			{
				BF_PrintError("Error getting block counter");
				return -1;
			}
			if (BF_ReadBlock(fileDesc, numBlocks, &block) < 0)
			{
				BF_PrintError("Error reading heap block");
				return -1;
			}
			initBlock(block);
			memcpy(block, &record, sizeof(record));
			increaseNumEntries(block);
			if (BF_WriteBlock(fileDesc, numBlocks) < 0)
			{
				BF_PrintError("Error writing to block");
				return -1;
			}
			void *newBlockP = block;
			if (BF_ReadBlock(fileDesc, numBlocks - 1, &block) < 0)
			{
				BF_PrintError("Error reading heap block");
				return -1;
			}
			void *prevBlockP = getNextBlockP(block);
			memcpy(prevBlockP, &newBlockP, sizeof(newBlockP));
			if (BF_WriteBlock(fileDesc, numBlocks - 1) < 0)
			{
				BF_PrintError("Error writing to block");
				return -1;
			}
			return numBlocks;
		}
	}
}

int HP_DeleteEntry(HP_info header_info, void *value)
{
	int bfs;
	void *block;
	int fileDesc = header_info.fileDesc;
	char *fileName = header_info.fileName;
	if ((bfs = BF_OpenFile(fileName)) < 0)
	{
		BF_PrintError("Error opening heap file");
		return -1;
	}
	if (BF_ReadBlock(fileDesc, 1, &block) < 0)
	{
		BF_PrintError("Error reading heap block");
		return -1;
	}
	Record *entry = (Record *)findEntryWithId(block, *((int *)(value)));
	int currentBlockIndex = findBlockNumber(block, *((int *)(value)));
	if (entry != NULL)
	{
		memset(entry, 0, RECORD_SIZE);
		if (BF_WriteBlock(bfs, currentBlockIndex) < 0)
		{
			BF_PrintError("Error writing to block");
			return -1;
		}
		return 0;
	}
	return -1;
}

int HP_GetAllEntries(HP_info header_info, void *value)
{
	int bfs;
	void *block;
	int fileDesc = header_info.fileDesc;
	char *fileName = header_info.fileName;
	if ((bfs = BF_OpenFile(fileName)) < 0)
	{
		BF_PrintError("Error opening heap file");
		return -1;
	}
	if (BF_ReadBlock(fileDesc, 1, &block) < 0)
	{
		BF_PrintError("Error reading heap block");
		return -1;
	}
	int idBlockNumber = findEntriesToPrint(block, value);
	if (idBlockNumber == 0)
	{
		return BF_GetBlockCounter(fileDesc) - 1;
	}
	return idBlockNumber;
}