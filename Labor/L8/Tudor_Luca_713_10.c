#include "Tudor_Luca_713_10.h"

int main()
{
    // 1. Create named pipes
    if((mkfifo(FIFO_1_2, 0600)) && (errno != EEXIST))
    {
        perror("Error creating FIFO 1_2");
        exit(1);
    }
    printf("FIFO 1_2 created successfully\n");

    if(mkfifo(FIFO_1_3, 0600) && (errno != EEXIST))
    {
        perror("Error creating FIFO 1_3");
        exit(1);
    }
    printf("FIFO 1_3 created successfully\n");

    if(mkfifo(FIFO_2_4, 0600) && (errno != EEXIST))
    {
        perror("Error creating FIFO 2_4");
        exit(1);
    }
    printf("FIFO 2_4 created successfully\n");

    pid_t pid2, pid3, pid4;



    //Create process 1, that reads that from the keyboard and writes it to FIFO 1_2 and FIFO 1_3

    int fd1;
    fd1 = open(FIFO_1_2, O_RDWR);

    int fd2;
    fd2 = open(FIFO_1_3, O_RDWR);

    int fd3;
    fd3 = open(FIFO_2_4, O_RDWR);

    if(fd1 < 0)
    {
        perror("Error opening FIFO 1_2");
        exit(1);
    }

    // Read from user
    char buffer[100];
    printf("Enter a string: ");
    fgets(buffer, 100, stdin);

    //display the string

//    printf("The string is: %s\n", buffer);


    //Write to FIFO 1_2 only the alhpanumeric rows
    for(int i = 0; i < strlen(buffer); i++)
    {
        if(isalnum(buffer[i]) || isspace(buffer[i]))
        {
            if(write(fd1, &buffer[i], 1) < 0)
            {
                perror("Error writing to FIFO 1_2");
                exit(1);
            }
        }
    }

    printf("String written to FIFO 1_2 %s\n", buffer);

    // Write to FIFO 1_3 only the non-letter characters
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (!isalpha(buffer[i]))
        {
            if (write(fd2, &buffer[i], 1) < 0)
            {
                perror("Error writing to FIFO 1_3");
                exit(1);
            }
        }
    }

    printf("String written to FIFO 1_3: ");
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (!isalpha(buffer[i]) && !isspace(buffer[i]))
        {
            printf("%c", buffer[i]);
        }
    }
    printf("\n");


//     2. Create process 2, that reads from FIFO 1_2 and writes to FIFO 2_4

    if((pid2 = fork()) < 0) {
        printf("Error creating process 2\n");
        exit(1);
    } else if (pid2 == 0) {


        //Read from FIFO 1_2 and display the string the exact string
        char buffer2[100];

        if(read(fd1, buffer2, 100) < 0)
        {
            perror("Error reading from FIFO 1_2");
            exit(1);
        }

        printf("String read from FIFO 1_2: %s\n", buffer2);

        for(int i = 0; i < strlen(buffer2); i++)
        {
            if(isdigit(buffer2[i]))
            {
                buffer2[i] = '_';
            }
        }

//            Display the string
        printf("String after replacing digits with _: %s\n", buffer2);

        close(fd1);

        //Write buffer2 to FIFO 2_4

        if(write(fd3, buffer2, strlen(buffer2)) < 0)
        {
            perror("Error writing to FIFO 2_4");
            exit(1);
        }

        printf("String written to FIFO 2_4: %s\n", buffer2);

        //Close the file descriptors


        exit(0);
    }

//    Process 3 reads the string from FIFO 1_3 and displays it on the screen
    if ((pid3 = fork()) < 0) {
        printf("Error creating process 3\n");
        exit(1);
    } else if (pid3 == 0) {

        // Read from FIFO 1_3
        char buffer3[100];

        // Display to the screen without the white spaces
        if (read(fd2, buffer3, 100) < 0) {
            perror("Error reading from FIFO 1_3");
            exit(1);
        }

        printf("String read from FIFO 1_3: ");
        for (int i = 0; i < strlen(buffer3); i++) {
            if (!isspace(buffer3[i])) {
                printf("%c", buffer3[i]);
            }
        }
        printf("\n");

        // Close the file descriptor
        close(fd2);

        // Exit the child process
        exit(0);
    }

// Wait for process 2 and process 3 to finish
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

// Create process 4, that reads from FIFO 2_4 and displays the string on the screen
    if ((pid4 = fork()) < 0) {
        printf("Error creating process 4\n");
        exit(1);
    } else if (pid4 == 0) {

        // Read from FIFO 2_4
        char buffer4[100];

        if (read(fd3, buffer4, 100) < 0) {
            perror("Error reading from FIFO 2_4");
            exit(1);
        }

        // Close the file descriptor
        close(fd3);

        // Display the string
        printf("String read from FIFO 2_4: %s\n", buffer4);

        // Exit the child process
        exit(0);
    }

// Close the remaining file descriptors in the parent process
    close(fd1);
    close(fd2);
    close(fd3);

    //Wait for process 2 to finish
//    waitpid(pid2, NULL, 0);
//    waitpid(pid3, NULL, 0);
    waitpid(pid4, NULL, 0);




    return 0;
}
