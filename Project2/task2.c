#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define LIMIT 5

sem_t chopsticks[LIMIT];
pthread_t philosophers[LIMIT];

void *philosopher(void *arg)
{
    int id = (int)arg;

    while(1)
    {
        printf("Philosopher %d is thinking...\n", id);
        usleep(rand() % 5000000);

        // Try to pick up left chopstick
        sem_wait(&chopsticks[id]);
        printf("Philosopher %d picked up left chopstick\n", id);

        // Introduce a small delay to avoid deadlocks
        usleep(1000);

        // Try to pick up right chopstick
        sem_wait(&chopsticks[(id + 1) % LIMIT]);
        printf("Philosopher %d picked up right chopstick\n", id);

        printf("Philosopher %d is having meal\n", id);
        usleep(rand() % 5000000);

        // Put down left chopstick
        sem_post(&chopsticks[id]);
        printf("Philosopher %d put down left chopstick\n", id);

        // Put down right chopstick
        sem_post(&chopsticks[(id + 1) % LIMIT]);
        printf("Philosopher %d put down right chopstick\n", id);
    }

    printf("Philosopher %d has finished eating\n", id);
    return NULL;
}

int main()
{
    srand(time(NULL));

    // Initialize the semaphores representing chopsticks
    for (int i = 0; i < LIMIT; i++)
    {
        sem_init(&chopsticks[i], 0, 1);
    }

    // Create threads for each philosopher
    for (int i = 0; i < LIMIT; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher, (void *)i);
    }

    // Wait for all philosopher threads to finish
    for (int i = 0; i < LIMIT; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy the semaphores representing chopsticks
    for (int i = 0; i < LIMIT; i++)
    {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}
