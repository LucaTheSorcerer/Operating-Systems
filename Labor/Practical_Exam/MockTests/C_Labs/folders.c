#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 4096
#define MAX_FILENAME_LENGTH 256
#define MAX_LINE_LENGTH (MAX_FILENAME_LENGTH + 1)

int compare_dirent(const void *a, const void *b) {
    const struct dirent *entry_a = *(const struct dirent **)a;
    const struct dirent *entry_b = *(const struct dirent **)b;
    return strcmp(entry_a->d_name, entry_b->d_name);
}

int main() {
    int parent_to_child_pipe[2], child_to_parent_pipe[2];
    char path[MAX_PATH_LENGTH + 1];
    char line[MAX_LINE_LENGTH + 1];

    if (pipe(parent_to_child_pipe) == -1 || pipe(child_to_parent_pipe) == -1) {
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

        DIR *dir;
        struct dirent **entries;
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
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
            num_entries++;
        }
    }

    rewinddir(dir);

    entries = malloc(num_entries * sizeof(struct dirent *));
    if (entries == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
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

        printf("Enter directory name: ");
        scanf("%s", path);

        if (write(parent_to_child_pipe[1], path, strlen(path) + 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(parent_to_child_pipe[1]);

        char filename[MAX_FILENAME_LENGTH + 1];
        while (read(child_to_parent_pipe[0], filename, MAX_FILENAME_LENGTH + 1) > 0) {
            if (strcmp(filename, "error") == 0) {
                fprintf(stderr, "The provided directory does not exist or is not a directory.\n");
                break;
            } else {
                printf("%s\n", filename);
            }
        }

        close(child_to_parent_pipe[0]);
        wait(NULL);
    }

    return 0;
}