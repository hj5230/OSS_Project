/*
Project 2 Process synchronization implementation
2 The dining-philosophers problem
below is the C program to solve the dining-philosophers problem based on semaphores
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>

pthread_t philosophers[5];
sem_t chopsticks[5];

void *philosopher(void *arg)
{
    int id = (int)arg;
    while (true)
    {
        printf("Philosopher %d is thinking...\n", id + 1);
        sleep(rand() % 3);
        sem_wait(&chopsticks[id]); // Try to pick up left chopstick
        printf("Philosopher %d picked up left chopstick\n", id + 1);
        usleep(1000); // Introduce a small delay to avoid deadlocks
        sem_wait(&chopsticks[(id + 1) % 5]); // Try to pick up right chopstick
        printf("Philosopher %d picked up right chopstick\n", id + 1);
        printf("Philosopher %d is having meal\n", id + 1);
        sleep(rand() % 2);
        sem_post(&chopsticks[id]); // Put down left chopstick
        printf("Philosopher %d put down left chopstick\n", id + 1);
        sem_post(&chopsticks[(id + 1) % 5]); // Put down right chopstick
        printf("Philosopher %d put down right chopstick\n", id + 1);
    }
    printf("Philosopher %d has finished eating\n", id + 1);
    return NULL;
}

int main()
{
    srand(time(NULL));
    // Initialize the semaphores representing chopsticks
    for (int i = 0; i < 5; i++)
    {
        sem_init(&chopsticks[i], 0, 1);
    }
    // Create threads for each philosopher
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher, (void *)i);
    }
    // Wait for all philosopher threads to finish
    for (int i = 0; i < 5; i++)
    {
        pthread_join(philosophers[i], NULL);
    }
    // Destroy the semaphores representing chopsticks
    for (int i = 0; i < 5; i++)
    {
        sem_destroy(&chopsticks[i]);
    }
    return 0;
}
