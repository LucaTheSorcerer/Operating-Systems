/**
 * Schreiben Sie ein Client-Server-Programm, das die VERWALTUNG
EINES WOHNBLOCKS berechnet. Der Client sendet an den Server
eine natürliche Zahl, die die Anzahl der Bewohner eines
Blocks darstellt. Der Server hat die folgende Tabelle zur
Verfügung. Der Server gibt die Gesamtkosten für die vom
Client gesendete Zahl zurück. Der Client zeigt das empfangene
Ergebnis an. Für eine vollständige Lösung müssen Ausnahmen
(exceptions) berücksichtigt werden.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

#define WATER 10
#define ELECTRICITY 15
#define MAINTENANCE 20

int main() {
    int pipe_parent_child[2];
    int pipe_child_parent[2];

    int residents;
    double price;

    if(pipe(pipe_parent_child) == -1 || pipe(pipe_child_parent) == -1) {
        perror("pipe() error");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    if(pid == -1) {
        perror("fork() error");
        exit(EXIT_FAILURE);
    }

    if(pid == 0) {
        //child process
        //close unused pipes
        close(pipe_parent_child[1]);
        close(pipe_child_parent[0]);

        //read from parent
        if(read(pipe_parent_child[0], &residents, sizeof(int)) == -1) {
            perror("read() error");
            exit(EXIT_FAILURE);
        }

        //calculate price
        price = residents * (WATER + ELECTRICITY + MAINTENANCE);

        //write to parent
        if(write(pipe_child_parent[1], &price, sizeof(double)) == -1) {
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
    printf("Enter number of residents: ");
    scanf("%d", &residents);

    //write to child
    if(write(pipe_parent_child[1], &residents, sizeof(int)) == -1) {
        perror("write() error");
        exit(EXIT_FAILURE);
    }

    //read from child
    if(read(pipe_child_parent[0], &price, sizeof(double)) == -1) {
        perror("read() error");
        exit(EXIT_FAILURE);
    }

    //print price
    printf("Price: %.2lf\n", price);

    //close used pipes
    close(pipe_parent_child[1]);
    close(pipe_child_parent[0]);

    wait(NULL);
}