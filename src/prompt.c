#include <stdio.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 50

int main() {
  char command[MAX_COMMAND_LENGTH];
  int memory_units;
  int pid;

  printf("Comandos suportados: create -m {unidades_de_memoria}, kill {PID}\n");

  while (1) {
    printf("> ");
    fgets(command, MAX_COMMAND_LENGTH, stdin);
    if (strcmp(command, "exit\n") == 0) {
      break;
    }

    // Comando "create -m X"
    if (sscanf(command, "create -m %d", &memory_units) == 1) {
        FILE *file = fopen("comandos.txt", "w");
        if (file == NULL) {
          perror("fopen");
          return 1;
        }
        
      if (memory_units >= 1 && memory_units <= 20) {
        fprintf(file, "create -m %d\n", memory_units);
        fflush(file);
      } else {
        printf("Numero de unidades de memoria invalido. Digite um numero entre "
               "1 e 20.\n");
      }
      fclose(file);

    }
    // Comando "kill PID"
    else if (sscanf(command, "kill %d", &pid) == 1) {
      FILE *file = fopen("comandos.txt", "w");
        if (file == NULL) {
          perror("fopen");
          return 1;
        }
      fprintf(file, "kill %d\n", pid);
      fflush(file);
      fclose(file);
    } else {
      printf("Comando invalido. Comandos suportados: create -m {unidades_de_memoria}, kill {PID}\n");
    }
  }

  // fclose(file);
  return 0;
}
