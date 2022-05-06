#include "headers/fileRepository.h"
#include "headers/dir.h"
#include <utime.h>
#include "headers/config.h"
#include <string.h>
#include <sys/stat.h>

fileList *createList (){
    fileList *list = malloc(sizeof(fileList));
    list->next = NULL;
    list->name = NULL;
    list->path = NULL;
    return list;
}

fileType getFileType(char *path){
    struct stat dirstat;
    stat(path, &dirstat);
    if(S_ISREG(dirstat.st_mode)) return regularFile;
    else if(S_ISDIR(dirstat.st_mode)) return directory;
    else return unidentified;
}

fileList *addToList(fileList *list, char *name, char *path, fileType type){
    while(list->next != NULL){
        list = list->next; //postawy programowania flashbacks
    }
    list->next = malloc(sizeof(fileList)); //jak koniec listy to dodajemy do listy plik - przgotowujemy pamiec
    list = list->next; //lecymy
    list->name = malloc(strlen(name)+1);
    list->path = malloc(strlen(path)+1);
    strcpy(list->name, name);
    strcpy(list->path, path);
    list->next = NULL;
    list->type = type;
    return list; //done
}

void *emptyList(fileList *first){
    if(first == NULL) {
        return 0;
    }
    while(first->next != NULL){ //usuwaj poki lista nie bedzie pusta
        fileList *prev = first;
        first = first->next;
        free(prev->name);
        free(prev->path);
        free(prev);
    }
    free(first);
}

fileList *reverseList(fileList *list){
    fileList *first = NULL;
    fileList *next = NULL;
    while(list->next != NULL){
        list = list->next;
        next = first;
        first = malloc(sizeof(fileList));
        first->next = next;
        first->name = malloc(strlen(list->name) + 1);
        first->path = malloc(strlen(list->path) + 1);
        strcpy(first->name, list->name);
        strcpy(first->path, list->path);
    }
    next = first;
    first = malloc(sizeof(fileList));
    first->next = next;
    first->name = NULL;
    first->path = NULL;
    return first;
}

fileList *mergeList(fileList *list, fileList *next){
    while(list->next != NULL)
        list = list->next;

    list->next = next->next;
    free(next->name);
    free(next->path);
    free(next);
    return list;
}

fileList *deleteIfNotInSource(config conf){
    fileList *list = getFilesFromDirectory(conf.destinationDir, conf.recursive);
    fileList *reverse = reverseList(list);
    fileList *first = reverse;
    emptyList(list);

    while(reverse->next != NULL){
        reverse = reverse->next;

        int sourcePathLength = strlen(reverse->path) + strlen(reverse->path) - strlen(conf.destinationDir) + 2 + strlen(conf.sourceDir);
        int targetPathLength = strlen(reverse->path) + strlen(reverse->name) + 3;
        char targetPath[targetPathLength];
        char sourcePath[sourcePathLength];
        snprintf(targetPath, targetPathLength, "%s/%s", reverse->path, reverse->name);
        snprintf(sourcePath, sourcePathLength, "%s%s/%s", conf.sourceDir, reverse->path + strlen(conf.destinationDir), reverse->name);
        if(!checkIfFileExists(sourcePath)){
            if(reverse->type == directory){
                rmdir(targetPath);
            }
            else{
                remove(targetPath);
            }
        }
        emptyList(first);
    }
}

void injectTimestamps(char *source, char *dest){
    struct stat tstmp;
    struct utimbuf newTstmp;
    stat(source, &tstmp);
    newTstmp.actime = tstmp.st_atim.tv_sec;
    newTstmp.modtime = tstmp.st_mtim.tv_sec;
    utime(dest, &newTstmp);
    chmod(dest, tstmp.st_mode);
}

int compareTimestamps(char *source, char *dest){
    struct stat tstmp1;
    struct stat tstmp2;
    stat(source, &tstmp1);
    stat(dest, &tstmp2);
    return (tstmp1.st_mtim.tv_sec == tstmp2.st_mtim.tv_sec);
}