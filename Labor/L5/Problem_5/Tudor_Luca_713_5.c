#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]) {
    char *word1, *word2, *filename;
    FILE *fp;
    char buffer[1024];
    char *ptr;

    //Check is the number of arguments is correct
    if (argc != 4) {
        printf("Usage: %s <programe_name> <word1> <word2> <filename>\n", argv[0]);
        exit(1);
    }

    //Assign the arguments to the variables
    word1 = argv[1];
    word2 = argv[2];
    filename = argv[3];

    //Open the file for reading and writing
    fp = fopen(filename, "r+");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    //Read the file and replace the first word with the second word

    while(fgets(buffer, sizeof(buffer), fp)) {
        ptr = strstr(buffer, word1);
        if(ptr != NULL) {
            strncpy(ptr, word2, strlen(word2));
        }
        printf("%s", buffer);
    }

    //Close the file
    fclose(fp);
    return 0;
}
