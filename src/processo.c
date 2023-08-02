#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/processo.h"
#include "../include/memoria.h"


Process create_process(int pid, char instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH], int num_instructions) {
    Process process;
    process.pid = pid;
    process.state = PROCESS_STATE_READY;
    process.program_counter = 0;

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
Process get_random_process(Process* processes, int num_processes, Queue* ready_queue) {
    int random_index = rand() % num_processes;
    int selected_process = random_index;

    printf("Processo selecionado: %d\n", selected_process);

    // Enquanto o processo selecionado estiver na fila de processos prontos, escolha outro
    while (is_process_in_queue(ready_queue, selected_process)) {
        selected_process = rand() % num_processes;
    }

    return processes[selected_process-1];
}


int is_process_in_queue(Queue* ready_queue, int pid) {
    printf("Procurando processo %d na fila...\n", pid);

    Node* current = ready_queue->front;
    printf("PID: %d\n", pid);

    while (current != NULL) {
        printf("dentro do while\n");
        if (current->process.pid == pid) {
            return 1; // Processo encontrado na fila
        }
        current = current->next;
    }

    printf("Processo nao encontrado na fila.\n");

    return 0; // Processo não encontrado na fila
}


void remove_process_by_pid(Queue* ready_queue, int pid) {
    Node* current = ready_queue->front;
    Node* previous = NULL;

    while (current != NULL) {
        if (current->process.pid == pid) {
            if (previous == NULL) {
                ready_queue->front = current->next;
            } else {
                previous->next = current->next;
            }
            deallocate_memory(current->process.memory_start, current->process.program_size);
            free(current);
            return;
        }

        previous = current;
        current = current->next;
    }
}