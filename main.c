#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_INSTRUCTIONS 100
#define MAX_INSTRUCTION_LENGTH 10
#define MEMORY_SIZE 20


typedef struct {
    int pid;  // Process ID
    int state;  // 0 = pronto, 1 = rodando, 2 = bloqueado, 3 = terminado
    int program_size;  // Número de unidades de memória necessárias para armazenar o programa
    int program_counter;  // Contador de programa
    char instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH];  // Instruções do programa
} Process;


// typedef struct tcb_node {
//     Process process;
//     struct tcb_node *next;
// } TCB_Node;


void init_memory_bitmap();
int allocate_memory(int size);
Process create_process(int pid, int program_size, char instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH]);
void deallocate_memory(int memory_start, int program_size);
void finish_process(Process process, int memory_start);


// Mapa de bits para representar a memória
int memory_bitmap[MEMORY_SIZE];


int main() {
    init_memory_bitmap();

    // Cria um processo
    Process process = create_process(1, 3, (char [][MAX_INSTRUCTION_LENGTH]) {"ADD", "SUB", "HTL"});
    Process process2 = create_process(2, 5, (char [][MAX_INSTRUCTION_LENGTH]) {"ADD", "SUB", "HTL"});

    // Aloca memória para o processo
    int memory_start = allocate_memory(process.program_size);
    int memory_start2 = allocate_memory(process2.program_size);

    printf("memory_start 1: %d\n", memory_start);
    printf("memory_start 2: %d\n", memory_start2);

    printf("memory_bitmap: ");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%d ", memory_bitmap[i]);
    }

    printf("\n");

    if (memory_start != -1) {
        // Memória alocada com sucesso
        process.state = 1;
        printf("Processo %d alocado na posicao %d.\n", process.pid, memory_start);

        // Simular a execução do programa
        while (process.program_counter < process.program_size) {
            char* instruction = process.instructions[process.program_counter];

            printf("Executando instrucao: %s\n", instruction);

            // Verificar o fim do processo
            if (strcmp(instruction, "HTL") == 0) {
                printf("Processo %d terminou.\n", process.pid);
                finish_process(process, memory_start);
                break;
            }

            process.program_counter++;
        }

    } else {
        // Não há memória contígua disponível para alocar o processo
        printf("Nao foi possivel alocar memoria para o processo %d.\n", process.pid);
    }

    printf("final memory_bitmap: ");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%d ", memory_bitmap[i]);
    }

    return 0;
    
}


// Inicializar o mapa de bits
void init_memory_bitmap() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory_bitmap[i] = 0;
    }
}


// Função para alocar memória usando o algoritmo First-Fit
int allocate_memory(int size) {
    int start = -1;
    int count = 0;

    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (!memory_bitmap[i]) {
            if (count == 0) {
                start = i;
            }
            count++;
            if (count == size) {
                // Marcar a memória como ocupada no mapa de bits
                for (int j = start; j < start + size; j++) {
                    memory_bitmap[j] = 1;
                }
                return start;
            }
        } else {
            count = 0;
        }
    }

    return -1; // Não há memória contígua suficiente disponível
}


Process create_process(int pid, int program_size, char instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH]) {
    Process process;
    process.pid = pid;
    process.state = 0; // Estado "pronto"
    process.program_size = program_size;
    process.program_counter = 0;
    for (int i = 0; i < program_size; i++) {
        strcpy(process.instructions[i], instructions[i]);
    }
    return process;
}


// Função para desalocar a memória ocupada por um processo
void deallocate_memory(int memory_start, int program_size) {
    for (int i = memory_start; i < memory_start + program_size; i++) {
        memory_bitmap[i] = 0;
    }
}


void finish_process(Process process, int memory_start) {
    deallocate_memory(memory_start, process.program_size);
    process.state = 3; // Estado "encerrado"
}
