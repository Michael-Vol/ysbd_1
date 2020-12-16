#include "HP/heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int heapCreated = HP_CreateFile("test", 'i', "id", 10);
  if (heapCreated == -1)
  {
    fprintf(stderr, "Couldn't create Heap\n");
    exit(EXIT_FAILURE);
  }
  HP_info *hpInfo = HP_OpenFile("test");

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen("records/records1K.txt", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1)
  {

    char extract[read];
    sscanf(line, "{%[^}]", extract);
    const char s[2] = ",";
    char *token;

    /* get the first token */
    token = strtok(extract, s);
    int id;
    char name[15], surname[25], address[50];
    id = atoi(token);
    strcpy(name, strtok(NULL, s));
    sscanf(name, "    \"%[^\"]\"  ", name);

    strcpy(surname, strtok(NULL, s));
    sscanf(surname, "    \"%[^\"]\"  ", surname);

    strcpy(address, strtok(NULL, s));
    sscanf(address, "    \"%[^\"]\"  ", address);

    //printf("Read Record: %d  %s %s %s \n", id, name, surname, address);
    Record *record = createRecord(id, name, surname, address);

    HP_InsertEntry(*hpInfo, *record);
  }
  // HP_GetAllEntries(*hpInfo, NULL);
  fclose(fp);
  if (line)
  {
    free(line);
    exit(EXIT_SUCCESS);
  }
  return 0;
}
