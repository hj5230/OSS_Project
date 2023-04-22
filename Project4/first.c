#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Partition {
    int id;
    int start;
    int size;
    bool isAvailable;
    struct Partition *next;
} Partition;

Partition *head = NULL;

void alloc(int jobId, int size) {
    Partition *current = head;
    while (current != NULL) {
        if (current->isAvailable && current->size >= size) {
            current->isAvailable = false;
            current->id = jobId;

            if (current->size > size) {
                Partition *newPartition = (Partition *) malloc(sizeof(Partition));
                newPartition->id = 0;
                newPartition->start = current->start + size;
                newPartition->size = current->size - size;
                newPartition->isAvailable = true;
                newPartition->next = current->next;
                current->next = newPartition;
                current->size = size;
            }

            break;
        }
        current = current->next;
    }
}

void release(int jobId) {
    Partition *current = head;
    Partition *prev = NULL;
    while (current != NULL) {
        if (current->id == jobId) {
            current->isAvailable = true;
            current->id = 0;

            // Merge with next partition if available
            if (current->next != NULL && current->next->isAvailable) {
                current->size += current->next->size;
                current->next = current->next->next;
            }

            // Merge with previous partition if available
            if (prev != NULL && prev->isAvailable) {
                prev->size += current->size;
                prev->next = current->next;
            }

            break;
        }
        prev = current;
        current = current->next;
    }
}

void printPartitions() {
    Partition *current = head;
    printf("ID\tstart\tsize\tisAvailable\n");
    while (current != NULL) {
        printf("%d\t%d\t%d\t%d\n", current->id, current->start, current->size, current->isAvailable);
        current = current->next;
    }
    printf("\n");
}

int main() {
    head = (Partition *) malloc(sizeof(Partition));
    head->id = 0;
    head->start = 0;
    head->size = 640;
    head->isAvailable = true;
    head->next = NULL;

    alloc(1, 130);
    printPartitions();

    alloc(2, 60);
    printPartitions();

    alloc(3, 100);
    printPartitions();

    release(2);
    printPartitions();

    alloc(4, 200);
    printPartitions();

    release(3);
    printPartitions();

    release(1);
    printPartitions();

    alloc(5, 140);
    printPartitions();

    alloc(6, 60);
    printPartitions();

    alloc(7, 50);
    printPartitions();

    release(6);
    printPartitions();

    return 0;
}
