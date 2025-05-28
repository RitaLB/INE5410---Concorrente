/*
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

FILE* out;

// Usando 2 utex, inspitação no exemplo rodutor  consumidor da aula
sem_t createA, createB;  // Com essa solução, há peigo das threads tentarem modificar o arquivo ao msm tempo né? 
                         // mas, se há arquivo compartilhado, como fazer execução paralela???

void *thread_a(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
	//      +---> arquivo (FILE*) destino
	//      |    +---> string a ser impressa
	//      v    v
        sem_wait(&createA);
        // A função sem_wait() bloqueia a execução da thread até que o semáforo

        fprintf(out, "A");
        // Importante para que vocês vejam o progresso do programa
        // mesmo que o programa trave em um sem_wait().
        fflush(stdout);
        sem_post(&createB);
    }
    return NULL;
}

void *thread_b(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
        sem_wait(&createB);
        fprintf(out, "B");
        fflush(stdout);
        sem_post(&createA);
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s [ITERAÇÕES]\n", argv[0]);
        return 1;
    }
    int iters = atoi(argv[1]);
    srand(time(NULL));
    out = fopen("result.txt", "w");

    pthread_t ta, tb;

    // Inicializando semaforos
    // int sem_init(sem_t *sem, int pshared, unsigned int value);
    sem_init(&createA, 0, 1);
    sem_init(&createB, 0, 1);
    // Cria threads
    pthread_create(&ta, NULL, thread_a, &iters);
    pthread_create(&tb, NULL, thread_b, &iters);

    // Espera pelas threads
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    //Imprime quebra de linha e fecha arquivo
    fprintf(out, "\n");
    fclose(out);

    // Destrói semáforos
    sem_destroy(&createA);
    sem_destroy(&createB);
  
    return 0;
}

*/