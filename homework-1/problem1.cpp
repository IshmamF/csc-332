#include <fcntl.h> 
#include <stdio.h> 
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Use the following: %s directoryNameWithPath source.txt newfile.txt\n", argv[0]);
        return 1;
    }

    char *dir_path = argv[1];
    char *source_file = argv[2];
    char *new_file = argv[3];

    if (mkdir(dir_path, 0777) == -1)
    {
        perror("Error creating directory");
        return 1;
    }

    char full_path[4096];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, new_file);

    // Open source file
    int source_fd = open(source_file, O_RDONLY);
    if (source_fd == -1)
    {
        perror("Error opening source file");
        return 1;
    }

    // Create and open new file
    int dest_fd = open(full_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1)
    {
        perror("Error creating new file");
        close(source_fd);
        return 1;
    }

    int bytesRead;
    char buffer[101];

    while ((bytesRead = read(source_fd, buffer, 100)) > 0)
    {
        buffer[bytesRead] = '\0'; // Null-terminate the string

        for (int i = 0; i < bytesRead; i++)
        {
            if (buffer[i] == '1')
            {
                buffer[i] = 'A';
            }
            if (write(dest_fd, &buffer[i], 1) == -1)
            {
                perror("Error writing to destination file");
                close(source_fd);
                close(dest_fd);
                return 1;
            }
        }
        if (write(dest_fd, "XYZ", 3) == -1)
        {
            perror("Error writing XYZ to destination file");
            close(source_fd);
            close(dest_fd);
            return 1;
        }
    }
    if (bytesRead == -1)
    {
        perror("Error reading from source file");
        close(source_fd);
        close(dest_fd);
        return 1;
    }

    close(source_fd);
    close(dest_fd);
    
    return 0;
}