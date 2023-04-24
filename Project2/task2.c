#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

pthread_t philosophers[5];
sem_t chopsticks[5];

void *philosopher(void *arg)
{
    int id = (int)arg;
    while (true)
    {
        printf("Philosopher %d is thinking...\n", id);
        usleep(rand() % 5000000);

        sem_wait(&chopsticks[id]); // Try to pick up left chopstick
        printf("Philosopher %d picked up left chopstick\n", id);
        usleep(1000);                        // Introduce a small delay to avoid deadlocks
        sem_wait(&chopsticks[(id + 1) % 5]); // Try to pick up right chopstick
        printf("Philosopher %d picked up right chopstick\n", id);
        printf("Philosopher %d is having meal\n", id);
        usleep(rand() % 5000000);
        sem_post(&chopsticks[id]); // Put down left chopstick
        printf("Philosopher %d put down left chopstick\n", id);
        sem_post(&chopsticks[(id + 1) % 5]); // Put down right chopstick
        printf("Philosopher %d put down right chopstick\n", id);
    }
    printf("Philosopher %d has finished eating\n", id);
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
