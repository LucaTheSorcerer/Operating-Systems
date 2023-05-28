#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    int pipe_parent_to_child[2], pipe_child_to_parent[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    off_t total_size = 0;

    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // child process
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);

        bytes_read = read(pipe_parent_to_child[0], buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        
        // Remove the newline character from the input buffer
        buffer[strcspn(buffer, "\n")] = '\0';

        dir = opendir(buffer);
        if (dir == NULL) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }

        while ((entry = readdir(dir)) != NULL) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", buffer, entry->d_name);

            if (stat(path, &file_stat) == -1) {
                perror("stat");
                exit(EXIT_FAILURE);
            }

            if (S_ISREG(file_stat.st_mode)) {
                total_size += file_stat.st_size;
            }
        }

        if (closedir(dir) == -1) {
            perror("closedir");
            exit(EXIT_FAILURE);
        }

        bytes_read = snprintf(buffer, BUFFER_SIZE, "%ld", (long) total_size);
        if (bytes_read < 0) {
            perror("snprintf");
            exit(EXIT_FAILURE);
        }

        if (write(pipe_child_to_parent[1], buffer, bytes_read) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);

        exit(EXIT_SUCCESS);
    } else { // parent process
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);

        printf("Enter directory name: ");
        fflush(stdout);

        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (write(pipe_parent_to_child[1], buffer, bytes_read) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        bytes_read = read(pipe_child_to_parent[0], buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Total size of files in directory: %s bytes\n", buffer);

        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);

        exit(EXIT_SUCCESS);
    }
}