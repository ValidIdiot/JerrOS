#include <fs.h>
#include <jerrio.h>
#include <string.h>
#include <jerrmem.h>

static Directory root;
static Directory *current_directory;

void init_fs() {
    root.used = true;
    strncpy(root.name, "/", FILENAME_LEN);
    root.parent = NULL;
    for (int i = 0; i < MAX_FILES; i++) root.files[i].used = false;
    for (int i = 0; i < MAX_DIRS; i++) root.subdirs[i] = NULL;
    
    current_directory = &root;
    print_str("Filesystem initialized.\n");
}

bool create_file(const char *name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!current_directory->files[i].used) {
            strncpy(current_directory->files[i].name, name, FILENAME_LEN);
            current_directory->files[i].size = 0;
            current_directory->files[i].used = true;
            current_directory->files[i].parent = current_directory;
            print_str("File created: ");
            print_str(name);
            print_str("\n");
            return true;
        }
    }
    print_str("Error: Directory full.\n");
    return false;
}

bool write_file(const char *name, const char *data) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (current_directory->files[i].used && strcmp(current_directory->files[i].name, name) == 0) {
            size_t len = strlen(data);
            if (len > FILE_SIZE) len = FILE_SIZE;
            strncpy(current_directory->files[i].data, data, len);
            current_directory->files[i].size = len;
            print_str("File written: ");
            print_str(name);
            print_str("\n");
            return true;
        }
    }
    print_str("Error: File not found.\n");
    return false;
}

bool read_file(const char *name, char *buffer, size_t size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (current_directory->files[i].used && strcmp(current_directory->files[i].name, name) == 0) {
            if (size > current_directory->files[i].size) size = current_directory->files[i].size;
            strncpy(buffer, current_directory->files[i].data, size);
            buffer[size] = '\0';
            return true;
        }
    }
    print_str("Error: File not found.\n");
    return false;
}

bool delete_file(const char *name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (current_directory->files[i].used && strcmp(current_directory->files[i].name, name) == 0) {
            current_directory->files[i].used = false;
            print_str("File deleted: ");
            print_str(name);
            print_str("\n");
            return true;
        }
    }
    print_str("Error: File not found.\n");
    return false;
}

void list_files() {
    print_str("Contents of directory: ");
    print_str(current_directory->name);
    print_str("\n");

    for (int i = 0; i < MAX_FILES; i++) {
        if (current_directory->files[i].used) {
            print_str(" - [FILE] ");
            print_str(current_directory->files[i].name);
            print_str("\n");
        }
    }

    for (int i = 0; i < MAX_DIRS; i++) {
        if (current_directory->subdirs[i] != NULL) {
            print_str(" - [DIR]  ");
            print_str(current_directory->subdirs[i]->name);
            print_str("\n");
        }
    }
}

bool create_directory(const char *name) {
    for (int i = 0; i < MAX_DIRS; i++) {
        if (current_directory->subdirs[i] == NULL) {
            Directory *new_dir = (Directory *)malloc(sizeof(Directory));
            if (!new_dir) {
                print_str("Error: Memory allocation failed.\n");
                return false;
            }
            strncpy(new_dir->name, name, FILENAME_LEN);
            new_dir->parent = current_directory;
            new_dir->used = true;
            for (int j = 0; j < MAX_FILES; j++) new_dir->files[j].used = false;
            for (int j = 0; j < MAX_DIRS; j++) new_dir->subdirs[j] = NULL;

            current_directory->subdirs[i] = new_dir;
            print_str("Directory created: ");
            print_str(name);
            print_str("\n");
            return true;
        }
    }
    print_str("Error: Directory limit reached.\n");
    return false;
}

bool change_directory(const char *name) {
    if (strcmp(name, "..") == 0) {
        if (current_directory->parent != NULL) {
            current_directory = current_directory->parent;
            print_str("Changed to parent directory.\n");
            return true;
        }
        print_str("Already at root directory.\n");
        return false;
    }

    for (int i = 0; i < MAX_DIRS; i++) {
        if (current_directory->subdirs[i] != NULL && strcmp(current_directory->subdirs[i]->name, name) == 0) {
            current_directory = current_directory->subdirs[i];
            print_str("Changed to directory: ");
            print_str(name);
            print_str("\n");
            return true;
        }
    }
    print_str("Error: Directory not found.\n");
    return false;
}

void print_working_directory() {
    char path[PATH_LEN];
    path[0] = '\0';
    
    Directory *dir = current_directory;
    while (dir != NULL) {
        char temp[PATH_LEN];
        strncpy(temp, "/", PATH_LEN);
        strncat(temp, dir->name, PATH_LEN - strlen(temp) - 1);
        strncat(temp, path, PATH_LEN - strlen(temp) - 1);
        strncpy(path, temp, PATH_LEN);
        dir = dir->parent;
    }

    print_str("Current directory: ");
    print_str(path);
    print_str("\n");
}
