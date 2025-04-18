#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//       (pai)      
//         |        
//    +----+----+
//    |         |   
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!


int main(int argc, char** argv) {

    // ....

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/
    for(int i=0; i<2; i++){
        fflush(stdout);
        pid_t pid = fork();
        if (pid >= 0){
            if (pid == 0){
                printf("Processo filho %d criado\n", getpid());
                return 0;
            } else {
                printf("Processo pai criou %d\n", pid);
                wait(NULL);
            }
        } 
    }
    while(wait(NULL) >= 0);

    printf("Processo pai finalizado!\n");   
    return 0;
}