/**
 * Schreiben Sie ein Client-Server-Programm, das DELTA einer
quadratischen Gleichung berechnet. Der Client sendet drei
Zahlen an den Server: a, b, c. Der Server gibt die Lösungen
der quadratischen Gleichung mit den Koeffizienten a, b, c
zurück oder schickt eine Nachricht dem Client, falls DELTA <
0. Der Client zeigt das empfangene Ergebnis an. Für eine
vollständige Lösung müssen Ausnahmen (exceptions)
berücksichtigt werden.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

int main() {

    int pipe_parent_child[2];
    int pipe_child_parent[2];

    int a, b, c, pid;

    pipe(pipe_parent_child);
    pipe(pipe_child_parent);

    pid = fork();

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
        read(pipe_parent_child[0], &a, sizeof(int));
        read(pipe_parent_child[0], &b, sizeof(int));
        read(pipe_parent_child[0], &c, sizeof(int));

        //calculate delta
        int delta = b*b - 4*a*c;

        if(delta < 0) {
            printf("Delta is negative. Exiting...\n");
            exit(EXIT_SUCCESS);
        }

        //calculate solutions
        double x1 = (-b + sqrt(delta)) / (2*a);
        double x2 = (-b - sqrt(delta)) / (2*a);
        
        //write to parent
        write(pipe_child_parent[1], &x1, sizeof(double));
        write(pipe_child_parent[1], &x2, sizeof(double));

        //close pipes
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

    //read numbers from stdin
    printf("Enter a: ");
    scanf("%d", &a);
    printf("Enter b: ");
    scanf("%d", &b);
    printf("Enter c: ");
    scanf("%d", &c);

    //write to child
    write(pipe_parent_child[1], &a, sizeof(int));
    write(pipe_parent_child[1], &b, sizeof(int));
    write(pipe_parent_child[1], &c, sizeof(int));

    //read from child
    double x1, x2;
    read(pipe_child_parent[0], &x1, sizeof(double));
    read(pipe_child_parent[0], &x2, sizeof(double));

    //print solutions
    printf("x1 = %lf\n", x1);
    printf("x2 = %lf\n", x2);


}