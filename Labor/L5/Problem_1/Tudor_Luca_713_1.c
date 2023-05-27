#include <stdio.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if(f == NULL) {
        printf("Error opening the file");
        return 1;
    }

    int character;
    int first_line = 1;
    while((character = fgetc(f)) != EOF) {
        putchar(character);
        if(character == '\n' && !feof(f)) {
            putchar('\n');
        }
        first_line = 0;
    }
    fclose(f);
    return 0;
}
