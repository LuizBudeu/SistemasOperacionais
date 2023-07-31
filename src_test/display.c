#include <stdio.h>

#define MAX_COMMAND_LENGTH 50

int main() {
    FILE* file = fopen("comandos.txt", "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char command[MAX_COMMAND_LENGTH];
    while(1){
        while (fgets(command, MAX_COMMAND_LENGTH, file) != NULL) {
            printf("Comando recebido: %s", command);
        }
    }

    fclose(file);

    int i;
    scanf("%d", i);
    return 0;
}
