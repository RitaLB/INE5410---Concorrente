#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);

// Avalia se o prod_escalar é o produto escalar dos vetores a e b. Assume-se
// que ambos a e b sejam vetores de tamanho size.
void avaliar(double* a, double* b, int size, double prod_escalar);

// Função que calcula o produto escalar de dois vetores a e b, de tamanho size
// e armazena o resultado em result. faz isso usando threads
// (Talvez parallel não seja exatameeente o melhor nome, n tenho certeza se as threads são paralelas, mas n consegui pensar num nome melhor (?))
void parallel_dot_product(double* a, double* b, int size, int n_threads, double* result);

// Função para mandar pra thread, calculo do produto escalar parcial
void* dot_product_thread(void* arg);

struct data {
    int id;
    int num_itens;
    int start_index;
    double* parcial_result;
    double* a;
    double* b;
};


int main(int argc, char* argv[]) {
    srand(time(NULL));

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n", 
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }

    //Calcula produto escalar. Paralelize essa parte
    /*
    double result = 0;
    for (int i = 0; i < a_size; ++i) 
        result += a[i] * b[i];
    */
    double result = 0;
    parallel_dot_product(a, b, a_size, n_threads, &result);
    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, a_size, result);

    //Libera memória
    free(a);
    free(b);

    return 0;
}

void parallel_dot_product(double* a, double* b, int size, int n_threads, double* result){
    // Numero min de itens por thread : min_num_t = size / n_threads
    // resto : rest = size % n_threads
    // Numero de itens por thread : min_num_t + (i < rest ? 1 : 0)

    pthread_t threads[n_threads];
    double partial_results[n_threads];
    int ids[n_threads];
    
    int min_num_t = size / n_threads;
    int rest = size % n_threads;

    struct data values[n_threads];  
    int start_index = 0;

    //Cria n_threads threads informando como argumento de entrada um id único.
    for (int i = 0; i < n_threads; ++i) {
        ids[i] = i;
        int num_items = min_num_t + (i < rest ? 1 : 0);
        // Cada thread vai calcular o produto escalar de um pedaço do vetor
        // e armazenar o resultado em result[i]
        // O resultado final é a soma de todos os resultados parciais

        values[i].id = ids[i];
        values[i].num_itens = num_items;
        values[i].parcial_result = &partial_results[i];
        values[i].a = a;
        values[i].b = b;
        values[i].start_index = start_index;

        //printf("bvai entrar na Thread %d ! i = %d \n", ids[i], i);
        pthread_create(&threads[i], NULL, dot_product_thread , (void *)&values[i]);

        start_index =  start_index + num_items;

    }
    
    for (int i = 0; i < n_threads; ++i)
        pthread_join(threads[i], NULL);
    
    // Soma os resultados parciais
    for (int i = 0; i < n_threads; ++i) {
        *result += partial_results[i];
    }

}

void* dot_product_thread(void* arg){
    struct data* values = (struct data*) arg;
    //int id = values->id;
    int num_items = values->num_itens;
    double* a = values->a;
    double* b = values->b;
    int start_index = values->start_index;

    // Calcula o produto escalar de a e b
    double result = 0;
    for (int j = 0; j < num_items; ++j) {
        result += a[start_index + j] * b[start_index + j];
    }
    
    // Armazena o resultado parcial
    *values-> parcial_result= result;

    //printf("Thread %d iniciada!\n", id);
    pthread_exit(NULL);
}