#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r+");
    if (file == NULL) {
        printf("Error: could not open file %s\n", argv[1]);
        return 1;
    }
    int num;
    int num_written = 0;
    while (fscanf(file, "%d", &num) == 1) {
        if (num % 2 == 1) {
            fseek(file, num_written * sizeof(int), SEEK_SET);
            fprintf(file, "%d", num);
            num_written++;
        }
    }
    ftruncate(fileno(file), num_written * sizeof(int));
    fclose(file);
    return 0;
}
