#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 1024

int compare_dirent(const void *a, const void *b) {
    const struct dirent *entry_a = *(const struct dirent **)a;
    const struct dirent *entry_b = *(const struct dirent **)b;
    return strcmp(entry_a->d_name, entry_b->d_name);
}

int main() {
    int parent_to_child_pipe[2];
    int child_to_parent_pipe[2];

    if (pipe(parent_to_child_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(child_to_parent_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child process
        close(parent_to_child_pipe[1]);
        close(child_to_parent_pipe[0]);

        char path[MAX_PATH_LENGTH + 1];
        struct dirent **entries;
        DIR *dir;
        struct stat statbuf;

        if (read(parent_to_child_pipe[0], path, MAX_PATH_LENGTH + 1) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (stat(path, &statbuf) == -1) {
            if (write(child_to_parent_pipe[1], "error", strlen("error") + 1) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        if (!S_ISDIR(statbuf.st_mode)) {
            if (write(child_to_parent_pipe[1], "error", strlen("error") + 1) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        dir = opendir(path);
        if (dir == NULL) {
            if (write(child_to_parent_pipe[1], "error", strlen("error") + 1) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        int num_entries = 0;
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                num_entries++;
            }
        }

        rewinddir(dir);

        entries = malloc(num_entries * sizeof(struct dirent *));
        if (entries == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
int compare_dirent(const void *a, const void *b) {
    const struct dirent *entry_a = *(const struct dirent **)a;
    const struct dirent *entry_b = *(const struct dirent **)b;
    return strcmp(entry_a->d_name, entry_b->d_name);
}
        int i = 0;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                entries[i++] = entry;
            }
        }

        qsort(entries, num_entries, sizeof(struct dirent *), compare_dirent);

        for (i = 0; i < num_entries; i++) {
            if (write(child_to_parent_pipe[1], entries[i]->d_name, strlen(entries[i]->d_name) + 1) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        closedir(dir);
        close(parent_to_child_pipe[0]);
        close(child_to_parent_pipe[1]);

        exit(EXIT_SUCCESS);
    } else {
        // parent process
        close(parent_to_child_pipe[0]);
        close(child_to_parent_pipe[1]);

        char path[MAX_PATH_LENGTH + 1];
        printf("Enter directory name: ");
        fgets(path, MAX_PATH_LENGTH + 1, stdin);
        path[strlen(path) - 1] = '\0';

        if (write(parent_to_child_pipe[1], path, strlen(path) + 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_PATH_LENGTH + 1];
        int num_bytes_read;
        while ((num_bytes_read = read(child_to_parent_pipe[0], buffer, MAX_PATH_LENGTH + 1)) > 0) {
            if (strcmp(buffer, "error") == 0) {
                printf("Error: directory does not exist or is not a directory\n");
                break;
            } else {
                printf("%s\n", buffer);
            }
        }

        close(parent_to_child_pipe[1]);
        close(child_to_parent_pipe[0]);

        wait(NULL);
    }

    return 0;
}