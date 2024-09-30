#include <string.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <stdio.h>
#include <sys/stat.h>

int search (char* file, char* dir){
    DIR *dirptr=opendir(dir);
    if (dirptr == NULL) {
        perror("Error opening directory");
        return 1;
    }
    struct dirent *entry = readdir(dirptr);
    while (entry != NULL) {
        if (strlen(entry->d_name) == strlen(file) && (strcmp(entry->d_name,file) == 0)) {
            closedir(dirptr);
            return 0; /* return success */
        }
        entry = readdir(dirptr);
    }
    closedir(dirptr);
    return 1; /* return failure */
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Use the following: %s <file> <directory>\n", argv[0]);
        return 1;
    }

    char *file = argv[1];
    char *dir = argv[2];

    int result = search(file, dir);
    if (result == 0) {
        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, file);
        struct stat file_stat;
        if (stat(full_path, &file_stat) == 0) {
            printf("File size: %lld bytes\n", file_stat.st_size);
        } else {
            perror("Error getting file stats");
        }
    } else {
        printf("File '%s' is not found in the directory '%s'.\n", file, dir);
    }
}