#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 1000

int count_line_words(char *line);


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }

    char line[MAX_LINE_LEN];

    while(fgets(line, MAX_LINE_LEN, f) != NULL) {
        int nr_words = count_line_words(line);
        printf("%d\n", nr_words);
    }

    fclose(f);
    return 0;
}

int count_line_words(char *line) {
    int nr_words = 0;
    char *token = strtok(line, " ");
    while (token != NULL) {
        nr_words++;
        token = strtok(NULL, " ");
    }
    return nr_words;
}
