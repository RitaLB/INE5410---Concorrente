/*
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

sem_t sem_a, sem_b;
int iters;
FILE* out;

void* thread_a(void* args) {
    for (int i = 0; i < iters; i++) {
        sem_wait(&sem_a);     // Espera permissão para imprimir A
        fprintf(out, "A");
        fflush(stdout);
        sem_post(&sem_b);     // Libera B
    }
    return NULL;
}

void* thread_b(void* args) {
    for (int i = 0; i < iters; i++) {
        sem_wait(&sem_b);     // Espera permissão para imprimir B
        fprintf(out, "B");
        fflush(stdout);
        sem_post(&sem_a);     // Libera A
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    printf("\n TESTE \n");
    if (argc < 2) {
        printf("Uso: %s [ITERAÇÕES]\n", argv[0]);
        return 1;
    }

    iters = atoi(argv[1]);
    srand(time(NULL));
    out = fopen("result.txt", "w");

    sem_init(&sem_a, 0, 1);  // Começa com permissão para A
    sem_init(&sem_b, 0, 0);  // B começa bloqueado

    pthread_t ta, tb;
    pthread_create(&ta, NULL, thread_a, NULL);
    pthread_create(&tb, NULL, thread_b, NULL);

    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    sem_destroy(&sem_a);
    sem_destroy(&sem_b);

    fprintf(out, "\n");
    fclose(out);

    return 0;
}

*/