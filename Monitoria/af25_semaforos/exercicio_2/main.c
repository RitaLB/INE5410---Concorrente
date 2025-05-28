#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

/* ---------- Definições Globais. ---------- */
#define TEMPO_BASE 1000000

typedef enum { 
	CONTINENTE, 
	ILHA 
} cabeceira_t;

typedef struct {
	int id;
	cabeceira_t cabeceira;
	pthread_t thread;
} veiculo_t;
/* ---------------------------------------- */

/* ---------- Variáveis Globais. ---------- */
char cabeceiras[2][11] = { { "CONTINENTE" }, { "ILHA" } };  // ## Dúvida  ?? Qual a utilidade desses 2 ???
int total_veiculos;
int veiculos_turno;

int contador_veiculos;
sem_t ent_continente;
sem_t ent_ilha;
pthread_mutex_t mtx_contador_veiculos;



// ToDo: Adicione aque quaisquer outras variávels globais necessárias.
/* ---------------------------------------- */

// Muda direcao travessia
void mudar_direcao_travessia(veiculo_t *v){
	contador_veiculos = 0;
	sem_t * sem_nova_direcao;
	if (v->cabeceira == CONTINENTE){
		sem_nova_direcao = &ent_continente;
	}
	else {
		sem_nova_direcao = &ent_ilha;
	}

	for (int i = 0; i< veiculos_turno; i++){
		sem_post(sem_nova_direcao);
	}
	/* Você deverá imprimir a nova direção da travessia quando for necessário! */	
	printf("\n[PONTE] *** Novo sentido da travessia: %s -> %s. ***\n\n", cabeceiras[v->cabeceira], cabeceiras[!v->cabeceira]); //  verificar [!v->cabeceira]
	fflush(stdout);
}

/* Inicializa a ponte. */
void ponte_inicializar(unsigned int veiculos_turno ) {
	
	// inicializando semáforos:
	//  int sem_init(sem_t *sem, int pshared, unsigned int value)
    sem_init(&ent_continente, 0 , veiculos_turno); // Começa com velicuos_turno inicializado
	sem_init(&ent_ilha, 0, 0); // Inicializa sem permições, para carros esperarem musança de sentido

	// Inicializa mutex controle número de veículos por turno
	pthread_mutex_init(&mtx_contador_veiculos, NULL);

	/* Imprime direção inicial da travessia. NÃO REMOVER! */
	printf("\n[PONTE] *** Novo sentido da travessia: CONTINENTE -> ILHA. ***\n\n");
	fflush(stdout);
}

/* Função executada pelo veículo para ENTRAR em uma cabeceira da ponte. */
void ponte_entrar(veiculo_t *v) {
	sem_t * direcao_veiculo;
	if (v->cabeceira == CONTINENTE){
		direcao_veiculo = &ent_continente;
	}
	else {
		direcao_veiculo = &ent_ilha;
	}
	sem_wait(direcao_veiculo);
}

/* Função executada pelo veículo para SAIR de uma cabeceira da ponte. */
void ponte_sair(veiculo_t *v) {

	// ToDo: IMPLEMENTAR!

	// Mudança travessia
	pthread_mutex_lock(&mtx_contador_veiculos);
	contador_veiculos ++;
	if (contador_veiculos == veiculos_turno){
		mudar_direcao_travessia(v);
	}
	pthread_mutex_unlock(&mtx_contador_veiculos);

}

/* FINALIZA a ponte. */
void ponte_finalizar() {

	// ToDo: IMPLEMENTAR!
	
	/* Imprime fim da execução! */
	printf("[PONTE] FIM!\n\n");
	fflush(stdout);
}

/* Implementa o comportamento de um veículo. */
void * veiculo_executa(void *arg) {
	veiculo_t *v = (veiculo_t *) arg;
	
	printf("[Veiculo %3d] Aguardando para entrar na ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);

	/* Entra na ponte. */
	ponte_entrar(v);
	printf("[Veiculo %3d] ENTROU na ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);

	/* Faz a travessia. */
	usleep(TEMPO_BASE + rand() % 1000);
	
	/* Seta cabeceira oposta para sair. */
	v->cabeceira = !v->cabeceira;

	/* Sai da ponte na cabeceira oposta. */
	printf("[Veiculo %3d] SAIU da ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);
	ponte_sair(v);

	pthread_exit(NULL);
}

int main(int argc, char **argv) {
 
	if (argc < 3) {
		printf("Indique a quantidade total de veiculos e o numero de veiculos que podem atravessar a ponte por turno:\n\n %s [QTD_TOTAL_VEICULOS] [VEICULOS_POR_TURNO]\n\n", argv[0]);
		return 1;
	}

	total_veiculos = atoi(argv[1]);
	veiculos_turno = atoi(argv[2]);

	if (total_veiculos % (veiculos_turno * 2)) {
		printf("ERRO: O valor [QTD_TOTAL_VEICULOS] deve ser divisivel por ([VEICULOS_POR_TURNO] * 2)!\n\n %s [QTD_TOTAL_VEICULOS] [VEICULOS_POR_TURNO]\n\n", argv[0]);
		return 1;
	}

	srand(time(NULL));

	/* Aloca os veículos. */
	veiculo_t veiculos[total_veiculos];

	ponte_inicializar(veiculos_turno);

	/* Cria os veículos. */
	for (int i = 0; i < total_veiculos; i++) {
		/* Define o id do veículo. */
		veiculos[i].id = i;
		
		/* Escolhe aleatoreamente se o veículo entra pela ILHA ou CONTINENTE. */
		veiculos[i].cabeceira = i % 2; 
		
		/* Cria a thread veículo. */
		pthread_create(&veiculos[i].thread, NULL, veiculo_executa, (void *) &veiculos[i]);		
	}

	/* Aguarda o término da execução de todos os veículos antes de finalizar o programa. */
	for (int i = 0; i < total_veiculos; i++) {
		pthread_join(veiculos[i].thread, NULL);
	}

	ponte_finalizar();

	return 0;
}