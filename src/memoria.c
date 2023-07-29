#include "../include/memoria.h"


// Mapa de bits para representar a memória
int memory_bitmap[MEMORY_SIZE];


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


// Função para desalocar a memória ocupada por um processo
void deallocate_memory(int memory_start, int program_size) {
    for (int i = memory_start; i < memory_start + program_size; i++) {
        memory_bitmap[i] = 0;
    }
}