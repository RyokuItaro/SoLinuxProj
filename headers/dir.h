#ifndef DIR_H
#define DIR_H

#include "fileRepository.h"

int checkIfDirectoryExists(char *name);
int checkIfDirectoriesContainEachOther(char *namea, char *nameb);
int checkIfDirectoryExists(char *name);
fileList *getFilesFromDirectory(char *path, int recusive);
int checkIfDirectoriesContainEachOther(char *namea, char *nameb);
int checkIfFileExists(char *name);
int checkIfFileIsDirectory(char *name);
void createFile(char *path);
void copyFile(char *source, char *destination);
void removeFile(char *path);
#endif