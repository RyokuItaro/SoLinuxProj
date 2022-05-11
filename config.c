#include "headers/config.h"
#include "headers/converter.h"
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>

config defaultConfig(){
    syslog(LOG_INFO, "defaultConfig - In");
    config DEFAULT_CONFIG = {300, 0, "", ""};
    syslog(LOG_INFO, "defaultConfig - Out");
    return DEFAULT_CONFIG;
}

void showAvailableParams(){
    syslog(LOG_INFO, "showAvailableParams - In");
    printf("\n");
    printf("Demon pobiera 4 argumenty \n");
    printf("\"-s [sekundy]\" - ustawia czas oczekiwania demona na kolejna synchronizacje katalogow\n");
    printf("\"-r\" - wlacza rekurswyne kopiowanie katalogow\n");
    printf("\"[sourceDir]\" - wkazuje na folder z ktorego maja byc kopiowane pliki\n");
    printf("\"[destinationDir]\" - wkazuje na folder do ktorego maja byc kopiowane pliki\n");
    syslog(LOG_INFO, "showAvailableParams - Out");
}

config parseParams(int argc, char *argv[]){
    syslog(LOG_INFO, "parseParams - In");
    config conf = defaultConfig();
    int dirCount = 0;
    
    for(int i = 1; i < argc; i++){
        syslog(LOG_INFO, "parseParams - Get %d parameter", i);
        char *arg = argv[i];
        if(arg[0] == '-'){
            char param = arg[1];
            if(strlen(arg) != 2){
                syslog(LOG_CRIT, "Invalid parameters");
                exit(EXIT_FAILURE);
            }

            switch (param) {
            case 's':
                syslog(LOG_INFO, "parseParams - Received %c", param);
                int ret = parseTime(argv[i+1]);
                conf.syncingBreak = ret;
                i++;
                break;

            case 'r':
                syslog(LOG_INFO, "parseParams - Received %c", param);
                conf.recursive = 1;
                break;

            case '?':
            case 'h':
                syslog(LOG_INFO, "parseParams - Received %c", param);
                showAvailableParams();
                exit(EXIT_SUCCESS);
                break;

            default:
                syslog(LOG_CRIT, "Unknown parameter \"%c\"", param);
                exit(EXIT_FAILURE);
                break;
            }
        }
        else {
            if(arg[strlen(arg)-1] == '/') //jesli ktos da / na koncu scieżki to trzeba usunąć
                arg[strlen(arg)-1] = '\0';
            if(dirCount == 0){
                conf.sourceDir = arg;
            }
            else if(dirCount == 1){
                conf.destinationDir = arg;
            }
            dirCount++;
        }
    }
    if(conf.sourceDir == ""){
        syslog(LOG_CRIT, "Source directory not set");
        exit(EXIT_FAILURE);
    }
    if(conf.destinationDir == ""){
        syslog(LOG_CRIT, "Destination directory not set");
        exit(EXIT_FAILURE);
    }
    return conf;
}