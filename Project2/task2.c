#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

sem_t mutex;
sem_t S[NUM_PHILOSOPHERS];
int state[NUM_PHILOSOPHERS];

void test(int philosopher) {
    if (state[philosopher] == HUNGRY &&
        state[(philosopher + 4) % NUM_PHILOSOPHERS] != EATING &&
        state[(philosopher + 1) % NUM_PHILOSOPHERS] != EATING) {
        state[philosopher] = EATING;
        sleep(2);
        printf("Philosopher %d takes chopsticks %d and %d\n",
               philosopher + 1, philosopher + 1,
               (philosopher + 1) % NUM_PHILOSOPHERS + 1);
        printf("Philosopher %d is eating\n", philosopher + 1);
        sem_post(&S[philosopher]);
    }
}

void take_chopsticks(int philosopher) {
    sem_wait(&mutex);
    state[philosopher] = HUNGRY;
    printf("Philosopher %d is hungry\n", philosopher + 1);
    test(philosopher);
    sem_post(&mutex);
    sem_wait(&S[philosopher]);
    sleep(1);
}

void put_chopsticks(int philosopher) {
    sem_wait(&mutex);
    state[philosopher] = THINKING;
    printf("Philosopher %d puts down chopsticks %d and %d\n",
           philosopher + 1, philosopher + 1,
           (philosopher + 1) % NUM_PHILOSOPHERS + 1);
    printf("Philosopher %d is thinking\n", philosopher + 1);
    test((philosopher + 4) % NUM_PHILOSOPHERS);
    test((philosopher + 1) % NUM_PHILOSOPHERS);
    sem_post(&mutex);
}

void *philosopher(void *num) {
    int *philosopher_num = num;
    while (1) {
        sleep(1);
        take_chopsticks(*philosopher_num);
        sleep(0);
        put_chopsticks(*philosopher_num);
    }
}

int main() {
    pthread_t thread_id[NUM_PHILOSOPHERS];
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&S[i], 0, 0);
    }

    int philosopher_nums[NUM_PHILOSOPHERS];
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_nums[i] = i;
        pthread_create(&thread_id[i], NULL, philosopher, &philosopher_nums[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}