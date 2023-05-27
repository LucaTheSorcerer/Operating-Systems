#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024

int compare_files(char *filename1, char *filename2) {
    FILE *file1 = fopen(filename1, "rb");
    if (!file1) {
        fprintf(stderr, "Error: Could not open file %s\n", filename1);
        return -1;
    }
    FILE *file2 = fopen(filename2, "rb");
    if (!file2) {
        fprintf(stderr, "Error: Could not open file %s\n", filename2);
        fclose(file1);
        return -1;
    }

    int block_count1 = 0;
    int block_count2 = 0;
    int identical_blocks = 0;

    unsigned char block1[BLOCK_SIZE];
    unsigned char block2[BLOCK_SIZE];

    while (1) {
        size_t n1 = fread(block1, 1, BLOCK_SIZE, file1);
        size_t n2 = fread(block2, 1, BLOCK_SIZE, file2);

        if (n1 != n2 || memcmp(block1, block2, n1) != 0) {
            // blocks are different
            if (n1 > 0) {
                block_count1++;
            }
            if (n2 > 0) {
                block_count2++;
            }
        } else {
            // blocks are identical
            if (n1 == 0) {
                break; // end of file reached
            }
            block_count1++;
            block_count2++;
            identical_blocks++;
        }
    }

    fclose(file1);
    fclose(file2);

    printf("%s: %d\n", filename1, block_count1);
    printf("%s: %d\n", filename2, block_count2);
    printf("Identical blocks: %d\n", identical_blocks);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    return compare_files(argv[1], argv[2]);
}
