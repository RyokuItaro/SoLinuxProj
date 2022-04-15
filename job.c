#include <headers/job.h>
#include <headers/config.h>
#include <headers/fileRepository.h>
#include <headers/dir.h>
#include <string.h>
#include <stdlib.h>

void doJob(config conf){
    fileList *list = getFilesFromDirectory(conf.sourceDir, conf.recursive);
    fileList *first = list;

    while(list->next != NULL){
        list = list->next;

        int fullSourcePathLength = strlen(list->path) + strl(list->name) + 2;
        int fullDestPathLength = strlen(list->path) + strlen(list->name) - strlen(conf.sourceDir) + strlen(conf.destinationDir) + 2;

        char fullSourcePath[fullSourcePathLength];
        char fullDestPath[fullDestPathLength];

        snprintf(fullSourcePath, fullSourcePathLength,"%s/%s", list->path, list->name);
        snprintf(fullDestPath, fullDestPathLength, "%s%s/%s", conf.destinationDir, list->path + strlen(conf.sourceDir), list->name);
    }
}