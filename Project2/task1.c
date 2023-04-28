/*
Project 2 Process synchronization implementation
1 The producer-consumer problem
below is the C program to solve the producer-consumer problem based on semaphores.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
/*
as required, define a buffer of size N
note that N can be changed
*/
#define N 5
int buffer[N];
/* declare initial value of pool io */
int i = 0;
int o = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void *producer(void *arg)
{
    int item;
    while (1)
    {
        item = rand() % 20; // assign a random items
        sem_wait(&empty);    // wait for an empty slot
        sem_wait(&mutex);    // wait for the mutual exclusion semaphore
        buffer[i] = item;                                              // insertong item into buffer
        printf("Producer: Item %d produced and stored at slot %d of buffer pool.\n", item, i + 1); // print producing result
        printf("|%d|%d|%d|%d|%d|\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
        i = (i + 1) % N;                                               // renew input index
        sem_post(&mutex); // Signal the mutual exclusion semaphore
        sem_post(&full);  // Signal that a new full slot is available
        sleep(rand() % 2); // sleep random time
    }
}

void *consumer(void *arg)
{
    int item;
    while (1)
    {
        sem_wait(&full);  // wait for a full slot
        sem_wait(&mutex); // wait for the mutual exclusion semaphore
        item = buffer[o];                                                // get item from the buffer
        buffer[o] = 0;
        printf("Consumer: Item %d at slot %d of buffer pool consumed.\n", item, o + 1); // print consuming result
        printf("|%d|%d|%d|%d|%d|\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
        o = (o + 1) % N;                                                 // renew output index
        sem_post(&mutex); // signal the mutual exclusion semaphore
        sem_post(&empty); // signal that a new empty slot is available
        sleep(rand() % 4); // sleep random time
    }
}

int main()
{
    pthread_t producer_thread, consumer_thread;
    sem_init(&empty, 0, N); // Initialize the empty semaphore with N empty slots
    sem_init(&full, 0, 0);  // Initialize the full semaphore with 0 full slots
    sem_init(&mutex, 0, 1); // Initialize the mutual exclusion semaphore with 1 count
    pthread_create(&producer_thread, NULL, producer, NULL); // create the producer thread
    pthread_create(&consumer_thread, NULL, consumer, NULL); // create the consumer thread
    pthread_join(producer_thread, NULL); // waiting the producer thread terminate
    pthread_join(consumer_thread, NULL); // waiting the consumer thread terminate
    sem_destroy(&empty); // destroy the empty semaphore
    sem_destroy(&full);  // destroy the full semaphore
    sem_destroy(&mutex); // destroy the mutual exclusion semaphore
    return 0;
}
