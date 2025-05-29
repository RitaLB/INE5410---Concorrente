#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

void benchmark(int size, int max_threads);
;

// Usar OpenMP para paralelizar aqui
// # ?? Dúvida: Pode usar schedule para otiizar? 
void calcular(double* c, int size, int n_threads) {
    omp_set_num_threads(n_threads); // Dúvida: Deveria estar aqui ou na main?
    // # Dpuvia? Deveria estar tudo numa mesma linha de diretiva?
    #pragma omp parallel shared(c) //  lista C é compartilhada
    #pragma omp for schedule(dynamic) //  guided pois quanto maior o i, mais custoso é o cálculo do sqrt
    for (long long int i = 0; i < size; i++) {
        c[i] = sqrt(i * 32) + sqrt(i * 16 + i * 8) + sqrt(i * 4 + i * 2 + i);
        c[i] -= sqrt(i * 32 * i * 16 + i * 4 + i * 2 + i);
        c[i] += pow(i * 32, 8) + pow(i * 16, 12);
    }
}


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s threads [tamanho]\n", argv[0]);
        return 1;
    }
    int n_threads = atoi(argv[1]);


    // Se o último argumento for "bench", executa benchmark
    if (strcmp(argv[argc - 1], "bench") == 0) {
        int size = argc > 3 ? atoi(argv[3]) : 20000000;
        benchmark(size, n_threads); 
        return 0;
    }

    int size = argc > 2 ? atoi(argv[2]) : 20000000;
    double *c = (double *) malloc (sizeof(double) * size);


    //Guarda ponto de início da computação
    double start = omp_get_wtime(); // Testar numero diferente de threads e calcular speedup
    calcular(c, size, n_threads);
    double duration = omp_get_wtime()-start; //quanto tempo passou
    printf("n_threads: %d, size: %d, tempo: %.3f secs\n", 
           n_threads, size, duration);

    free(c);

    return 0;
}

void benchmark(int size, int max_threads){
    double *c = (double *) malloc (sizeof(double) * size);
    double tempos[max_threads + 1];

    printf("schedule(guided)\n");
    printf("%10s %15s %15s\n", "Threads", "Tempo (s)", "Speedup");

    for (int t = 1; t<= max_threads; t*=2){
        //Guarda ponto de início da computação
        double start = omp_get_wtime(); // Testar numero diferente de threads e calcular speedup
        calcular(c, size, t);
        double duration = omp_get_wtime()-start; //quanto tempo passou
        tempos[t] = duration;


        // Print resultados cada iteração:
        if (t == 1) {
            printf("%10d %15.5f %15s\n", t, duration, "1.00 (base)");
        } else {
            printf("%10d %15.5f %15.2f\n", t, duration, tempos[1] / duration);
        }
    }
    free(c);

}
