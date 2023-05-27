#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BLOCK_SIZE 10

/**
 * Comparison function for qsort function
 * @param a char a to be compared
 * @param b char b to be compared
 */
int cmpfunc(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}
int main() {
    int pipe_process_1_to_2[2], pipe_process_1_to_3[2], pipe_process_1_to_4[2], pipe_process_2_to_5[2], pipe_process_3_to_5[2];
    pid_t pid2, pid3, pid4, pid5;

    //Create the pipes and check if there is an error when creating them
    if (pipe(pipe_process_1_to_2) < 0 || pipe(pipe_process_1_to_3) < 0 || pipe(pipe_process_1_to_4) < 0 || pipe(pipe_process_2_to_5) < 0 || pipe(pipe_process_3_to_5) < 0) {
        printf("Error by creating pipes\n");
        exit(1);
    }


    char inputBuffer[1000];
    int i = 2;

    printf("Enter a text to send to processes: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    int bufferLength = strlen(inputBuffer);
    int numBlocks = bufferLength / BLOCK_SIZE;

    for (int j = 0; j < numBlocks; j++) {
        // Send block of 10 bytes to each of the appropriate pipes
        if (i == 2) {
            write(pipe_process_1_to_2[1], &inputBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        } else if (i == 3) {
            write(pipe_process_1_to_3[1], &inputBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        } else if (i == 4) {
            write(pipe_process_1_to_4[1], &inputBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        }

        i++;
        if (i > 4) {
            i = 2;
        }
    }

    //Close the write ends of the pipes
    close(pipe_process_1_to_2[1]);
    close(pipe_process_1_to_3[1]);
    close(pipe_process_1_to_4[1]);



    //Create process 2
    if ((pid2 = fork()) < 0) {
        printf("Error at creating process 2\n");
        exit(1);
    } else if (pid2 == 0) {

        close(pipe_process_1_to_3[0]);
        close(pipe_process_1_to_4[0]);
        close(pipe_process_3_to_5[1]);
        close(pipe_process_1_to_2[1]);
        close(pipe_process_1_to_3[1]);
        close(pipe_process_1_to_4[1]);
        int sum = 0;

        char buf[1024];
        ssize_t bytes_read;

        //Read from the pipe and add the numbers to the sum variable
        while ((bytes_read = read(pipe_process_1_to_2[0], buf, sizeof(buf))) > 0) {
            for (int i = 0; i < bytes_read; i++) {
                char c = buf[i];
                if (c >= '0' && c <= '9') {
                    sum += (c - '0');
                }
                else if (c == '\0') {
                    break; // end of string
                }
            }
        }
        write(pipe_process_2_to_5[1], &sum, sizeof(sum));


        close(pipe_process_1_to_2[0]);
        close(pipe_process_2_to_5[1]);

        exit(0);
    }

    //Create process 3
    if ((pid3 = fork()) < 0) {
        printf("Error at creating process 3\n");
        exit(1);
    } else if (pid3 == 0) {

        //Close the read ends of the pipes
        close(pipe_process_1_to_4[0]);
        close(pipe_process_1_to_2[1]);
        close(pipe_process_1_to_3[1]);
        close(pipe_process_1_to_4[1]);
        close(pipe_process_1_to_2[0]);
        close(pipe_process_2_to_5[1]);
        char c;

        //Read from the pipe and convert all letters to uppercase Z
        while (read(pipe_process_1_to_3[0], &c, 1) > 0) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                c = 'Z';
            }

            write(pipe_process_3_to_5[1], &c, 1);
        }

        close(pipe_process_1_to_3[0]);
        close(pipe_process_3_to_5[1]);

        exit(0);
    }

    //Create process 4
    if ((pid4 = fork()) < 0) {
        printf("Error at creating process 4\n");
        exit(1);
    } else if (pid4 == 0) {

        close(pipe_process_1_to_2[1]);
        close(pipe_process_1_to_3[1]);
        close(pipe_process_1_to_4[1]);
        close(pipe_process_1_to_2[0]);
        close(pipe_process_2_to_5[1]);
        close(pipe_process_1_to_3[0]);
        close(pipe_process_3_to_5[1]);
        close(pipe_process_2_to_5[1]);
        close(pipe_process_3_to_5[1]);
        char buffer[10000];
        int bytes_read;

        bytes_read = read(pipe_process_1_to_4[0], buffer, 10000);

        // sort the received input using qsort and the additional function cmpfunc
        qsort(buffer, bytes_read, sizeof(char), cmpfunc);

        // print the sorted input to the console
        buffer[bytes_read]=0;
        printf("Process 4 output: %s \n", buffer);

        close(pipe_process_1_to_4[0]);
        exit(0);
    }

    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    if((pid5= fork()) < 0) {
        printf("Error by creating process 5\n");
        exit(1);
    } else if (pid5==0) {

        close(pipe_process_1_to_4[0]);
        close(pipe_process_1_to_2[1]);
        close(pipe_process_1_to_3[1]);
        close(pipe_process_1_to_4[1]);
        close(pipe_process_1_to_2[0]);
        close(pipe_process_2_to_5[1]);
        close(pipe_process_1_to_3[0]);
        close(pipe_process_3_to_5[1]);
        int nr;
        char buf[10];
        printf("Process 2 output sum: ");
        if (read(pipe_process_2_to_5[0], &nr, sizeof(nr)) > 0) {
            printf("%d\n", nr);
        }


        printf("Process 3 letters to Z output: ");
        while (read(pipe_process_3_to_5[0], buf, 1) > 0) {
            printf("%s", buf);
        }

        close(pipe_process_2_to_5[1]);
        close(pipe_process_3_to_5[1]);
        exit(0);
    }

    close(pipe_process_2_to_5[1]);
    close(pipe_process_3_to_5[1]);

    //Wait for all processes to finish
    waitpid(pid4, NULL, 0);
    waitpid(pid5, NULL, 0);

    return 0;
}