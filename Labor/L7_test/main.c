#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define BLOCK_SIZE 10


int main( )
{
    int p2c_that_adds_digits[2]; // parent to child
    int c2p_that_adds_digits[2]; // child to parent

    int a, b, s = 0;

    pipe(p2c_that_adds_digits);
    pipe(c2p_that_adds_digits);

    int pid1 = fork();

    if(pid1 == -1) {
        perror("Error creating process 1");
        exit(1);
    }

    if(pid1 == 0) {
        //child process reads the text from the parent process and displays it

        char buffer[100];
        read(p2c_that_adds_digits[0], buffer, 100);
        printf("Child process that adds the numbers received: %s\n", buffer);

        //Calculate the sum of the digits from the text received from the parent process

        for(int i = 0; i < 100; i++) {
            if(isdigit(buffer[i])) {
                s += buffer[i] - '0';
            }
        }

        // send the sum to the parent process

        write(c2p_that_adds_digits[1], &s, sizeof(int));

        close(p2c_that_adds_digits[0]);
        close(p2c_that_adds_digits[1]);
        close(c2p_that_adds_digits[0]);
        close(c2p_that_adds_digits[1]);

        exit(0);

    }


    int p2c_replace_letters_with_z[2]; // parent to child
    int c2p_replace_letters_with_z[2]; // child to parent

    pipe(p2c_replace_letters_with_z);
    pipe(c2p_replace_letters_with_z);

    int pid2 = fork();

    if(pid2 == -1) {
        perror("Error creating process 2");
        exit(1);
    }

    if(pid2 == 0) {
        //child process reads the text from the parent process and displays it

        char buffer[100];
        read(p2c_replace_letters_with_z[0], buffer, 100);
        printf("Child process that replaces letters with Z: %s\n", buffer);

        //Replace all letters from the text received from the parent process with the character ‘Z’ and ignore the digits

        for(int i = 0; i < 100; i++) {
            if(isalpha(buffer[i])) {
                buffer[i] = 'Z';
            }
        }

        // send the text to the parent process

        write(c2p_replace_letters_with_z[1], buffer, 100);

        close(p2c_replace_letters_with_z[0]);
        close(p2c_replace_letters_with_z[1]);
        close(c2p_replace_letters_with_z[0]);
        close(c2p_replace_letters_with_z[1]);

        exit(0);

    }

    int p3c_that_sorts_characters_by_ascii[2]; // parent to child
    int c3p_that_sorts_characters_by_ascii[2]; // child to parent

    pipe(p3c_that_sorts_characters_by_ascii);
    pipe(c3p_that_sorts_characters_by_ascii);

    int pid3 = fork();

    if(pid3 == -1) {
        perror("Error creating process 3");
        exit(1);
    }

    if(pid3 == 0) {
        //child process reads the text from the parent process and displays it to the screen

        char buffer[100];
        read(p3c_that_sorts_characters_by_ascii[0], buffer, 100);
        printf("Child process that sorts characters by ASCII: %s\n", buffer);

        //Sort the characters from the text received from the parent process by ASCII code

        for(int i = 0; i < 100; i++) {
            for(int j = i + 1; j < 100; j++) {
                if(buffer[i] > buffer[j]) {
                    char aux = buffer[i];
                    buffer[i] = buffer[j];
                    buffer[j] = aux;
                }
            }
        }

        // send the text to the parent process

        write(c3p_that_sorts_characters_by_ascii[1], buffer, 100);

        close(p3c_that_sorts_characters_by_ascii[0]);
        close(p3c_that_sorts_characters_by_ascii[1]);
        close(c3p_that_sorts_characters_by_ascii[0]);
        close(c3p_that_sorts_characters_by_ascii[1]);

        exit(0);
    }

    int p4c_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[2]; // parent to child
    int c4p_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[2]; // child to parent

    pipe(p4c_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ);
    pipe(c4p_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ);

    int pid4 = fork();

    if(pid4 == -1) {
        perror("Error creating process 4");
        exit(1);
    }

    if(pid4 == 0) {

        //child process that reads the text from the pipes that come from the process that adds digits and the process that replaces letters with Z and displays it to the screen

        char buffer[100];

        // read the text from the process that adds digits
        read(p2c_that_adds_digits[0], buffer, 100);

        // read the text from the process that replaces letters with Z
        read(p2c_replace_letters_with_z[0], buffer, 100);

        printf("Child process that prints the output of process that adds digits and process that replaces letters with Z: %s\n", buffer);

        // send the text to the parent process
        write(c4p_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[1], buffer, 100);

        close(p2c_that_adds_digits[0]);
        close(p2c_that_adds_digits[1]);
        close(c2p_that_adds_digits[0]);
        close(c2p_that_adds_digits[1]);

        close(p2c_replace_letters_with_z[0]);
        close(p2c_replace_letters_with_z[1]);
        close(c2p_replace_letters_with_z[0]);
        close(c2p_replace_letters_with_z[1]);

        close(p3c_that_sorts_characters_by_ascii[0]);
        close(p3c_that_sorts_characters_by_ascii[1]);
        close(c3p_that_sorts_characters_by_ascii[0]);
        close(c3p_that_sorts_characters_by_ascii[1]);

        close(p4c_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[0]);
        close(p4c_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[1]);
        close(c4p_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[0]);
        close(c4p_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[1]);

        exit(0);
    }

    // parent process that reads a text from the user

//    printf("Enter a text: ");
//    char buffer[100];
//    fgets(buffer, 100, stdin);

    char inputBuffer[1000];
    int bytesRead;
    int i=2;
    //scanf("%s",inputBuffer);
    fgets(inputBuffer, sizeof inputBuffer, stdin);
    int bufferLength = strlen(inputBuffer);
    int numBlocks = bufferLength / BLOCK_SIZE;

    for (int j = 0; j < numBlocks; j++) {
        // Send block of 10 bytes to the appropriate pipe
        if (i == 2) {
        } else if (i == 3) {
            write(c2p_that_adds_digits[1], &inputBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        } else if (i == 4) {
            write(pipe3[1], &inputBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        }

        i++;
        if (i > 4) {
            i = 2;
        }

    // send the text to the child process


    //read the sum from the child process back to parent and display

    read(c2p_that_adds_digits[0], &s, sizeof(int));
    printf("Sum of digits: %d\n", s);

    close(p2c_that_adds_digits[0]);
    close(p2c_that_adds_digits[1]);
    close(c2p_that_adds_digits[0]);
    close(c2p_that_adds_digits[1]);

    // send the text to the child process that replaces letters with Z

    write(p2c_replace_letters_with_z[1], buffer, 100);

    //read the text from the child process back to parent and display

    read(c2p_replace_letters_with_z[0], buffer, 100);
    printf("Text with letters replaced with Z: %s\n", buffer);

    close(p2c_replace_letters_with_z[0]);
    close(p2c_replace_letters_with_z[1]);
    close(c2p_replace_letters_with_z[0]);
    close(c2p_replace_letters_with_z[1]);

    // send the text to the child process that sorts characters by ASCII

    write(p3c_that_sorts_characters_by_ascii[1], buffer, 100);

    //read the text from the child process back to parent and display

    read(c3p_that_sorts_characters_by_ascii[0], buffer, 100);
    printf("Text with characters sorted by ASCII: %s\n", buffer);

    close(p3c_that_sorts_characters_by_ascii[0]);
    close(p3c_that_sorts_characters_by_ascii[1]);
    close(c3p_that_sorts_characters_by_ascii[0]);
    close(c3p_that_sorts_characters_by_ascii[1]);

    // send the text to the child process that prints the output of process that adds digits and process that replaces letters with Z

    write(p4c_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[1], buffer, 100);

    //read the text from the child process back to parent and display

// send the text to the child process that prints the output of process that adds digits and process that replaces letters with Z
    write(p4c_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[1], buffer, 100);

    // read the text from the child process back to the parent and display
    read(c4p_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[0], buffer, 100);
    printf("Text with digits added and letters replaced with Z: %s\n", buffer);

    close(p4c_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[0]);
    close(p4c_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[1]);
    close(c4p_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[0]);
    close(c4p_prints_the_output_of_processThatAddDigits_processThatReplaceLettersWithZ[1]);

    // wait for the child process to finish

    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}
