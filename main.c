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
#include "headers/config.h"
#include "headers/dir.h"
#include "headers/job.h"

volatile short int killDaemon = 0;
volatile short int syncInProgress = 0;
volatile config conf;

void forkProcess(){
        pid_t pid, sid;
        
        pid = fork();
        if (pid < 0) {
                syslog(LOG_CRIT, "Could not fork process");
                exit(EXIT_FAILURE);
        }
        
        if (pid > 0) {
                exit(EXIT_SUCCESS);
        }

        umask(0);

        syslog(LOG_INFO, "Setting SID for child");               
        sid = setsid();
        if (sid < 0) {
                syslog(LOG_CRIT, "Could not set SID");
                exit(EXIT_FAILURE);
        }
        
        if ((chdir("/")) < 0) {
                syslog(LOG_CRIT, "Could not change working directory");
                exit(EXIT_FAILURE);
        }
}

void signalKillDaemon(int signum){
    syslog(LOG_INFO, "Forced daemon to stop working");
    killDaemon = 1;
}

void signalForceDeamonJob(int signum){
    if(syncInProgress == 1){
        syslog(LOG_INFO, "Daemon is synchronizing directories now!");
    }
    else{
        syslog(LOG_INFO, "Forcing directory sync");
        doJob(conf);
    }
}

void setCustomSignals(){
    signal(SIGUSR1, signalForceDeamonJob);
    signal(SIGINT, signalKillDaemon);
}

int main(int argc, char *argv[]) {
        openlog("syncingDaemonLog", LOG_PID | LOG_CONS, LOG_USER);
        forkProcess();
        setCustomSignals();
        conf = parseParams(argc, argv);
        char sourceBuf[PATH_MAX+1];
        char destinationBuf[PATH_MAX+1];

        if(0 == checkIfDirectoryExists(conf.sourceDir)){
                syslog(LOG_CRIT, "Directory not found - %s", conf.sourceDir);
                exit(EXIT_FAILURE);
        }

        if(0 == checkIfDirectoryExists(conf.destinationDir)){
                syslog(LOG_CRIT, "Directory not found - %s", conf.destinationDir);
                exit(EXIT_FAILURE);
        }

        conf.sourceDir = realpath(conf.sourceDir, sourceBuf);
        conf.destinationDir = realpath(conf.destinationDir, destinationBuf);

        if(checkIfDirectoriesContainEachOther(conf.sourceDir, conf.destinationDir)){
                syslog(LOG_CRIT, "Directory cant include /A/B/ or /B/A/");
                exit(EXIT_FAILURE);
        }

        while(killDaemon == 0){
                syslog(LOG_INFO, "Synchronizing directories");
                syncInProgress = 1;
                doJob(conf);
                syncInProgress = 0;
                syslog(LOG_INFO, "Synchronizing ended properly");        
                sleep(conf.syncingBreak);
        }

        closelog();
        return 0;
}