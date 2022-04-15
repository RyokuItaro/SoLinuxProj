#include <headers/fileRepository.h>
#include <headers/config.h>
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
        list = list->next; //jak ja nienawidze list w struct :>
    }
    list->next = malloc(sizeof(fileList)); //jak koniec listy to dodajemy do listy plik - przgotowujemy pamiec
    list = list->next; //lecymy
    list->name = malloc(strllen(name)+1);
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
    fileList *list = ;
    fileList *reverse = ;
    fileList *first = ;
}