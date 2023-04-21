#include <stdio.h>
#include <stdlib.h>

typedef struct Partition {
    int id;
    int size;
    int start;
    int end;
    struct Partition *next;
} Partition;

typedef enum { FIRST_FIT, BEST_FIT } Algorithm;

Partition *init_memory(int size) {
    Partition *memory = (Partition *)malloc(sizeof(Partition));
    memory->id = 0;
    memory->size = size;
    memory->start = 0;
    memory->end = size - 1;
    memory->next = NULL;
    return memory;
}

void print_partitions(Partition *head) {
    printf("Free partition chain status:\n");
    Partition *current = head;
    while (current) {
        printf("ID: %d, Start: %d, End: %d, Size: %d\n",
               current->id, current->start, current->end, current->size);
        current = current->next;
    }
    printf("\n");
}

Partition *alloc(Partition **head, int size, Algorithm algorithm) {
    Partition *best = NULL;
    Partition *previous_best = NULL;
    Partition *previous = NULL;
    Partition *current = *head;

    while (current) {
        if (current->size >= size) {
            if (algorithm == FIRST_FIT) {
                best = current;
                break;
            } else if (algorithm == BEST_FIT) {
                if (!best || current->size < best->size) {
                    best = current;
                    previous_best = previous;
                }
            }
        }
        previous = current;
        current = current->next;
    }

    if (best) {
        Partition *allocated = (Partition *)malloc(sizeof(Partition));
        allocated->id = best->id;
        allocated->size = size;
        allocated->start = best->start;
        allocated->end = allocated->start + size - 1;

        best->id++;
        best->size -= size;
        best->start += size;

        if (best->size == 0) {
            if (previous_best) {
                previous_best->next = best->next;
            } else {
                *head = best->next;
            }
            free(best);
        }

        return allocated;
    } else {
        return NULL;
    }
}

void free_partition(Partition **head, Partition *partition) {
    Partition *new_free = (Partition *)malloc(sizeof(Partition));
    new_free->id = partition->id;
    new_free->size = partition->size;
    new_free->start = partition->start;
    new_free->end = partition->end;
    new_free->next = *head;
    *head = new_free;
    free(partition);
}

int main() {
    Partition *memory = init_memory(640);
    Partition *allocated_partitions[10] = {NULL};

    allocated_partitions[1] = alloc(&memory, 130, BEST_FIT);
    print_partitions(memory);

    allocated_partitions[2] = alloc(&memory, 60, BEST_FIT);
    print_partitions(memory);

    allocated_partitions[3] = alloc(&memory, 100, BEST_FIT);
    print_partitions(memory);

    free_partition(&memory, allocated_partitions[2]);
    print_partitions(memory);

    allocated_partitions[4] = alloc(&memory, 200, BEST_FIT);
    print_partitions(memory);

     free_partition(&memory, allocated_partitions[3]);
    print_partitions(memory);

    free_partition(&memory, allocated_partitions[1]);
    print_partitions(memory);

    allocated_partitions[5] = alloc(&memory, 140, BEST_FIT);
    print_partitions(memory);

    allocated_partitions[6] = alloc(&memory, 60, BEST_FIT);
    print_partitions(memory);

    allocated_partitions[7] = alloc(&memory, 50, BEST_FIT);
    print_partitions(memory);

    free_partition(&memory, allocated_partitions[6]);
    print_partitions(memory);

    // Free memory
    Partition *temp;
    while (memory) {
        temp = memory;
        memory = memory->next;
        free(temp);
    }

    return 0;
}