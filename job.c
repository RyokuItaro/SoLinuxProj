#include "headers/job.h"
#include "headers/config.h"
#include "headers/fileRepository.h"
#include "headers/dir.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <limits.h>

void doJob(config conf){
    syslog(LOG_INFO, "doJob - In");
    
    fileList *list = getFilesFromDirectory(conf.sourceDir, conf.recursive);
    fileList *first = list;

    while(list->next != NULL){
        syslog(LOG_INFO, "while - IN");
        list = list->next;

        int fullSourcePathLength = strlen(list->path) + strlen(list->name) + 2;
        int fullDestPathLength = strlen(list->path) + strlen(list->name) - strlen(conf.sourceDir) + strlen(conf.destinationDir) + 2;

        char fullSourcePath[fullSourcePathLength];
        char fullDestPath[fullDestPathLength];
        snprintf(fullSourcePath, fullSourcePathLength,"%s/%s", list->path, list->name);
        snprintf(fullDestPath, fullDestPathLength, "%s%s/%s", conf.destinationDir, list->path + strlen(conf.sourceDir), list->name);

        if(checkIfFileExists(fullDestPath)){
            if(list->type == directory){
                if(!compareTimestamps(fullSourcePath, fullDestPath)){
                    injectTimestamps(fullSourcePath, fullDestPath);
                }
            }
            else if(list->type == regularFile){
                if(!compareTimestamps(fullSourcePath, fullDestPath)){
                    removeFile(fullDestPath);
                    copyFile(fullSourcePath, fullDestPath);
                    injectTimestamps(fullSourcePath, fullDestPath);
                }
            }
        }
        else{
            if(list->type == directory){
                mkdir(fullDestPath, 0700);
                injectTimestamps(fullSourcePath, fullDestPath);
            }
            else{
                copyFile(fullSourcePath, fullDestPath);
                injectTimestamps(fullSourcePath, fullDestPath);
            }
        }
    }
    syslog(LOG_INFO, "doJob - Out");
    emptyList(first);
}