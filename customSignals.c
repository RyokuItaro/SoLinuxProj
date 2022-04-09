#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void setCustomSignals(){
    signal(SIGUSR1, signalForceDeamonJob);
    signal(SIGTERM, signalKillDaemon);
}
void signalForceDeamonJob(){
    syslog(LOG_INFO, "Wymuszenie w trakcie synchronizacji, kontynuuję");
}
void signalKillDaemon(){
    syslog(LOG_INFO, "Daemon process killed");
    exit(EXIT_SUCCESS);
}