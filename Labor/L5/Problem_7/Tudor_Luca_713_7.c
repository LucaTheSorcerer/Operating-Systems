#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    char *filename = argv[1];
    FILE *file = fopen(filename, "r+b");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(size);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for buffer.\n");
        exit(1);
    }
    fread(buffer, 1, size, file);
    for (int i = 0; i < size/2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[size-1-i];
        buffer[size-1-i] = temp;
    }
    fseek(file, 0, SEEK_SET);
    fwrite(buffer, 1, size, file);
    free(buffer);
    fclose(file);
    return 0;
}