#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

int main() {
    int fd1[2], fd2[2], fd3[2], fd4[2];
    pid_t pid1, pid2, pid3, pid4, pid5;

    if (pipe(fd1) == -1 || pipe(fd2) == -1 || pipe(fd3) == -1 || pipe(fd4) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();

    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // Process 1
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);

        char buffer[11];
        int i;
        while (fgets(buffer, 11, stdin) != NULL) {
            for (i = 0; i < 10; i++) {
                write(fd1[1], &buffer[i], 1);
                write(fd2[1], &buffer[i], 1);
                write(fd3[1], &buffer[i], 1);
            }
        }

        close(fd1[1]);
        exit(EXIT_SUCCESS);
    }

    pid2 = fork();

    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // Process 2
        close(fd1[1]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);

        char buffer[11];
        int sum = 0, nread;
        while ((nread = read(fd1[0], buffer, 10)) > 0) {
            buffer[nread] = '\0';
            for (int i = 0; i < strlen(buffer); i++) {
                if (isdigit(buffer[i])) {
                    sum += buffer[i] - '0';
                }
            }
        }

        close(fd1[0]);
        printf("%d\n", sum);
        exit(EXIT_SUCCESS);
    }

    pid3 = fork();

    if (pid3 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid3 == 0) {
        // Process 3
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd4[1]);

        char buffer[11];
        int nread;
        while ((nread = read(fd2[0], buffer, 10)) > 0) {
            buffer[nread] = '\0';
            for (int i = 0; i < strlen(buffer); i++) {
                if (isalpha(buffer[i])) {
                    buffer[i] = 'Z';
                }
            }
            write(fd3[1], buffer, 10);
        }

        close(fd2[0]);
        close(fd3[1]);
        exit(EXIT_SUCCESS);
    }

    pid4 = fork();

    if (pid4 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid5 == 0) {
        // Process 5
        close(fd1[1]);
        close(fd1[0]);
        close(fd2[1]);
        close(fd2[0]);
        close(fd3[1]);
        close(fd3[0]);
        close(fd4[1]);

        char buffer[31];
        int num_bytes;
        while ((num_bytes = read(fd4[0], buffer, 30)) > 0) {
        buffer[num_bytes] = '\0';
        printf("%s\n", buffer);
        }

        close(fd4[0]);
        } else {
        // Parent process
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        char buffer[11];
        int num_bytes;
        while ((num_bytes = read(STDIN_FILENO, buffer, 10)) > 0) {
        buffer[num_bytes] = '\0';
        write(fd1[1], buffer, 10);
        }
        close(fd1[1]);
        }

return 0;
}