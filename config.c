#include "headers/config.h"
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>

config defaultConfig(){
    config DEFAULT_CONFIG = {300, 0, "", "" };
    return DEFAULT_CONFIG;
}

void showAvailableParams(){
    printf("Demon pobiera 4 argumenty \n");
    printf("\"-s [sekundy]\" - ustawia czas oczekiwania demona na kolejna synchronizacje katalogow\n");
    printf("\"-r\" - wlacza rekurswyne kopiowanie katalogow\n");
    printf("\"[sourceDir]\" - wkazuje na folder z ktorego maja byc kopiowane pliki\n");
    printf("\"[destinationDir]\" - wkazuje na folder do ktorego maja byc kopiowane pliki\n");
}

config parseParams(int argc, char *argv[]){
    config conf = defaultConfig();

    for(int i = 1; i < argc; i++){
        char *arg = argv[i];
        if(arg[0] == '-'){
            char param = arg[1];
            if(strlen(arg) != 2){
                syslog(LOG_CRIT, "Invalid parameters");
                exit(EXIT_FAILURE);
            }

            switch (param) {
            case 's':
                conf.syncingBreak = argv[i+1];
                i++;
                break;

            case 'r':
                conf.recursive = 1;
                break;

            case '?':
            case 'h':
                showAvailableParams();
                exit(EXIT_SUCCESS);
                break;

            default:
                syslog(LOG_CRIT, "Unknown parameter \"%c\"", param);
                exit(EXIT_FAILURE);
            }
        }
        else {
            if(arg[strlen(arg)-1] == '/') //jesli ktos da / na koncu scieżki to trzeba usunąć
                arg[strlen(arg)-1] = '\0';
            conf.sourceDir = arg;
            conf.destinationDir = arg;
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
}