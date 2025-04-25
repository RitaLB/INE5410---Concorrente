#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Matrizes a serem multiplicadas
extern int tamanho_matriz;
extern  int **matriz1;
extern  int **matriz2;
// Matriz resultante
extern  int **resultado;


void multiplicar_matrizes(int **A, int **B, int resultado_v[][tamanho_matriz]) {
    for (int i = 0; i < tamanho_matriz; i++)
        for (int j = 0; j < tamanho_matriz; j++) {
            resultado_v[i][j] = 0;
            for (int k = 0; k < tamanho_matriz; k++)
                resultado_v[i][j] += A[i][k] * B[k][j];
        }
}

int comparar_matrizes(int **A, int resultado_v[][tamanho_matriz]) {
    for (int i = 0; i < tamanho_matriz; i++)
        for (int j = 0; j < tamanho_matriz; j++)
            if (A[i][j] != resultado_v[i][j])
                return 0;
    return 1;
}

void print_matriz(int **M) {
    for (int i = 0; i < tamanho_matriz; i++) {
        for (int j = 0; j < tamanho_matriz; j++)
            printf("%d ", M[i][j]);
        printf("\n");
    }
}

int verificar_multiplicacao() {

    int resultado_v[tamanho_matriz][tamanho_matriz];

    multiplicar_matrizes(matriz1, matriz2, resultado_v);

    // Compara com resultado esperado
    if (comparar_matrizes(resultado, resultado_v)) {
        printf("A multiplicação está correta!\n");
    } else {
        printf("Resultado incorreto. Diferenças detectadas.\n");
        printf("\nResultado esperado:\n");
        int *pt_r[tamanho_matriz];
        for (int i = 0; i < tamanho_matriz; i++)
            pt_r[i] = resultado_v[i];
        print_matriz(pt_r);
        printf("\nResultado calculado:\n");
        print_matriz(resultado);
    }

    return 0;
}
