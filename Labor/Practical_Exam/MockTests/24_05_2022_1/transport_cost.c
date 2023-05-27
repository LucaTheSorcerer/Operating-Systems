/**
 * Schreiben Sie ein Client-Server-Programm, das die KOSTEN EINES
TRANSPORTS berechnet. Der Client sendet an den Server eine
Anzahl von Kilometern, die er zurücklegen möchte, und der
Server erstattet die Kosten für den Kauf des Kraftstoffs (in
Lei). Es gilt ein Verbrauch von 5,5l/100km und ein
Kraftstoffpreis von 4,5 Lei/l. Der Client zeigt das empfangene
Ergebnis an. Für eine vollständige Lösung müssen Ausnahmen
(exceptions) berücksichtigt werden.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

#define CONSUMPTION 5.5
#define PRICE 4.5

int main() {

    int pip_parent_child[2];
    int pip_child_parent[2];

    int kilometers;
    double price;

    pipe(pip_parent_child);
    pipe(pip_child_parent);

    int pid = fork();

    if(pid == -1) {
        perror("fork() error");
        exit(EXIT_FAILURE);
    }

    if(pid == 0) {

        //child process
        //close unused pipes

        close(pip_parent_child[1]);
        close(pip_child_parent[0]);

        //read from parent
        if(read(pip_parent_child[0], &kilometers, sizeof(int)) == -1) {
            perror("read() error");
            exit(EXIT_FAILURE);
        }

        //calculate price

        price = (kilometers * CONSUMPTION * PRICE) / 100;

        //write to parent

        if(write(pip_child_parent[1], &price, sizeof(double)) == -1) {
            perror("write() error");
            exit(EXIT_FAILURE);
        }

        //close used pipes
        close(pip_parent_child[0]);
        close(pip_parent_child[1]);
        close(pip_child_parent[0]);
        close(pip_child_parent[1]);

        exit(EXIT_SUCCESS);
    }

    //parent process

    //close unused pipes
    close(pip_parent_child[0]);
    close(pip_child_parent[1]);

    //read from stdin
    printf("Enter kilometers: ");
    scanf("%d", &kilometers);

    //write to child
    if(write(pip_parent_child[1], &kilometers, sizeof(int)) == -1) {
        perror("write() error");
        exit(EXIT_FAILURE);
    }

    //read from child
    if(read(pip_child_parent[0], &price, sizeof(double)) == -1) {
        perror("read() error");
        exit(EXIT_FAILURE);
    }

    //print price
    printf("Price: %.2lf\n", price);

    //close used pipes
    close(pip_parent_child[1]);
    close(pip_child_parent[0]);

    wait(NULL);

    return 0;
    

}