#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

void ProcessoNetos(int netos){
    for(int i = 0; i < netos; i++){
        fflush(stdout);
        pid_t pid = fork();
        if (pid == 0){
            printf("Processo %d, filho de %d\n", getpid(), getppid());
            sleep(5);
            printf("Processo %d finalizado\n", getpid());
            exit(0);
        }
    }

    // Espera os 3 netos
    for (int i = 0; i < netos; i++) {
        wait(NULL);
    }
}

void ProcessoFilhos(int filhos, int netos){
    for(int i = 0; i < filhos; i++){
        fflush(stdout);
        pid_t pid = fork();
        if (pid == 0){
            printf("Processo %d, filho de %d\n", getpid(), getppid());
            ProcessoNetos(netos);
            printf("Processo %d finalizado\n", getpid());
            exit(0);
        }
    }

    // Espera os 2 filhos
    for (int i = 0; i < filhos; i++) {
        wait(NULL);
    }
}

int main() {
    ProcessoFilhos(2, 3);
    printf("Processo principal %d finalizado\n", getpid());    
    return 0;
}
