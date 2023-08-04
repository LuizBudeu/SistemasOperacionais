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
    int memory_start;
    int max_instructions_execution;
    int num_instructions;
} Process;


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


Queue* create_queue();
void enqueue(Queue* queue, Process process);
Process dequeue(Queue* queue);
int is_queue_empty(Queue* queue);

Process get_random_process(Process* process, int num_processes, Queue* ready_queue);
int is_process_in_queue(Queue* ready_queue, int pid);
void remove_process_by_pid(Queue* ready_queue, int pid);
int get_random_pid_not_in_queue(Queue* ready_queue, Process* process_array, int num_processes);
Process get_process_by_pid(Process* process_array, int num_processes, int pid);

#endif
