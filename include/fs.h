#ifndef FS_H
#define FS_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_FILES 128
#define MAX_DIRS  32
#define FILENAME_LEN 32
#define FILE_SIZE 1024
#define PATH_LEN 256

typedef struct Directory Directory;

typedef struct {
    char name[FILENAME_LEN];
    char data[FILE_SIZE];
    size_t size;
    bool used;
    Directory *parent;
} File;

struct Directory {
    char name[FILENAME_LEN];
    File files[MAX_FILES];
    Directory *subdirs[MAX_DIRS];
    Directory *parent;
    bool used;
};

void init_fs();
bool create_file(const char *name);
bool write_file(const char *name, const char *data);
bool read_file(const char *name, char *buffer, size_t size);
bool delete_file(const char *name);
void list_files();
bool create_directory(const char *name);
bool change_directory(const char *name);
void print_working_directory();

#endif
