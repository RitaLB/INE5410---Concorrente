// ARQUIVO PARA PODER VER A VERSÃO ANTIGA INCORRETA E ESCREVER O TEXTO DE EXPLICAÇÃO DA NOVA SOLUÇÃO

/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

void init_matrix(double* m, int rows, int columns) {
    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            m[i*columns+j] = i + j;
}
*/
/* Anotação sobre acesso à posição de matriz em C:
Cada parte acessa um elemento das matrizes em formato linear (1D), já que C não tem vetores 2D em C puro.

    left[i*cols_left + k] → elemento A[i][k]

    right[k*cols_right + j] → elemento B[k][j]

    out[i*cols_right + j] → elemento C[i][j]
*/

/*
void mult_matrix(double* out, double* left, double *right, 
                 int rows_left, int cols_left, int cols_right) {
    int i, j, k;
    #pragma omp parallel for schedule(dynamic, 1) // Cada thread faz uma linha de left
    for (i = 0; i < rows_left; ++i) { // Para cada linha da matriz A
        for (j = 0; j < cols_right; ++j) { // Para cada coluna da matriz B
            out[i*cols_right+j] = 0;
            #pragma omp parallel for firstprivate(i, j) schedule(guided) // Aqui, cada thread faz a soma de 1 elevent do somatório para criação do novo ítem da matriz C (condição de corrida no acesso à posição do novo item de C. Além de ineficiente e desnecessário)
            for (k = 0; k < cols_left; ++k) // PAra cada ítem das matrizes A e B (somatório para achar novo item da matrz C)
                out[i*cols_right+j] += left[i*cols_left+k]*right[k*cols_right+j]; // out[i][j] += left[i][k] * right[k][j]
        }
    }
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s tam_matriz\n", argv[0]);
        return 1;
    }
    int sz = atoi(argv[1]);
    double* a = malloc(sz*sz*sizeof(double));
    double* b = malloc(sz*sz*sizeof(double));
    double* c = calloc(sz*sz, sizeof(double));

    init_matrix(a, sz, sz);
    init_matrix(b, sz, sz);

    //          c = a * b
    mult_matrix(c,  a,  b, sz, sz, sz);

*/
    /* ~~~ imprime matriz ~~~ */

/*
    char tmp[32];
    int max_len = 1;
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            int len = sprintf(tmp, "%ld", (unsigned long)c[i*sz+j]);
            max_len = max_len > len ? max_len : len;
        }
    }
    char fmt[16];
    if (snprintf(fmt, 16, "%%s%%%dld", max_len) < 0) 
        abort();
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) 
            printf(fmt, j == 0 ? "" : " ", (unsigned long)c[i*sz+j]);
        printf("\n");
    }

    free(a);
    free(b);
    free(c);

    return 0;
}
*/