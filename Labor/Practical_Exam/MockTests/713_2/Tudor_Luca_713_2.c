   /**
    * Schreiben Sie ein Client-Server-Programm, das ein Fitness-Tracking-System simuliert. Der
    Client sendet dem Server die Anzahl der Schritte, die ein Benutzer unternommen hat, und der
    Server berechnet den entsprechenden Fitness-Level basierend auf vordefinierten Kriterien.
    Der Client zeigt dann den empfangenen Fitness-Level an. Stellen Sie sicher, dass Sie
    Ausnahmen in Ihrer Lösung behandeln.
    Der Server berechnet den Fitness-Level basierend auf den folgenden Kriterien:
        - Schritte unter 5.000: Bewegungsmangel
        - Schritte 5.000-9.999: Leichte Aktivität
        - Schritte 10.000-14.999: Mäßige Aktivität
        - Schritte 15.000-19.999: Aktiv
        - Schritte 20.000 und mehr: Sehr aktiv
    Zusätzliche Anforderungen:
    1. Behandeln Sie Ausnahmen für ungültige Eingaben, wie nicht-numerische Werte für
    die Anzahl der Schritte.
    2. Behandeln Sie Ausnahmen für negative Werte oder Werte, die für die Anzahl der
    Schritte zu groß sind.
    * */ 

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

    if(pid == 0) {
        close(pipe_parent_to_child[PIPE_WRITE_END]);
        close(pipe_child_to_parent[PIPE_READ_END]);

        int fitness;
        char buffer[256];

        if(read(pipe_parent_to_child[PIPE_READ_END], buffer, sizeof(buffer)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if(sscanf(buffer, "%d", &fitness) != 1) {
            fprintf(stderr, "Invalid Input. Please enter a valid fitnes level\n");
            exit(EXIT_FAILURE);
        }

        //The below checks are not necessarily needed because the above check already looks for positive integers
        //but I left them here
        //Check if the read value is negative and if it is output a message saying that the input is invalid

        if(fitness < 0) {
            // fprintf(stderr, "Invalid Input\n");
            exit(EXIT_FAILURE);
        }

        //Check if the read value is a string and if it is output a message saying that the input is invalid

        if(fitness == 0) {
            // fprintf(stderr, "Invalid Input\n");
            exit(EXIT_FAILURE);
        }

        //If the fitness level is under 5000 output that it is not a normal fitness level

        if(fitness < 5000) {
            snprintf(buffer, sizeof(buffer), "Bewegungsmangel");
        }

        if(fitness >= 5000 && fitness < 10000) {
            snprintf(buffer, sizeof(buffer), "Leichte Aktivität");
        }

        //If the fintenss level is between 1000 and <15000 then output Massige Akitivitat
        if(fitness >= 10000 && fitness < 15000) {
            snprintf(buffer, sizeof(buffer), "Mäßige Aktivität");
        }

        if(fitness >= 15000 && fitness < 20000) {
            snprintf(buffer, sizeof(buffer), "Mäßige Aktivität");
        }


        if(fitness >= 20000) {
            snprintf(buffer, sizeof(buffer), "Sehr aktiv");
        }

    

        if(write(pipe_child_to_parent[PIPE_WRITE_END], buffer, strlen(buffer) + 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipe_parent_to_child[PIPE_READ_END]);
        close(pipe_child_to_parent[PIPE_WRITE_END]);

        exit(EXIT_SUCCESS);
    
    } else {

        //Parent process
        //close used pipes
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);

        int fitness;
        char buffer[256];

        printf("Enter the fitness level: ");
        fgets(buffer, sizeof(buffer), stdin);

        if(sscanf(buffer, "%d", &fitness) != 1) {
            fprintf(stderr, "Invalid Input. Please enter a valid fitness level\n");
            exit(EXIT_FAILURE);
        }

        //The below checks are not necessarily needed because the above check already looks for positive integers
        //Check if the read value is negative and if it is output a message saying that the input is invalid

        if(fitness < 0) {
            fprintf(stderr, "Invalid Input. Please enter a valid fitness level\n");
            exit(EXIT_FAILURE);
        }

        //Check if the read value is a string and if it is output a message saying that the input is invalid

        if(fitness == 0) {
            fprintf(stderr, "Invalid Input. Please enter a valid fitness level\n");
            exit(EXIT_FAILURE);
        }

        snprintf(buffer, sizeof(buffer), "%d", fitness);

        //Write to child process
        if(write(pipe_parent_to_child[PIPE_WRITE_END], buffer, strlen(buffer) + 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        //Read from child process

        if(read(pipe_child_to_parent[PIPE_READ_END], buffer, sizeof(buffer)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        //Output the fitness level message based on what it received from the child process

        printf("Fitness level: %s\n", buffer);

        //Close the pipes
        exit(EXIT_SUCCESS);


    }
}
