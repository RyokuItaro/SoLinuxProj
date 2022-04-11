typedef struct {
    int syncingBreak;
    short int recursive;
    char *sourceDir;
    char *destinationDir;
} config;

config defaultConfig();
config parseParams();
void showAvailableParams();