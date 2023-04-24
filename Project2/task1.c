/*
Project 2 Process synchronization implementation
1 The producer-consumer problem
below is the C program to solve the producer-consumer problem based on semaphores.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

int slot[10]; // init a buffer with size of 10
int i = 0, o = 0;
sem_t empty, full, mutex;

void *producer(void *arg)
{
    int i = 0;
    // run producer indefinitely to illustrate
    while (true)
    {
        usleep(1000000);                                            // sleep for 1s to observe outputs
        sem_wait(&empty);                                           // wait for an empty slot in the buffer
        sem_wait(&mutex);                                           // acquire the mutex lock to ensure exclusive access to the buffer
        slot[i] = i;                                                // place the produced item in the buffer
        printf("Producer produced item %d at position %d\n", i, i); // print item in detail
        i = (i + 1) % 10;                                           // Update the buffer index, wrap around if the buffer is full
        sem_post(&mutex);                                           // Release the mutex lock to allow other threads access to the buffer
        sem_post(&full);                                            // signal that a new item has been added to the buffer
    }
    return NULL;
}

void *consumer(void *arg)
{
    // run consumer indefinitely to illustrate
    while (true)
    {
        usleep(1000000);                                                 // sleep for 1s to observe outputs
        sem_wait(&full);                                                 // wait for a full slot in the buffer (i.e., an item to consume)
        sem_wait(&mutex);                                                // acquire the mutex lock to ensure exclusive access to the buffer
        int item = slot[o];                                              // retrieve the item from the buffer
        printf("Consumer consumed item %d from position %d\n", item, o); // print item in detail
        o = (o + 1) % 10;                                                // update the buffer index, wrap around if the buffer is full
        sem_post(&mutex);                                                // release the mutex lock to allow other threads access to the buffer
        sem_post(&empty);                                                // signal that an item has been removed from the buffer (i.e., a slot is now empty)
    }
    return NULL;
}

int main()
{
    pthread_t producer_thread, consumer_thread; // Declare producer and consumer threads
    sem_init(&empty, 0, 10); // Initialize the 'empty' semaphore to 10, indicating 10 empty slots in the buffer
    sem_init(&full, 0, 0);   // Initialize the 'full' semaphore to 0, indicating no full slots in the buffer
    sem_init(&mutex, 0, 1);  // Initialize the 'mutex' semaphore to 1, to be used as a lock for exclusive access to the buffer
    pthread_create(&producer_thread, NULL, producer, NULL); // Create and start the producer thread
    pthread_create(&consumer_thread, NULL, consumer, NULL); // Create and start the consumer thread
    pthread_join(producer_thread, NULL); // Wait for the producer thread to finish (in this case, it never will)
    pthread_join(consumer_thread, NULL); // Wait for the consumer thread to finish (in this case, it never will)
    sem_destroy(&empty); // Destroy the 'empty' semaphore
    sem_destroy(&full);  // Destroy the 'full' semaphore
    sem_destroy(&mutex); // Destroy the 'mutex' semaphore
    return 0;
}
