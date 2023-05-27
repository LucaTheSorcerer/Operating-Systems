/**
 * Schreiben Sie ein Client-Server-Programm, das MEHRWERTSTEUER
(TVA) verwaltet. Der Client sendet einen Preis an den Server
und dieser gibt den Preis mit Mehrwertsteuer zurück. (MwSt. =
24% des Anfangspreises). Der Client zeigt das empfangene
Ergebnis an. Für eine vollständige Lösung müssen Ausnahmen
(exceptions) berücksichtigt werden.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

#define TVA 0.24

int main() {

    int pipe_parent_child[2];
    int pipe_child_parent[2];

    int price;
    double price_with_tva;

    pipe(pipe_parent_child);
    pipe(pipe_child_parent);

    int pid = fork();

    if (pid == -1) {
        perror("fork() error");
        exit(EXIT_FAILURE);
    }

    if(pid == 0) {
        //child process
        //close unused pipes
        close(pipe_parent_child[1]);
        close(pipe_child_parent[0]);

        //read from parent
        if(read(pipe_parent_child[0], &price, sizeof(int)) == -1) {
            perror("read() error");
            exit(EXIT_FAILURE);
        }

        //calculate price with tva
        price_with_tva = price + (price * TVA);

        //write to parent
        if(write(pipe_child_parent[1], &price_with_tva, sizeof(double)) == -1) {
            perror("write() error");
            exit(EXIT_FAILURE);
        }

        //close used pipes
        close(pipe_parent_child[0]);
        close(pipe_parent_child[1]); 
        close(pipe_child_parent[0]);
        close(pipe_child_parent[1]);

        exit(EXIT_SUCCESS);
    }

    //parent process

    //close unused pipes
    close(pipe_parent_child[0]);
    close(pipe_child_parent[1]);

    //read from stdin
    printf("Enter price: ");
    scanf("%d", &price);

    //write to child
    if(write(pipe_parent_child[1], &price, sizeof(int)) == -1) {
        perror("write() error");
        exit(EXIT_FAILURE);
    }

    //read from child
    if(read(pipe_child_parent[0], &price_with_tva, sizeof(double)) == -1) {
        perror("read() error");
        exit(EXIT_FAILURE);
    }

    //print result
    printf("Price with TVA: %.2lf\n", price_with_tva);
}