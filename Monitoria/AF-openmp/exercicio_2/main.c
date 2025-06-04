#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

void init_matrix(double* m, int rows, int columns) {
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            m[i * columns + j] = i + j;
}

void mult_matrix(double* out, double* left, double* right, 
                 int rows_left, int cols_left, int cols_right) {
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < rows_left; ++i) {
        for (int j = 0; j < cols_right; ++j) {
            double sum = 0.0;
            for (int k = 0; k < cols_left; ++k)
                sum += left[i * cols_left + k] * right[k * cols_right + j];
            out[i * cols_right + j] = sum;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s tam_matriz\n", argv[0]);
        return 1;
    }

    int sz = atoi(argv[1]);
    double* a = malloc(sz * sz * sizeof(double));
    double* b = malloc(sz * sz * sizeof(double));
    double* c = calloc(sz * sz, sizeof(double));

    init_matrix(a, sz, sz);
    init_matrix(b, sz, sz);

    mult_matrix(c, a, b, sz, sz, sz);

    // Impressão da matriz
    char tmp[32];
    int max_len = 1;
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            int len = sprintf(tmp, "%.0f", c[i * sz + j]);
            if (len > max_len) max_len = len;
        }
    }

    char fmt[16];
    snprintf(fmt, 16, "%%s%%%d.0f", max_len);

    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j)
            printf(fmt, j == 0 ? "" : " ", c[i * sz + j]);
        printf("\n");
    }

    free(a);
    free(b);
    free(c);
    return 0;
}