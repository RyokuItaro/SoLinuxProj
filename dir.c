#include <dirent.h>
#include <errno.h>
#include <syslog.h>
#include <headers/dir.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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