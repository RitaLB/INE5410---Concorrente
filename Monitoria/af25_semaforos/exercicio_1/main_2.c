#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

FILE* out;

// Usando 2 mutex, inspiração no exemplo produtor  consumidor da aula
sem_t createA, createB;  // # Dúvida ?? : Se não usar o mutex, há perigo das threads tentarem modificar o arquivo ao msm tempo né? 
                         // mas, se há arquivo compartilhado, como fazer execução paralela???
pthread_mutex_t mutex_file; //  Adição de mutex pra controlar modificação no arquivo. Agora está sequencial e concorrente (## ??? problema)

// Problema (?) identificado: Está induzindo a produzir sempre a mesma sequência (ABBAABBAAB) pois a
// thread A está sendo criadra primeiro, e daí ela faz o seu máximo no momento, depois a B já faz o seu máximo, etc.

void *thread_a(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
	//      +---> arquivo (FILE*) destino
	//      |    +---> string a ser impressa
	//      v    v
        sem_wait(&createA);
        pthread_mutex_lock(&mutex_file);
        // A função sem_wait() bloqueia a execução da thread até que o semáforo

        fprintf(out, "A");
        // Importante para que vocês vejam o progresso do programa
        // mesmo que o programa trave em um sem_wait().
        fflush(stdout);
        pthread_mutex_unlock(&mutex_file); // Ordem??
        sem_post(&createB); // Ordem??
    }
    return NULL;
}

void *thread_b(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
        sem_wait(&createB);
        pthread_mutex_lock(&mutex_file);
        fprintf(out, "B");
        fflush(stdout);
        pthread_mutex_unlock(&mutex_file); // Ordem??
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
    sem_init(&createB, 0, 1); // Inicia os 2 com permissão, para permitir sequências com inícios diferentes
    pthread_mutex_init(&mutex_file, NULL);
    // Cria threads
    pthread_create(&ta, NULL, thread_a, &iters); // thread 'a' criada primeiro, faz iniciar com 'a' na maioria (ate o momento todas) ds vezes
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
    pthread_mutex_destroy(&mutex_file);
  
    return 0;
}

