typedef struct fileList fileList;

struct fileList{
    char *name;
    char *path;
    fileList *next;
};