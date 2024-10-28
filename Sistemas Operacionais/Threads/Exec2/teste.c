#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "cores.h"

#define BUFFER_SIZE 12 //Tamanho do buffer
#define NUM_PALAVRAS 4 //Número de Palavras e de Frases a serem escolhidas

char *buffer[BUFFER_SIZE]; //Criação do Buffer gerenciado pela main
int front = 0, rear = 0; //variáveis de controle do buffer

int consumidor_indica[BUFFER_SIZE]; //Array pra indicar se cada uma das palavras foi lidas por todos os consumidores

int qtd_palavras = 0; //Quantidade de palavras já enviadas ao buffer

int qtd_produtoras; //Qtd de threads produtoras
int qtd_consumidoras; //Qtd de threads consumidoras

int contador_produtoras = 0; //Qtd de threads produtoras que já finalizaram
int contador_consumidores = 0; //Qtd de threads consumidoras que já finalizaram


char* frases[NUM_PALAVRAS] = {
    "Arsenal Liverpool Manchester City",
    "Manchester United Everton Leicester",
    "Wolverhampton Luton Town Burnley",
    "Olá Meu Nome é",
}; //Palavras que serão escolhidas

typedef struct{
    int num_threads_produtoras;
    int num_threads_consumidoras;
}ThreadsDefine; //Abstrai isso aqui. É só pras flags na hora de execução. Não impacta mt o código n


ThreadsDefine verifica_args(int argc, char *argv[], int print_sucess); //Função pra verificar os argumentos enviados na hora de executar

//Thread do produtor
void* produtor(void* args){
    long id = (long)args; //Pega o id do produtor

    int index_palavra = rand() % NUM_PALAVRAS; //Sorteia uma das frases
    char *mensagem = strdup(frases[index_palavra]); //Pega essa frase

    printf("Mensagem a ser enviada pela thread %ld produtora: %s\n", id, mensagem);

    char* palavra = strtok(mensagem, " "); //Dá basicamente um strip na palavra
    while(palavra){ //Enquanto ainda estiverem palavras na frase (É como se tivesse ficado um array de palavras ["oi", "teste"], e ele fosse iterando sobre esse array. Não é na prática assim mas é mais fácil de entender até pq eu tbm abstrai mt essa função)
        while( (rear + 1) % BUFFER_SIZE == front){ //Enquanto não poder escrever no buffer 
        };
        buffer[rear] = palavra; //Colocar a palavra atual da frase no buffer
        rear = (rear + 1) % BUFFER_SIZE; //Atualizar a escrita no buffer
        palavra = strtok(NULL, " "); //Pegar a próxima palavra a ser escrita
        sleep(1); //sleep
    }

    contador_produtoras++; //Atualiza a quantidade de produtoras que terminaram
    pthread_exit(NULL);

}   

void *consumidor(void* args){
    long id = (long)args; //Pega id

    pthread_t tid = pthread_self();

    while(qtd_palavras < NUM_PALAVRAS*qtd_produtoras){
        while(front == rear){}; //Enquanto o buffer estiver vazio

        char* mensagem = buffer[front]; //Pegar a mensagem do buffer
        printf("%lu - Thread %ld consumidora: %s\n", tid, id, mensagem); 
        front = (front + 1) % BUFFER_SIZE; //Atualizar o acesso ao buffer
        sleep(1);
    }

    contador_consumidores++; //Atualizar qtd de consumidores que terminaram
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    ThreadsDefine threads_define;
    threads_define = verifica_args(argc, argv, 0); //Definição do número de threads que veio dos args

    pthread_t produtoras[threads_define.num_threads_produtoras]; //Definição das threads produtoras
    pthread_t consumidoras[threads_define.num_threads_consumidoras]; //Definição das threads consumidoras

    time_t t;
    srand(time(&t));

    qtd_produtoras = threads_define.num_threads_produtoras;
    qtd_consumidoras = threads_define.num_threads_consumidoras;

    for(long i = 0; i < threads_define.num_threads_produtoras; i++){ //Criação das threads produtoras
        pthread_create(&produtoras[i], NULL, produtor, (void*)i);
    }

    for(long i = 0; i < threads_define.num_threads_consumidoras; i++){ //Criação das threads consumidoras
        pthread_create(&consumidoras[i], NULL, consumidor, (void*)i);
    }

    //Mexendo no buffer com a thread main
    //Enquanto supostamente não tiver tudo terminado
    while(contador_consumidores < threads_define.num_threads_consumidoras && contador_produtoras < threads_define.num_threads_produtoras){
        if((rear + 1) % BUFFER_SIZE == front){ //Se o buffer tiver cheio
            for(int i = 0; i < BUFFER_SIZE; i++){
                if(consumidor_indica[i] < qtd_produtoras){ //Se uma das consumidoras n tiver lido uma das palavras
                    break;                    
                }

                if(i == BUFFER_SIZE - 1){ //Se todas as consumidoras leram todas as palavras
                    //Resetar buffer
                    rear = 0;
                    front = 0;
                }
            }
        }
    }
    
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