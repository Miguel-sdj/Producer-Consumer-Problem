#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
// codigo feito por Miguel Cabral de Carvalho.


// declaração das variáveis globais
int qtdProducer;
int qtdConsumer;
int limit;
int buffer_size;
int buffer_in = 0;
int buffer_out = 0;
int *bufferArray;

// criacao dos semaforos
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void producer(void* id){
    int* i = id; // id atual do produtor
    
    int x = 0;
    while(1){
        if (x >= 0 && x <= limit){ // condição para a criada do dado
            x = (2 * x) + 1;
        } else{
           x = 0;
           x = (2 * x) + 1;
        }
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        
        bufferArray[buffer_in] = x;
        printf("Produtor %d produzindo %d na posição %d\n", *i, bufferArray[buffer_in], buffer_in);
        buffer_in = (buffer_in + 1) % buffer_size;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }

}
void consumer(void* id){
    
    int* i = id; // id atual do consumidor
    int x;

    while(1){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        x = bufferArray[buffer_out];
        printf("Consumidor %d consumindo %d na posição %d\n", *i, x, buffer_out);
        buffer_out = (buffer_out + 1) % buffer_size;
        sleep(1);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        
    }
}

int main() {
    scanf("%d", &qtdProducer);
    scanf("%d", &qtdConsumer);
    scanf("%d", &limit);
    scanf("%d", &buffer_size);

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0, buffer_size);
    sem_init(&full, 0, 0);

    bufferArray = calloc(buffer_size, sizeof(int)); // calloc para poder alterar o valor do array definido global dentro da main
    
    // criacao dos dois arrays de threads (consumididor e produtor)
    pthread_t thread_consumer[qtdConsumer];
    pthread_t thread_producer[qtdProducer];

    // criacao de dois arrays de id 
    int consumidores_id[qtdConsumer];
    int produtores_id[qtdProducer];

    for (int i = 0; i < qtdConsumer; i++){
        consumidores_id[i] = i;
    }

    for (int i = 0; i < qtdProducer; i++){
        produtores_id[i] = i;
    }
    
    // criacao das threads e join delas   
    for (int i = 0; i < qtdConsumer; i++)
	pthread_create(&thread_consumer[i], NULL, (void *)consumer, &consumidores_id[i]);

    for (int i = 0; i < qtdProducer; i++)
	pthread_create(&thread_producer[i], NULL, (void *)producer, &produtores_id[i]);
    
    for (int i = 0; i < qtdProducer; i++)
        pthread_join(thread_producer[i], NULL);

    for (int i = 0; i < qtdConsumer; i++)
        pthread_join(thread_consumer[i], NULL);

    return 0;
}