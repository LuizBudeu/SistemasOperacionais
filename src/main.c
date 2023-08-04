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


int running_process_pid = -1;


int main() {
    init_memory_bitmap();

    // Criar a fila de processos prontos
    Queue* ready_queue = create_queue();

    char instructions1[][MAX_INSTRUCTION_LENGTH] = {"ADD", "SUB", "ADD", "SUB", "ADD", "SUB", "MOV", "ADD","HTL"};
    char instructions2[][MAX_INSTRUCTION_LENGTH] = {"SUB", "SUB", "MUL", "DIV", "MUL", "MOV", "ADD","HTL"};
    char instructions3[][MAX_INSTRUCTION_LENGTH] = {"MUL", "MOV", "SUB", "MOV","MOV", "ADD","HTL"};
    char instructions4[][MAX_INSTRUCTION_LENGTH] = {"MOV", "ADD", "MOV","MOV", "ADD", "HTL"};
    char instructions5[][MAX_INSTRUCTION_LENGTH] = {"SUB", "ADD", "MOV", "ADD","HTL"};

    Process process1 = create_process(1, 0, instructions1, 9);
    Process process2 = create_process(2, 0, instructions2, 8);
    Process process3 = create_process(3, 0, instructions3, 7);
    Process process4 = create_process(4, 0, instructions4, 6);
    Process process5 = create_process(5, 0, instructions5, 5);

    Process process_array[] = {process1, process2, process3, process4, process5};
    int num_processes = sizeof(process_array) / sizeof(process_array[0]);

    // Inicializar a geração de números aleatórios com o tempo atual
    srand(time(NULL));

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

        running_process_pid = current_process.pid;

        // Verificar se é um processo de criação ("new PID")
        if (strcmp(current_process.instructions[0], "create") == 0) {
            int create_pid = current_process.pid;

            Process process_to_create = get_process_by_pid(process_array, num_processes, create_pid);

            process_to_create.program_size = current_process.program_size;
            process_to_create.memory_start = allocate_memory(current_process.program_size);

            if (process_to_create.memory_start == -1) {
                printf("Erro na alocacao de memoria para o processo %d.\n", process_to_create.pid);
            }

            enqueue(ready_queue, process_to_create);

            printf("Processo %d criado.\n", create_pid);

            display_tcb(current_process);
            printf("\n");

            display_process_status(current_process);
            printf("\n");

            display_memory_bitmap();
            printf("\n");

            display_ready_queue(*ready_queue);

            continue_to_next_cycle();

            continue;
        }

        // Verificar se é um processo de eliminação ("kill PID")
        if (strcmp(current_process.instructions[0], "kill") == 0) {
            int pid_to_kill = current_process.pid;
            remove_process_by_pid(ready_queue, pid_to_kill);

            printf("Processo %d removido.\n", pid_to_kill);

            display_tcb(current_process);
            printf("\n");

            display_process_status(current_process);
            printf("\n");

            display_memory_bitmap();
            printf("\n");

            display_ready_queue(*ready_queue);

            continue_to_next_cycle();

            continue;
        }

        int process_remaining_instructions = current_process.num_instructions - current_process.program_counter;
        int instructions_to_execute = process_remaining_instructions < current_process.max_instructions_execution
                                            ? process_remaining_instructions
                                            : current_process.max_instructions_execution;

        for (int i = 0; i < instructions_to_execute; i++) {
            check_commands_txt(process_array, num_processes, ready_queue);

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
                running_process_pid = -1;
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
        if (strcmp(current->process.instructions[0], "kill") == 0) {
            printf("kill%d", current->process.pid);
        }
        else if (strcmp(current->process.instructions[0], "create") == 0) {
            printf("create%d", current->process.pid);
        }
        else {
            printf("PID%d", current->process.pid);
        }

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
        // char pid_to_kill[MAX_INSTRUCTION_LENGTH];
        int pid_to_kill;

        if (strstr(line, "create -m") != NULL) {
            int mem_required;
            if (sscanf(line, "create -m %d", &mem_required) == 1) {
                // Process new_process = get_random_process(process_array, num_processes, ready_queue);
                // if (mem_required <= 20) {
                //     new_process.program_size = mem_required;
                //     new_process.memory_start = allocate_memory(mem_required);

                //     if (new_process.memory_start == -1) {
                //         printf("Erro na alocacao de memoria para o processo %d.\n", new_process.pid);
                //     }

                //     enqueue(ready_queue, new_process);
                // }
                printf("Comando create -m %d encontrado.\n", mem_required);
                int pid = get_random_pid_not_in_queue(ready_queue, process_array, num_processes);
                Process process_create = create_process(pid, mem_required, (char[][MAX_INSTRUCTION_LENGTH]) {"create"}, 1);
                enqueue(ready_queue, process_create);
                printf("Processo create %d criado.\n", pid);
            }
            else {
                printf("Comando create -m %d invalido.\n", mem_required);
            }
        }

        else if (sscanf(line, "kill %d", &pid_to_kill) == 1) {
            // printf("Comando kill %s encontrado.\n", pid_to_kill);
            Process kill_process = create_process(pid_to_kill, 0, (char[][MAX_INSTRUCTION_LENGTH]) {"kill"}, 1);
            enqueue(ready_queue, kill_process);
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