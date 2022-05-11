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

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
}

void signalKillDaemon(int signum){
    syslog(LOG_INFO, "Daemon process killed");
    killDaemon = 1;
}

void signalForceDeamonJob(int signum){
    syslog(LOG_INFO, "Wymuszenie synchronizacji");
}

void setCustomSignals(){
    signal(SIGUSR1, signalForceDeamonJob);
    signal(SIGINT, signalKillDaemon);
}

int main(int argc, char *argv[]) {
        openlog("syncingDaemonLog", LOG_PID | LOG_CONS, LOG_USER);
        forkProcess();
        setCustomSignals();
        config conf = parseParams(argc, argv);
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
                syslog(LOG_INFO, "main - Mirroring directory - In");
                doJob(conf);
                syslog(LOG_INFO, "main - Mirroring directory - Out");        
                sleep(conf.syncingBreak);
        }

        closelog();
        return 0;
}