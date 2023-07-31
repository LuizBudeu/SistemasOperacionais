#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/processo.h"
#include "../include/memoria.h"


int main() {
    init_memory_bitmap();

    // Criar a fila de processos prontos
    Queue* ready_queue = create_queue();

    // char *instructions1[10] = {"ADD", "SUB", "HTL"};
    // char *instructions2[10] = {"ADD2", "SUB2", "HTL"};
    Process process1 = create_process(1, 3, (char [][MAX_INSTRUCTION_LENGTH]) {"ADD", "SUB", "HTL"}, 3);  // TODO: num_instructions automatico
    Process process2 = create_process(2, 5, (char [][MAX_INSTRUCTION_LENGTH]) {"ADD2", "SUB2", "HTL"}, 3);
    // Process process1 = create_process(1, 3, instructions1);
    // Process process2 = create_process(2, 5, instructions2);

    // Criar o TCB para cada processo
    // TCB tcb_process1 = {process.pid, process.state, process.program_counter};
    // TCB tcb_process2 = {process2.pid, process2.state, process2.program_counter};

    printf("initial memory_bitmap: ");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%d ", memory_bitmap[i]);
    }
    printf("\n");

    enqueue(ready_queue, process1);
    enqueue(ready_queue, process2);

    // // Simular a execução dos processos (Sequencial)
    // while (!is_queue_empty(ready_queue)) {
    //     Process current_process = dequeue(ready_queue);

    //     printf("Executando processo %d\n", current_process.pid);
    //     while (current_process.program_counter < current_process.program_size) {
    //         char* instruction = current_process.instructions[current_process.program_counter];

    //         printf("Executando instrucao: %s\n", instruction);

    //         // Verificar o fim do processo
    //         if (strcmp(instruction, "HTL") == 0) {
    //             printf("Processo %d terminou.\n", current_process.pid);
    //             deallocate_memory(current_process.memory_start, current_process.program_size);
    //             current_process.state = PROCESS_STATE_TERMINATED;
    //             break;
    //         }

    //         current_process.program_counter++;
    //     }

    //     // Se o processo não terminou, adicioná-lo novamente à fila de prontos
    //     if (current_process.state != PROCESS_STATE_TERMINATED) {
    //         enqueue(ready_queue, current_process);
    //     }
    // }

    // Simular a execução dos processos (Round Robin (preemptivo))
    while (!is_queue_empty(ready_queue)) {
        Process current_process = dequeue(ready_queue);

        printf("Executando processo %d\n", current_process.pid);

        int process_remaining_instructions = current_process.num_instructions - current_process.program_counter;  // 3
        int instructions_to_execute = process_remaining_instructions < current_process.max_instructions_execution // 2
                                            ? process_remaining_instructions
                                            : current_process.max_instructions_execution;

        for (int i = 0; i < instructions_to_execute; i++) {
            char* instruction = current_process.instructions[current_process.program_counter];

            printf("Executando instrucao: %s\n", instruction);

            // Verificar o fim do processo
            if (strcmp(instruction, "HTL") == 0) {
                printf("Processo %d terminou.\n", current_process.pid);
                deallocate_memory(current_process.memory_start, current_process.program_size);
                current_process.state = PROCESS_STATE_TERMINATED;
                break;
            }

            current_process.program_counter++;
        }

        // Se o processo não terminou, adicioná-lo novamente à fila de prontos
        if (current_process.state != PROCESS_STATE_TERMINATED) {
            enqueue(ready_queue, current_process);
        }

        // printf("memory_bitmap: ");
        // for (int i = 0; i < MEMORY_SIZE; i++) {
        //     printf("%d ", memory_bitmap[i]);
        // }
        // printf("\n");
    }

    free(ready_queue);

    printf("final memory_bitmap: ");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%d ", memory_bitmap[i]);
    }
    printf("\n");
    // for (int i = 0; i < MEMORY_SIZE; i++) {
    //     printf("%d ", memory_bitmap[i]);
    // }

    return 0;
    
}