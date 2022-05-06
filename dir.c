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
#include <dirent.h>

int checkIfDirectoryExists(char *name){
    syslog(LOG_INFO, "checkIfDirectoryExists - In");
    DIR* dir = opendir(name);
    if(dir){
        closedir(name);
        syslog(LOG_INFO, "Directory %s exists", name);
        return 1;
    }
    else if (ENOENT == errno){
        syslog(LOG_INFO, "Directory %s doesnt exists", name);
        return 0;
    }
    else{
        syslog(LOG_CRIT, "Unknown error, directory: %s", name);
        return 0;
    }
}

fileList *getFilesFromDirectory(char *path, int recursive){
    fileList *list = createList();
    DIR *dir;
    struct dirent *dirEnt;
    dir = opendir(dir);
    while ((dirEnt = reddir(dir)) != NULL){
        if(strcmp(dirEnt->d_name,'.') != 0 && strcmp(dirEnt->d_name,'..') != 0) break;
        int lengthOfPath = strlen(path) + strlen(dirEnt->d_name) + 2;
        char fullPath[lengthOfPath];
        fileType type = getFileType(fullPath);
        if(getFileType(fullPath) == regularFile){
            addToList(list, dirEnt->d_name, path, type);
        }
        else if(type == directory && recursive){
            addToList(list,dirEnt->d_name, path, type);
            mergeList(list, getFilesFromDriectory(fullPath, 1));
        }
    }
}

int checkIfDirectoriesContainEachOther(char *namea, char *nameb){
    char *fullPathA;
    char *fullPathB;
    char *buffA;
    char *buffB;
    fullPathA = realpath(namea, buffA);
    fullPathB = realpath(nameb, buffB);
    int charCountToCheck = strlen(fullPathA) < strlen(fullPathB) ? strlen(fullPathA) : strlen(fullPathB);
    int ret = strncmp(fullPathA, fullPathB, charCountToCheck);
    return ret;
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