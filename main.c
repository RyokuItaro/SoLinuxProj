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
#include "headers/config.h"

volatile short int killDaemon = 0;

void forkProcess(){
        syslog(LOG_INFO, "forkProcess - IN");     
        pid_t pid, sid;
        
        pid = fork();
        if (pid < 0) {
                syslog(LOG_CRIT, "forkProcess - Could not fork process");
                exit(EXIT_FAILURE);
        }
        
        if (pid > 0) {
                exit(EXIT_SUCCESS);
        }

        umask(0);

        syslog(LOG_INFO, "forkProcess - Setting SID for child");               
        sid = setsid();
        if (sid < 0) {
                syslog(LOG_CRIT, "forkProcess - Could not set SID");
                exit(EXIT_FAILURE);
        }
        
        if ((chdir("/")) < 0) {
                syslog(LOG_CRIT, "forkProcess - Could not change working directory");
                exit(EXIT_FAILURE);
        }

        syslog(LOG_INFO, "forkProcess - OUT");
}

void signalKillDaemon(){
    syslog(LOG_INFO, "Daemon process killed");
    exit(EXIT_SUCCESS);
}

void signalForceDeamonJob(){
    syslog(LOG_INFO, "Wymuszenie w trakcie synchronizacji, kontynuuję");
}

void setCustomSignals(){
    signal(SIGUSR1, signalForceDeamonJob);
    signal(SIGTERM, signalKillDaemon);
}

int main(int argc, char *argv[]) {
        openlog("syncingDaemonLog", LOG_PID | LOG_CONS, LOG_USER);
        forkProcess();
        setCustomSignals();
        config conf = parseParams(argc, argv);
        int sec = 0;
        //while(1){
        //        syslog(LOG_INFO, "main - Mirroring directory - In");
        //        syslog(LOG_INFO, "main - Mirroring directory - Out");        
        //        sleep(1);
        //}       
}