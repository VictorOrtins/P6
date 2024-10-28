#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "cores.h"

#define BUFFER_SIZE 12
#define NUM_PALAVRAS 4

char *buffer[BUFFER_SIZE];
int front = 0, rear = 0;

int bool_produtor[BUFFER_SIZE];
int bool_consumidor[BUFFER_SIZE];

int qtd_palavras = 0;

int qtd_produtoras;
int qtd_consumidoras;

int contador_produtoras = 0;
int contador_consumidores = 0;


char* frases[NUM_PALAVRAS] = {
    "Arsenal Liverpool Manchester City",
    "Manchester United Everton Leicester",
    "Wolverhampton Luton Town Burnley",
    "Olá Meu Nome é"
};

typedef struct{
    int num_threads_produtoras;
    int num_threads_consumidoras;
}ThreadsDefine;


ThreadsDefine verifica_args(int argc, char *argv[], int print_sucess);

void insereNoBuffer(char *palavra);

void* produtor(void* args){
    long id = (long)args;

    int index_palavra = rand() % NUM_PALAVRAS;
    char *mensagem = strdup(frases[index_palavra]);

    printf("Mensagem a ser enviada pela thread %ld produtora: %s\n", id, mensagem);

    char* palavra = strtok(mensagem, " ");
    while(palavra){
        while( (rear + 1) % BUFFER_SIZE == front){
        };
        buffer[rear] = palavra;
        rear = (rear + 1) % BUFFER_SIZE;
        palavra = strtok(NULL, " ");
        sleep(1);
        printf("%d\n", ++qtd_palavras);
    }

    contador_produtoras++;
    pthread_exit(NULL);

}   

void *consumidor(void* args){
    long id = (long)args;

    pthread_t tid = pthread_self();
    printf("Quantidade Produtoras: %d\n", qtd_produtoras);

    while(qtd_palavras < NUM_PALAVRAS*qtd_produtoras){
        while(front == rear){
            if (qtd_palavras >= NUM_PALAVRAS*qtd_produtoras){
                pthread_exit(NULL);
            }
        };
        char* mensagem = buffer[front];
        printf("%lu - Thread %ld consumidora: %s\n", tid, id, mensagem);
        front = (front + 1) % BUFFER_SIZE;
        sleep(1);
    }

    contador_consumidores++;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    ThreadsDefine threads_define;
    threads_define = verifica_args(argc, argv, 0);

    pthread_t produtoras[threads_define.num_threads_produtoras];
    pthread_t consumidoras[threads_define.num_threads_consumidoras];

    time_t t;
    srand(time(&t));

    qtd_produtoras = threads_define.num_threads_produtoras;
    qtd_consumidoras = threads_define.num_threads_consumidoras;

    for(long i = 0; i < threads_define.num_threads_produtoras; i++){
        pthread_create(&produtoras[i], NULL, produtor, (void*)i);
    }

    for(long i = 0; i < threads_define.num_threads_consumidoras; i++){
        pthread_create(&consumidoras[i], NULL, consumidor, (void*)i);
    }

    while(contador_consumidores < threads_define.num_threads_consumidoras && contador_produtoras < threads_define.num_threads_produtoras){
        for(int i = 0; i < qtd_consumidoras; i++){
            if(bool_consumidor[i] == qtd_produtoras){

            }
        }
    }

    // for(long i = 0; i < threads_define.num_threads_produtoras; i++){
    //     pthread_join(produtoras[i], NULL);
    // }

    // for(long i = 0; i < threads_define.num_threads_consumidoras; i++){
    //     pthread_join(consumidoras[i], NULL);
    // }
    
    return 0;
}

ThreadsDefine verifica_args(int argc, char *argv[], int print_success){
if (argc != 3){
        printf("%sIndique o número de threads produtoras e consumidoras!\nO programa deve ser executado seguindo o padrão ./main --4 --5, em que o segundo argumento é o número de threads produtoras \ne o terceiro o número de threads consumidoras\n%s", COR_VERMELHA, COR_RESET);
        exit(-1);
    }

    int num_threads_produtoras = 0, num_threads_consumidoras = 0;

    if (strlen(argv[1]) < 2 && strncmp(argv[1], "--", 2)){
        printf("%sOs argumentos devem ser passados no formato --Número!%s\n", COR_VERMELHA, COR_RESET);
        exit(-1);
    }
    strcpy(argv[1], argv[1] + 2);

    num_threads_produtoras = atoi(argv[1]);

    if (strlen(argv[2]) < 2 && strncmp(argv[2], "--",2 )){
        printf("%sOs argumentos devem ser passados no formato --Número!%s\n", COR_VERMELHA, COR_RESET);
        exit(-1);
    }

    strcpy(argv[2], argv[2] + 2);

    num_threads_consumidoras = atoi(argv[2]);

    if (print_success){
        printf("Número de Threads Produtoras: %d\nNúmero de Threads Consumidoras: %d\n", num_threads_produtoras, num_threads_consumidoras);
    }

    ThreadsDefine threads_define;
    threads_define.num_threads_produtoras = num_threads_produtoras;
    threads_define.num_threads_consumidoras = num_threads_consumidoras;

    return threads_define;
}