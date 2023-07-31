#include <stdio.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 50

int main() {
  FILE *file = fopen("comandos.txt", "w");
  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  char command[MAX_COMMAND_LENGTH];
  int memory_units;
  int pid;

  while (1) {
    printf("Digite um comando: ");
    fgets(command, MAX_COMMAND_LENGTH, stdin);
    if (strcmp(command, "exit\n") == 0) {
      break;
    }

    // Comando "create -m X"
    if (sscanf(command, "create -m %d", &memory_units) == 1) {
      if (memory_units >= 1 && memory_units <= 20) {
        fprintf(file, "create -m %d\n", memory_units);
        fflush(file);
      } else {
        printf("Numero de unidades de memoria invalido. Digite um numero entre "
               "1 e 20.\n");
      }
    }
    // Comando "kill PID"
    else if (sscanf(command, "kill %d", &pid) == 1) {
      fprintf(file, "kill %d\n", pid);
      fflush(file);
    } else {
      printf("Comando invalido. Comandos suportados: create -m X, kill PID\n");
    }
  }

  fclose(file);
  return 0;
}
