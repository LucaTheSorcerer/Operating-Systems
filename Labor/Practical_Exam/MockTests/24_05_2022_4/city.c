// /**
//  * Schreiben Sie ein Client-Server-Programm, das Kreisstadte verwaltet. Der
//  * Server liest aus einer Datei eine Liste mit Kreisnamen aus Rumanien und deren Kreisstadte.
//  * Der Client sendet eine Zeichenfolge an den Server, die einen Kreisnamen darstellt und der
//  * Server gibt seinen Kreisstadt zuruck. Es muss auch die Moglichkeit bestehen, den Kreisstadtnamen zu
//  * senden unde den Kreis zu empfangen. Fur eine vollstandige Losung mussen Ausnahmen
//  * (exceptions) berucksichtigt werden.
// */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COUNTY_NAME_LENGTH 50
#define MAX_CITY_NAME_LENGTH 50
#define MAX_LINE_LENGTH (MAX_COUNTY_NAME_LENGTH + MAX_CITY_NAME_LENGTH + 2)

int main() {
    int parent_to_child_pipe[2], child_to_parent_pipe[2];
    char name[MAX_COUNTY_NAME_LENGTH + 1];
    char line[MAX_LINE_LENGTH + 1];
    int found = 0;

    if (pipe(parent_to_child_pipe) == -1 || pipe(child_to_parent_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child process
        close(parent_to_child_pipe[1]);
        close(child_to_parent_pipe[0]);

        FILE *file = fopen("cities.txt", "r");

        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        while (fgets(line, MAX_LINE_LENGTH + 1, file) != NULL) {
            line[strcspn(line, "\n")] = '\0'; // remove newline character

            char *county = strtok(line, ",");
            char *city = strtok(NULL, ",");

            if (county == NULL || city == NULL) {
                fprintf(stderr, "Invalid line in file: %s\n", line);
                continue;
            }

            if (read(parent_to_child_pipe[0], name, MAX_COUNTY_NAME_LENGTH + 1) == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            if (strcmp(name, county) == 0) {
                if (write(child_to_parent_pipe[1], city, strlen(city) + 1) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
                found = 1;
                break;
            } else if (strcmp(name, city) == 0) {
                if (write(child_to_parent_pipe[1], county, strlen(county) + 1) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
                found = 1;
                break;
            }
        }

        fclose(file);
        close(parent_to_child_pipe[0]);
        close(child_to_parent_pipe[1]);

        if (!found) {
            if (write(child_to_parent_pipe[1], "error", strlen("error") + 1) == -1) {
                perror("The city provided is not in the list");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    } else {
        // parent process
        close(parent_to_child_pipe[0]);
        close(child_to_parent_pipe[1]);

        printf("Enter county or city name: ");
        scanf("%s", name);

        if (write(parent_to_child_pipe[1], name, strlen(name) + 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(parent_to_child_pipe[1]);

        char city[MAX_CITY_NAME_LENGTH + 1];
        if (read(child_to_parent_pipe[0], city, MAX_CITY_NAME_LENGTH + 1) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (strcmp(city, "error") == 0) {
            fprintf(stderr, "The provided name does not exist in the file.\n");
        } else {
            printf("%s\n", city);
        }

        close(child_to_parent_pipe[0]);
        wait(NULL);
    }

    return 0;
}