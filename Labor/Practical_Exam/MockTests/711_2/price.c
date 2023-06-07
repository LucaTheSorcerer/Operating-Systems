#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

int main() {
    int pipe_parent_to_child[2], pipe_child_to_parent[2];
    pid_t pid;

    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // child process
        close(pipe_parent_to_child[PIPE_WRITE_END]);
        close(pipe_child_to_parent[PIPE_READ_END]);

        float original_price, discount_percentage, final_price;
        char buffer[256];

        if (read(pipe_parent_to_child[PIPE_READ_END], buffer, sizeof(buffer)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (sscanf(buffer, "%f %f", &original_price, &discount_percentage) != 2) {
            fprintf(stderr, "Invalid input\n");
            exit(EXIT_FAILURE);
        }

        if (discount_percentage < 0.0 || discount_percentage > 100.0) {
            fprintf(stderr, "Discount percentage outside valid range\n");
            exit(EXIT_FAILURE);
        }

        final_price = original_price * (1.0 - discount_percentage / 100.0);

        snprintf(buffer, sizeof(buffer), "%.2f", final_price);

        if (write(pipe_child_to_parent[PIPE_WRITE_END], buffer, strlen(buffer) + 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    } else { // parent process
        close(pipe_parent_to_child[PIPE_READ_END]);
        close(pipe_child_to_parent[PIPE_WRITE_END]);

        float original_price, discount_percentage;
        char buffer[256];

        printf("Enter the original price and discount percentage: ");
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%f %f", &original_price, &discount_percentage) != 2) {
            fprintf(stderr, "Invalid input\n");
            exit(EXIT_FAILURE);
        }

        if (discount_percentage < 0.0 || discount_percentage > 100.0) {
            fprintf(stderr, "Discount percentage outside valid range\n");
            exit(EXIT_FAILURE);
        }

        snprintf(buffer, sizeof(buffer), "%.2f %.2f", original_price, discount_percentage);

        if (write(pipe_parent_to_child[PIPE_WRITE_END], buffer, strlen(buffer) + 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        if (read(pipe_child_to_parent[PIPE_READ_END], buffer, sizeof(buffer)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Final price: %s\n", buffer);

        exit(EXIT_SUCCESS);
    }
}