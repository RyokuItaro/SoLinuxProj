#include <dirent.h>
#include <errno.h>
#include <syslog.h>
#include <headers/dir.h>

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