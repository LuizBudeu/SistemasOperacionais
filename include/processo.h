#ifndef PROCESSO_H
#define PROCESSO_H


#define MAX_INSTRUCTIONS 100
#define MAX_INSTRUCTION_LENGTH 10
#define MAX_INSTRUCTION_EXECUTION 2


// Estados do processo
#define PROCESS_STATE_READY 0
#define PROCESS_STATE_RUNNING 1
#define PROCESS_STATE_BLOCKED 2
#define PROCESS_STATE_TERMINATED 3


typedef struct {
    int pid;
    int state;
    int program_size;
    int program_counter;
    char instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH];
    // char** instructions;
    int memory_start;
    int max_instructions_execution;
    int num_instructions;
} Process;


// typedef struct {
//     // int pid;
//     // int state;
//     // int program_counter;
//     Process process;
// } TCB;


typedef struct Node {
    Process process;
    struct Node* next;
} Node;


typedef struct {
    Node* front;
    Node* rear;
} Queue;


Process create_process(int pid, int program_size, char instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH], int num_instructions);
void finish_process(Process process, int memory_start);

// Queue* create_queue();
// void enqueue(Queue* queue, TCB tcb);
// TCB dequeue(Queue* queue);
// int is_queue_empty(Queue* queue);

Queue* create_queue();
void enqueue(Queue* queue, Process process);
Process dequeue(Queue* queue);
int is_queue_empty(Queue* queue);

Process get_random_process(Process* process, int num_processes);

#endif
