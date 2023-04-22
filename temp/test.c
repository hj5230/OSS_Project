
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
//The situation is based on the instruction in project.pdf
#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS 5
#define NUM_MEALS 1
sem_t chopsticks[NUM_CHOPSTICKS]; // array of semaphores representing chopsticks
pthread_t philosophers[NUM_PHILOSOPHERS]; // array of philosopher threads
void *philosopher(void *arg)
{
int id = (int)arg; // philosopher id is the argument passed to the thread function
for (int i = 0; i < NUM_MEALS; i++)
{
printf("Philosopher %d is thinking...\n", id);
usleep(rand() % 5000000); // philosopher is thinking for a random amount of time
sem_wait(&chopsticks[id]); // philosopher picks up left chopstick
sem_wait(&chopsticks[(id + 1) % NUM_CHOPSTICKS]); // philosopher picks up right chopstick
printf("Philosopher %d is eating meal %d\n", id, i + 1);
usleep(rand() % 5000000); // philosopher eats for a random amount of time
sem_post(&chopsticks[id]); // philosopher puts down left chopstick
sem_post(&chopsticks[(id + 1) % NUM_CHOPSTICKS]); // philosopher puts down right chopstick
}
printf("Philosopher %d has finished eating\n", id);
return NULL;
}
int main()
{
srand(time(NULL)); // seed the random number generator
// initialize the semaphores representing chopsticks
for (int i = 0; i < NUM_CHOPSTICKS; i++)
{
sem_init(&chopsticks[i], 0, 1);
}
// create threads for each philosopher
for (int i = 0; i < NUM_PHILOSOPHERS; i++)
{
pthread_create(&philosophers[i], NULL, philosopher, (void *)i);
}
// wait for all philosopher threads to finish
for (int i = 0; i < NUM_PHILOSOPHERS; i++)
{
pthread_join(philosophers[i], NULL);
}
// destroy the semaphores representing chopsticks
for (int i = 0; i < NUM_CHOPSTICKS; i++)
{
sem_destroy(&chopsticks[i]);
}
return 0;}