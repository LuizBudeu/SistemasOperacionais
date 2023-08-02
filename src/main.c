#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/processo.h"
#include "../include/memoria.h"


void display_process_status(Process process);
void display_ready_queue(Queue queue);
void display_memory_bitmap();
void display_tcb();
void continue_to_next_cycle();
void check_commands_txt(Process* process_array, int num_processes, Queue* ready_queue);


int main() {
    init_memory_bitmap();

    // Criar a fila de processos prontos
    Queue* ready_queue = create_queue();

    char instructions1[][MAX_INSTRUCTION_LENGTH] = {"ADD", "SUB", "ADD", "SUB", "ADD", "SUB", "HTL"};
    char instructions2[][MAX_INSTRUCTION_LENGTH] = {"SUB", "SUB", "MUL", "DIV", "MUL", "HTL"};
    char instructions3[][MAX_INSTRUCTION_LENGTH] = {"MUL", "MOV", "SUB", "MOV","HTL"};
    char instructions4[][MAX_INSTRUCTION_LENGTH] = {"MOV", "ADD", "MOV", "HTL"};
    char instructions5[][MAX_INSTRUCTION_LENGTH] = {"SUB", "ADD", "HTL"};

    Process process1 = create_process(1, 3, instructions1, 7);
    Process process2 = create_process(2, 5, instructions2, 6);
    Process process3 = create_process(3, 4, instructions3, 5);
    Process process4 = create_process(4, 3, instructions4, 4);
    Process process5 = create_process(5, 5, instructions5, 3);

    Process process_array[] = {process1, process2, process3, process4, process5};
    int num_processes = sizeof(process_array) / sizeof(process_array[0]);

    // Inicializar a geração de números aleatórios com o tempo atual
    srand(time(NULL));

    // enqueue(ready_queue, process1);
    // enqueue(ready_queue, process2);
    // enqueue(ready_queue, process3);

    // Simular a execução dos processos (Round Robin (preemptivo))
    while (1) {

        check_commands_txt(process_array, num_processes, ready_queue);

        if (is_queue_empty(ready_queue)) {
            printf("Nenhum processo na fila de prontos.\n");

            display_memory_bitmap();
            printf("\n");

            display_ready_queue(*ready_queue);

            continue_to_next_cycle();
            continue;
        }

        Process current_process = dequeue(ready_queue);

        int process_remaining_instructions = current_process.num_instructions - current_process.program_counter;
        int instructions_to_execute = process_remaining_instructions < current_process.max_instructions_execution
                                            ? process_remaining_instructions
                                            : current_process.max_instructions_execution;

        for (int i = 0; i < instructions_to_execute; i++) {
            char* instruction = current_process.instructions[current_process.program_counter];

            display_tcb(current_process);
            printf("\n");

            display_process_status(current_process);
            printf("\n");

            display_memory_bitmap();
            printf("\n");

            display_ready_queue(*ready_queue);


            // Verificar o fim do processo
            if (strcmp(instruction, "HTL") == 0) {
                printf("Processo %d terminou.\n", current_process.pid);
                deallocate_memory(current_process.memory_start, current_process.program_size);
                current_process.state = PROCESS_STATE_TERMINATED;
                break;
            }

            current_process.program_counter++;

            continue_to_next_cycle();
        }

        // Se o processo não terminou, adicioná-lo novamente à fila de prontos
        if (current_process.state != PROCESS_STATE_TERMINATED) {
            enqueue(ready_queue, current_process);
        }
        else {
            display_tcb(current_process);
            printf("\n");

            display_memory_bitmap();
            printf("\n");

            display_ready_queue(*ready_queue);

            continue_to_next_cycle();
        }
    }

    free(ready_queue);
    return 0;
    
}


// Função para exibir o status do processo
void display_process_status(Process process) {
    for (int i = 0; i < process.num_instructions; i++) {
        if (i == process.program_counter) {
            printf("%s <---\n", process.instructions[i]);
        } else {
            printf("%s\n", process.instructions[i]);
        }
    }
}


// Função para exibir a fila de processos prontos
void display_ready_queue(Queue queue) {
    printf("Fila de processos prontos: ");
    Node* current = queue.front;
    while (current != NULL) {
        printf("PID%d", current->process.pid);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("\n");
}


void display_memory_bitmap() {
    printf("memory_bitmap: ");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%d ", memory_bitmap[i]);
    }
    printf("\n");
}


void display_tcb(Process current_process) {
    printf("TCB: PID=%d , PC: %d, REG AX: ..., REG BX: ...\n", current_process.pid, current_process.program_counter);
}


void continue_to_next_cycle() {
    // Apertar enter para seguir para próxima instrução
    char enter = 0;
    while (enter != '\r' && enter != '\n') {
        enter = getchar();
    }
}


void check_commands_txt(Process* process_array, int num_processes, Queue* ready_queue) {
    FILE* file = fopen("bin/comandos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de comandos.\n");
        return;
    }

    // Ler cada linha do arquivo e executar os comandos
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        // Verificar o comando "create -m X"
        if (strstr(line, "create -m") != NULL) {
            int mem_required;
            if (sscanf(line, "create -m %d", &mem_required) == 1) {
                Process new_process = get_random_process(process_array, num_processes);
                if (mem_required <= 20) {
                    new_process.program_size = mem_required;
                    new_process.memory_start = allocate_memory(mem_required);

                    if (new_process.memory_start == -1) {
                        printf("Erro na alocacao de memoria para o processo %d.\n", new_process.pid);
                    }

                    enqueue(ready_queue, new_process);
                }
            }
        }
    }

    // Fechar o arquivo
    fclose(file);

    // Fechar e reabrir o arquivo no modo de escrita para apagar seu conteúdo
    fclose(file);
    file = fopen("bin/comandos.txt", "w");
    if (file == NULL) {
        printf("Erro ao reabrir o arquivo de comandos.\n");
        return;
    }

    // Fechar o arquivo novamente
    fclose(file);
}