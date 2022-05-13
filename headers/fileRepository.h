#ifndef FILEREPOSITORY_H
#define FILEREPOSITORY_H

#include "config.h"

typedef struct fileList fileList;

typedef enum {
    regularFile,
    directory,
    unidentified
} fileType;

struct fileList{
    char *name;
    char *path;
    fileList *next;
    fileType type;
};

fileList *createList ();
fileType getFileType(char *path);
fileList *addToList(fileList *list, char *name, char *path, fileType type);
void *emptyList(fileList *first);
fileList *reverseList(fileList *list);
fileList *mergeList(fileList *list, fileList *next);
void deleteIfNotInSource(config conf);
void injectTimestamps(char *source, char *dest);
int compareTimestamps(char *source, char *dest);
#endif