#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int pipe_parent_to_child[2], pipe_child_to_parent[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    int file_descriptor, word_count = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

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

        dup2(pipe_parent_to_child[0], STDIN_FILENO);
        dup2(pipe_child_to_parent[1], STDOUT_FILENO);

        execlp("wc", "wc", "-w", NULL);

        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // parent process
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);

        file_descriptor = open(argv[1], O_RDONLY);

        if (file_descriptor == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        while (1) {
            ssize_t bytes_read = read(file_descriptor, buffer, BUFFER_SIZE);

            if (bytes_read == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            if (bytes_read == 0) {
                break;
            }

            ssize_t bytes_written = write(pipe_parent_to_child[1], buffer, bytes_read);

            if (bytes_written == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(pipe_parent_to_child[1]);

        wait(NULL);

        ssize_t bytes_read = read(pipe_child_to_parent[0], buffer, BUFFER_SIZE);

        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';

        word_count = atoi(buffer);

        printf("The file %s contains %d words.\n", argv[1], word_count);

        close(pipe_child_to_parent[0]);
    }

    return 0;
}

