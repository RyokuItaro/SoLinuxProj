#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

volatile bool killDaemon = false;

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

        syslog(LOG_INFO, "forkProcess - Setting SID for child")                
        sid = setsid();
        if (sid < 0) {
                syslog(LOG_CRIT, "forkProcess - Could not set SID");
                exit(EXIT_FAILURE);
        }
        
        if ((chdir("/")) < 0) {
                syslog(LOG_CRIT, "forkProcess - Could not change working directory");
                exit(EXIT_FAILURE);
        }
        
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        syslog(LOG_INFO, "forkProcess - OUT");
}

int main(void) {
        forkProcess();
        int sec = 0;
        while(true){
                syslog(LOG_INFO, "main - Mirroring directory - In");
                printf("Working s: %d", sec++);
                syslog(LOG_INFO, "main - Mirroring directory - Out");        
                sleep(1);
        }       
}