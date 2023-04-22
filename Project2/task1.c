#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define Max 20

sem_t empty;
sem_t full;
sem_t mutex;

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

void *producer(void *param) {
    int product = 0, counter = 0;
    while (counter < Max) {
        counter++;
        
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = product;
        printf("Produced: %d\n", product);
        product++;
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *param) {
    int counter = 0;
    while (counter < Max) {
        counter++;

        sem_wait(&full);
        sem_wait(&mutex);

        int product = buffer[out];
        printf("Consumed: %d\n", product);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);
    }
}

int main() {
    pthread_t producer_thread, consumer_thread;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}