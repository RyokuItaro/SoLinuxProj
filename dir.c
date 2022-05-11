#include <dirent.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include "headers/dir.h"
#include "headers/fileRepository.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define _GNU_SOURCE
#include <stdlib.h>

int checkIfDirectoryExists(const char *name){
    syslog(LOG_INFO, "checkIfDirectoryExists - In");
    DIR* dir = opendir(name);

    if(dir){
        closedir(dir);
        syslog(LOG_INFO, "Directory %s exists", name);
        syslog(LOG_INFO, "checkIfDirectoryExists - Out");
        return 1;
    }
    else{
        perror(name);
        syslog(LOG_INFO, "checkIfDirectoryExists - %s not found", name);
        return 0;
    }
}

fileList* getFilesFromDirectory(char *path, int recursive){
    syslog(LOG_INFO, "getFilesFromDirectory - In");
    fileList *list = createList();
    DIR *dir;
    struct dirent *dirEnt;
    dir = opendir(path);
    syslog(LOG_INFO, "opendir");
    while ((dirEnt = readdir(dir)) != NULL){
        syslog(LOG_INFO, "While - start");
        if(strcmp(dirEnt->d_name, ".") == 0 || strcmp(dirEnt->d_name, "..") == 0){
            syslog(LOG_INFO, "Ignoring %s", dirEnt->d_name);
            continue;
        }
        int lengthOfPath = strlen(path) + strlen(dirEnt->d_name) + 2;
        char fullPath[lengthOfPath];
        snprintf(fullPath, lengthOfPath, "%s/%s", path, dirEnt->d_name);
        fileType type = getFileType(fullPath);
        syslog(LOG_INFO, "%d", type);
        if(type == regularFile){
            syslog(LOG_INFO, "getFilesFromDirectory - regular file found: %s",dirEnt->d_name);
            addToList(list, dirEnt->d_name, path, type);
        }
        else if(type == directory && recursive){
            syslog(LOG_INFO, "getFilesFromDirecotry - directory found");
            addToList(list,dirEnt->d_name, path, type);
            mergeList(list, getFilesFromDirectory(fullPath, 1));
        }
    }
    closedir(dir);
    syslog(LOG_INFO, "getFilesFromDirectory - Out");
    return list;
}

int checkIfDirectoriesContainEachOther(char *namea, char *nameb){
    char *fullPathA;
    char *fullPathB;
    char buffA[PATH_MAX + 1];
    char buffB[PATH_MAX + 1];
    fullPathA = realpath(namea, buffA);
    fullPathB = realpath(nameb, buffB);
    int charCountToCheck = strlen(fullPathA) < strlen(fullPathB) ? strlen(fullPathA) : strlen(fullPathB);
    syslog(LOG_INFO, "%d", charCountToCheck);
    return strncmp(fullPathA, fullPathB, charCountToCheck) ? 0 : 1;
}

int checkIfFileExists(char *name){
    return access(name, F_OK) == 0 ? 1 : 0;
}

int checkIfFileIsDirectory(char *name){
    DIR *dir = opendir(name);
    if(dir){
        closedir(dir);
        return 1;
    }
    else{
        return 0;
    }
}

void createFile(char *path){
    int file = open(path, O_RDWR|O_CREAT, 0777);
    if(file != -1){
        syslog(LOG_INFO, "File created at: %s", path);
        close(file);
    }
}

void copyFile(char *source, char *destination){
    int fileToCopy = open(source, O_RDWR|O_CREAT, 0777);
    createFile(destination);
    int fileCopied = open(destination, O_RDWR|O_CREAT, 0777);
    size_t cursor = 0;
    size_t remainingBytes;
    size_t bytes;
    char buffer[16];
    do{
        bytes = read(fileToCopy, buffer, sizeof(buffer));
        remainingBytes = bytes;
        while(remainingBytes > 0){
            size_t writeCountHandler = write(fileCopied, buffer, bytes);
            if(writeCountHandler == -1) {
                //blad
            }
            remainingBytes -= writeCountHandler;
        }
        cursor += bytes;
    } while(bytes == sizeof(buffer));
    close(fileToCopy);
    close(fileCopied);
}
void removeFile(char *path){

}