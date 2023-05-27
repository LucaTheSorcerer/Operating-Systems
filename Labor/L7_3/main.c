#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 11 // 10 bytes + 1 null terminator

int main() {

    int pipe_process1_to_process2[2];
    int pipe_process1_to_process3[2];
    int pipe_process1_to_process4[2];

    if(pipe(pipe_process1_to_process2) == -1) {
        perror("Error creating pipe");
        exit(1);
    }

    // Create process 1

    pid_t process1_pid = fork();

    if(process1_pid == -1) {
        perror("Error creating process 1");
        exit(1);
    }

    if(process1_pid == 0) {
        // The parent process reads a text from standard input and writes 10 bytes at each output to processes 2, 3 and 4.
        // To clarify: In this process, data is stored in 10-byte blocks at three different recipients.
        // The first recipient will receive: the first 10 bytes, the second recipient receives the next 10 bytes and the
        // third recipient receives the following 10 Bytes. This sending process is repeated, with the first recipient
        // receiving the next 10 bytes and in the same order continues until there are no more bytes to read from the user.
        // For each process print the text that they receive
        //Read from process 1 to process 2, process 3 and process 4

        char buffer[BUFFER_SIZE];
        int counter = 0;

        while(1) {
            //Read a text from the standard input

            int readBytes = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);

            if(readBytes == 0) {
                //End of file reached
                break;
            }

            buffer[readBytes] = '\0';

            //Write 10 bytes to process 2, the next 10 bytes to process 3 ignoring the previous 10,
            // the next 10 bytes to process 4 igooring the previous 20 and so on

            write(pipe_process1_to_process2[1], buffer, readBytes);
            write(pipe_process1_to_process3[1], buffer, readBytes);
            write(pipe_process1_to_process4[1], buffer, readBytes);

            counter += readBytes;

            //Reset the counter after every 30 bytes

            if(counter >= 30) {
                //Reset the counter
                counter = 0;

                //Wait for the child processes to finish
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }
        }

    }

    // Create process 2

    pid_t process2_pid = fork();

    if(process2_pid == -1) {
        perror("Error creating process 2");
        exit(1);
    }

    //Process 2 has the following task: Adds only the digits from the text received from process 1 and sends the result to process 5.
    //Process 2 will receive data from process 1 and send data to process 5.

    if(process2_pid == 0) {
        //Read from process 1 to process 2
        //Write from process 2 to process 5

        char buffer[BUFFER_SIZE];
        int sum = 0;

        while(1) {
            //Read data from process 1
            int readBytes = read(pipe_process1_to_process2[0], buffer, BUFFER_SIZE - 1);

            if(readBytes == 0) {
                //End of file reached
                break;
            }

            buffer[readBytes] = '\0';

            //Add the digits from the text received from process 1
            for(int i = 0; i < readBytes; i++) {
                if(isdigit(buffer[i])) {
                    sum += buffer[i] - '0';
                }
            }
        }

        //Write the result to process 5
        char result[10];
        sprintf(result, "%d", sum);
        write(STDOUT_FILENO, result, strlen(result));

        //End the process
        exit(0);
    }

    // Create process 3
    // Process 3 has the following task: Replace all letters from the text received from process 1 with the character ‘Z’ and sends the result to process 5.

    pid_t process3_pid = fork();

    if(process3_pid == -1) {
        perror("Error creating process 3");
        exit(1);
    }

    if(process3_pid == 0) {
        //Read from process 1 to process 3
        //Write from process 3 to process 5

        char buffer[BUFFER_SIZE];

        while(1) {
            //Read data from process 1
            int readBytes = read(pipe_process1_to_process3[0], buffer, BUFFER_SIZE - 1);

            if(readBytes == 0) {
                //End of file reached
                break;
            }

            buffer[readBytes] = '\0';

            //Replace all letters from the text received from process 1 with the character ‘Z’
            for(int i = 0; i < readBytes; i++) {
                if(isalpha(buffer[i])) {
                    buffer[i] = 'Z';
                }
            }

            //Write the result to process 5
            write(STDOUT_FILENO, buffer, readBytes);
        }

        //End the process
        exit(0);
    }

    // Create process 4
    //Process 4 has the following task: Sorts the text received from process 1 by ascii code but does not send the result to process 5.

    pid_t process4_pid = fork();

    if(process4_pid == -1) {
        perror("Error creating process 4");
        exit(1);
    }

    if(process4_pid == 0) {
        //Read from process 1 to process 4
        //Write from process 4 to process 5

        char buffer[BUFFER_SIZE];

        while(1) {
            //Read data from process 1
            int readBytes = read(pipe_process1_to_process4[0], buffer, BUFFER_SIZE - 1);

            if(readBytes == 0) {
                //End of file reached
                break;
            }

            buffer[readBytes] = '\0';

            //Sort the text received from process 1 by ascii code
            for(int i = 0; i < readBytes - 1; i++) {
                for(int j = i + 1; j < readBytes; j++) {
                    if(buffer[i] > buffer[j]) {
                        char aux = buffer[i];
                        buffer[i] = buffer[j];
                        buffer[j] = aux;
                    }
                }
            }

        }

        //End the process
        exit(0);
    }

    // Create process 5
    //Process 5 has the following task: Print the results received from processes 2, 3 to the screen

    pid_t process5_pid = fork();

    if(process5_pid == -1) {
        perror("Error creating process 5");
        exit(1);
    }

    if(process5_pid == 0) {
        //Read from process 2, process 3 to process 5
        //Write from process 5 to the screen

        char buffer[BUFFER_SIZE];

        while(1) {
            //Read data from process 2
            int readBytes = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);

            if(readBytes == 0) {
                //End of file reached
                break;
            }

            buffer[readBytes] = '\0';

            //Write the result to the screen
            write(STDOUT_FILENO, buffer, readBytes);
        }

        //End the process
        exit(0);
    }

    //Wait for the child processes to finish






    return 0;
}
