#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int n; // length of sequence that we will compute partial sums for
    printf("Enter the length of the sequence: ");
    scanf("%d", &n);

    // check if sequence length is valid
    // that means it must be greater than 0
    if (n <= 0) {
        printf("Error: Sequence length must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    // read sequence of numbers from the user
    int sequence[n]; // sequence to compute partial sums
    printf("Enter the sequence: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &sequence[i]);
    }

    // we will use a pipe to communicate between parent and child processes
    // pipe is a one-way communication channel
    // we will use it to send partial sums from child processes to parent process
    //
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // create three child processes using fork
    // in the parent process, fork returns the pid of the child process
    // in the child process, fork returns 0
    // for each child we first close the read end of the pipe because the child will only write data to the parent
    // process
    // then we compute the start and end index of the sequence that the child will compute the partial sum for
    // then we compute the partial sum by looping through the sequence
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // child process
            close(fd[0]); // close unused read end of pipe

            int start = i * n / 3; // compute start index for this process
            int end = (i + 1) * n / 3; // compute end index for this process
            int sum = 0;

            for (int j = start; j < end; j++) {
                sum += sequence[j];
            }

            // we write the partial sum to the write end of the pipe using the write system call
            // if there is an error in the process we print an error message and exit the child process with a failure
            // status
            if (write(fd[1], &sum, sizeof(sum)) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }

            //then we close the write end of the pipe since we finished writing and exit the child process with a
            // success status
            close(fd[1]);
            exit(EXIT_SUCCESS);
        }
    }

    // close unused write end of pipe
    close(fd[1]);

    //we declare total_sum and partial_sum variables that we will use to compute the total sum and read the partial
    // sums from the pipe
    int total_sum = 0;
    int partial_sum;

    // read partial sums from child processes and accumulate total sum
    // we loop through the pipe 3 times because we have 3 child processes
    // we read the partial sum from the read end of the pipe using the read system call
    // if there is an error in the process we print an error message and exit the parent process with a failure status
    // we accumulate the total sum by adding the partial sum to the total sum
    for (int i = 0; i < 3; i++) {
        if (read(fd[0], &partial_sum, sizeof(partial_sum)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        total_sum += partial_sum;
    }

    // close read end of pipe
    close(fd[0]);

    // print partial sums
    int prefix_sum = 0;
    for (int i = 0; i < n; i++) {
        prefix_sum += sequence[i];
        printf("%d ", prefix_sum);
    }
    printf("\n");

    // print total sum to verify that it is correct
    printf("Total sum: %d\n", total_sum);

    // wait for child processes to terminate
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}
