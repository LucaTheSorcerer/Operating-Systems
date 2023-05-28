#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int count_lines(char *buffer, int size);

int main() {
    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];

    if (pipe(pipe_parent_to_child) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe_child_to_parent) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        if (close(pipe_parent_to_child[WRITE_END]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        if (close(pipe_child_to_parent[READ_END]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        char file_name[256];
        if (read(pipe_parent_to_child[READ_END], file_name, sizeof(file_name)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        FILE *fp = fopen(file_name, "r");
        if (fp == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        //sets the file position indicator to the end of the file
        fseek(fp, 0L, SEEK_END);
        //returns the current value of the file position indicator so the file size in bytes
        int file_size = ftell(fp);
        //sets the file position indicator to the beginning of the file
        rewind(fp);

        //allocate memory for the file contents
        char *buffer = malloc(file_size + 1);
        if (buffer == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        //read the file contents into the buffer
        if (fread(buffer, file_size, 1, fp) != 1) {
            perror("fread");
            exit(EXIT_FAILURE);
        }

        //add the null terminator to the end of the buffer
        buffer[file_size] = '\0';

        //count the number of lines in the file using the count_lines function
        int num_lines = count_lines(buffer, file_size);

        if (write(pipe_child_to_parent[WRITE_END], &num_lines, sizeof(num_lines)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        free(buffer);
        fclose(fp);

        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        if (close(pipe_parent_to_child[READ_END]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        if (close(pipe_child_to_parent[WRITE_END]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        char file_name[256];
        printf("Enter file name: ");
        scanf("%s", file_name);

        if (write(pipe_parent_to_child[WRITE_END], file_name, strlen(file_name) + 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        
        // Wait for child process to finish
        int status;
        if (wait(&status) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }

        // Check if child process exited normally
        //WIFEXITED returns true if the child terminated normally
        //WEXITSTATUS returns the exit status of the child
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != EXIT_SUCCESS) {
                fprintf(stderr, "Child process exited with failure status\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Child process terminated abnormally\n");
            exit(EXIT_FAILURE);
        }

        int num_lines;
        // Read the number of lines from the child process
        if (read(pipe_child_to_parent[READ_END], &num_lines, sizeof(num_lines)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Print the number of lines
        printf("Number of lines in file: %d\n", num_lines);
    }

    return 0;
}

// Function to count the number of lines in a buffer
int count_lines(char *buffer, int size) {
    int num_lines = 0;
    // Count the number of newlines in the buffer
    for (int i = 0; i < size; i++) {
        if (buffer[i] == '\n') {
            num_lines++;
        }
    }
    // Check if last character is not a newline
    if (size > 0 && buffer[size-1] != '\n') {
        num_lines++;
    }
    return num_lines;
}