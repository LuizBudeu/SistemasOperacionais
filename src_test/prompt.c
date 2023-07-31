#include <stdio.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 50

int main() {
    FILE* file = fopen("comandos.txt", "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char command[MAX_COMMAND_LENGTH];
    while (1) {
        printf("Digite um comando: ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        if (strcmp(command, "exit\n") == 0) {
            break;
        }
        fprintf(file, "%s", command);
        fflush(file); // Força a escrita imediata no arquivo
    }

    fclose(file);
    return 0;
}
