#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s word filename\n", argv[0]);
        return 1;
    }

    char *word = argv[1];
    char *filename = argv[2];

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }

    char *line = NULL;
    size_t line_len = 0;
    ssize_t read_len;
    int count = 0;

    while ((read_len = getline(&line, &line_len, file)) != -1) {
        char *match = strstr(line, word);
        while (match) {
            memmove(match, match + strlen(word), read_len - (match - line) - strlen(word) + 1);
            count++;
            match = strstr(line, word);
        }
        fputs(line, stdout);
    }

    fclose(file);
    free(line);

    file = fopen(filename, "w");
    if (!file) {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }

    fputs(line, file);
    fclose(file);

    printf("Deleted %d occurrences of %s from file %s\n", count, word, filename);
    return 0;
}
