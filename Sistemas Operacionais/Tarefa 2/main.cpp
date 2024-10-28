#include <iostream>
#include <string>
#include <bits/stdc++.h>

#include <queue>
#include <vector>

#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

#define NUM_THREADS_PRODUTORAS 4
#define NUM_THREADS_CONSUMIDORAS 4


queue<string> buffer;
const vector<string> frases{"Arsenal Liverpool Manchester City", "Aston Villa Tottenham Manchester United",
                            "West Ham Brighton Wolves", "Newcastle Chelsea Fulham Bournemouth", "Crystal Palace Brentford Everton",
                            "Nottingham Forest Luton Town Burnley Sheffield United"};


vector<bool> consumidor_pode_ler(NUM_THREADS_CONSUMIDORAS, true);

int qtd_leitura_threads = 0;

int finalizaram_produtor = 0;
int finalizaram_consumidor = 0;


void* produtor(void *args){
    long id = (long)args;

    // int index_frase = rand() % frases.size();

    string frase = frases.at(id);

    stringstream ss(frase);

    std::string token;

    printf("--- Produtor de id %ld vai mandar frase completa: %s ---\n", id, frase.c_str());


    while(ss >> token){
        printf("Produtor de id %ld: %s\n", id, token.c_str());

        buffer.push(token);

        sleep(1);
    }

    finalizaram_produtor++;
    pthread_exit(NULL);
}

void* consumidor(void *args){
    long id = (long)args;

    int id_thread = pthread_self();
    printf("--- Consumidor de id %d vai começar a ler --- \n", id_thread);

    while(1){
        if(consumidor_pode_ler.at(id) == false){
            continue;
        }

        string palavra = buffer.front();
        qtd_leitura_threads++;
        consumidor_pode_ler.at(id) = false;

        
        printf("--- Consumidor de id %d: %s --- \n", id_thread, palavra.c_str());

    }

    finalizaram_consumidor++;
    pthread_exit(NULL);
}


int main(void){

    pthread_t threads_produtoras[NUM_THREADS_PRODUTORAS];
    pthread_t threads_consumidoras[NUM_THREADS_CONSUMIDORAS];

    time_t t;
    srand(time(&t));

    for(long i = 0; i < NUM_THREADS_PRODUTORAS; i++){
        pthread_create(&threads_produtoras[i], NULL, &produtor, (void*)i);
    }

    for(long i = 0; i < NUM_THREADS_CONSUMIDORAS; i++){
        pthread_create(&threads_consumidoras[i], NULL, &consumidor, (void*)i);
    }

    while(finalizaram_produtor <= NUM_THREADS_PRODUTORAS && finalizaram_consumidor <= NUM_THREADS_CONSUMIDORAS){
        // printf("teste");
        if(qtd_leitura_threads >= NUM_THREADS_CONSUMIDORAS){
            qtd_leitura_threads = 0;
            buffer.pop();
            fill(consumidor_pode_ler.begin(), consumidor_pode_ler.end(), true);
        }
    }


    // for(int i = 0; i < NUM_THREADS_PRODUTORAS; i++){
    //     pthread_join(threads_produtoras[i], NULL);
    // }

    // for(int i = 0; i < NUM_THREADS_CONSUMIDORAS; i++){
    //     pthread_join(threads_consumidoras[i], NULL);
    // }



    return 0;
}