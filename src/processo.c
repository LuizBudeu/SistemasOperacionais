#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/processo.h"
#include "../include/memoria.h"


Process create_process(int pid, int program_size, char instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH], int num_instructions) {
    Process process;
    process.pid = pid;
    process.state = PROCESS_STATE_READY;
    // process.program_size = program_size;
    process.program_counter = 0;
    // process.memory_start = allocate_memory(program_size);

    // if (process.memory_start == -1) {
    //     printf("Erro na alocacao de memoria para o processo %d.\n", pid);
    //     return process;
    // }

    process.max_instructions_execution = MAX_INSTRUCTION_EXECUTION;
    process.num_instructions = num_instructions;

    for (int i = 0; i < num_instructions; i++) {
        strcpy(process.instructions[i], instructions[i]);
    }
    return process;
}

void finish_process(Process process, int memory_start) {
    deallocate_memory(memory_start, process.program_size);
    process.state = PROCESS_STATE_TERMINATED;
}

Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("Erro na alocacao de memoria para a fila.\n");
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(Queue* queue, Process process) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Erro na alocacao de memoria para o novo no da fila.\n");
        return;
    }
    new_node->process = process;
    new_node->next = NULL;

    if (queue->rear == NULL) {
        queue->front = new_node;
    } else {
        queue->rear->next = new_node;
    }
    queue->rear = new_node;
}

Process dequeue(Queue* queue) {
    if (is_queue_empty(queue)) {
        printf("A fila esta vazia.\n");
        Process empty_process = {0}; // Process com valores zerados
        return empty_process;
    }

    Node* front_node = queue->front;
    Process process = front_node->process;

    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(front_node);
    return process;
}

int is_queue_empty(Queue* queue) {
    return queue->front == NULL;
}


// Função para obter um processo aleatório do array
Process get_random_process(Process* processes, int num_processes) {
    int random_index = rand() % num_processes;
    return processes[random_index];
}