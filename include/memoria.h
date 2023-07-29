#ifndef MEMORIA_H
#define MEMORIA_H

#define MEMORY_SIZE 20

extern int memory_bitmap[MEMORY_SIZE];

void init_memory_bitmap();
int allocate_memory(int size);
void deallocate_memory(int memory_start, int program_size);

#endif