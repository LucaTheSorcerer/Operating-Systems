#include <stdio.h>
#include <stdlib.h>


/**
 * @brief This program reads a file and deletes the bytes at even addresses.
 * @param argc
 * @param argv
 * @return 0 if the program ran successfully, 1 otherwise
 */
int main(int argc, char* argv[]) {

    // Check if the user provided a file name
    // If not, print the usage and exit
    if (argc < 2) {
        printf("Usage: %s <file name>\n", argv[0]);
        return 1;
    }

    // Open the file
    // If the file cannot be opened, print an error message and exit
    char* file_name = argv[1];
    FILE* file = fopen(file_name, "rb");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", file_name);
        return 1;
    }

    // Find the size of the file
    // fseek() and ftell() are used to find the size of the file
    // fseek() sets the file pointer to the end of the file
    // ftell() returns the current position of the file pointer
    // rewind() is used to reset the file pointer to the beginning of the file
    fseek(file, 0L, SEEK_END);
    long int file_size = ftell(file);
    rewind(file);

    // Allocate memory for the file content of size file_size
    // If the memory cannot be allocated, print an error message and exit
    // The memory is allocated with malloc() and freed with free()
    char* content = (char*) malloc(file_size);
    if (!content) {
        printf("Error: Cannot allocate memory\n");
        fclose(file);
        return 1;
    }

    // Read the file content into memory
    // fread() is used to read the file content into memory
    // The file content is stored in the content variable
    // The file content is read in chunks of size sizeof(char)
    // The number of chunks read is file_size
    fread(content, sizeof(char), file_size, file);
    fclose(file);

    // Delete the bytes at even addresses
    // The bytes at even addresses are deleted by shifting the bytes at odd addresses
    // The bytes at odd addresses are shifted to the left by one position
    // The loop iterates over the content of the array an it copies each odd-indexed byte to the next free even index
    // i is for the even indices and j is for the odd indices
    // After the loop ends j holds the new size of the content array
    int i, j;
    for (i = 0, j = 0; i < file_size; i += 2, j++) {
        content[j] = content[i+1];
    }
    file_size = j;

    // Write the modified content back to the same file
    // The file is opened in write binary mode
    // If the file cannot be opened, print an error message and exit the program and free the memory
    file = fopen(file_name, "wb");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", file_name);
        free(content);
        return 1;
    }

    // Write the modified content to the file
    // The sizeof(char) is used to specify the size of the chunks to be written
    // The file_size is used to specify the number of chunks to be written
    // After the file is written, it is closed and the memory is freed
    fwrite(content, sizeof(char), file_size, file);
    fclose(file);
    free(content);

    return 0;
}
