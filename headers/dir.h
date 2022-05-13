#ifndef DIR_H
#define DIR_H

#include "fileRepository.h"
#include "config.h"

int checkIfDirectoriesContainEachOther(char *namea, char *nameb);
int checkIfDirectoryExists(const char *name);
fileList *getFilesFromDirectory(char *path, int recusive);
int checkIfFileExists(char *name);
int checkIfFileIsDirectory(char *name);
void createFile(char *path);
void copyFile(char *source, char *destination);
#endif