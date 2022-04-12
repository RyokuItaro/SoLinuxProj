typedef struct {
    int syncingBreak;
    short int recursive;
    char *sourceDir;
    char *destinationDir;
} config;

config defaultConfig();
config parseParams(int argc, char *argv[]);
void showAvailableParams();