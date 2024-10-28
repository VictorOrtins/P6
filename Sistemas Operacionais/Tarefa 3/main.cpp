#include <iostream>

#include <string>
#include <bits/stdc++.h>

#include <unistd.h>
#include <thread>
#include <vector>
#include <list>

#include <semaphore.h>

using namespace std;

#define QTD_CONSUMIDORES 4
#define QTD_PRODUTORES 4

sem_t mutexBuffer;
sem_t mutexThreadsProds;
sem_t mutexThreadsCons;

list<string> buffer;
bool threadConLeu[QTD_CONSUMIDORES];

const vector<string> frases{"Arsenal Liverpool Manchester City", "Aston Villa Tottenham Manchester United",
                            "West Ham Brighton Wolves", "Newcastle Chelsea Fulham Bournemouth", "Crystal Palace Brentford Everton",
                            "Nottingham Forest Luton Town Burnley Sheffield United"};


int threads_prods_terminadas = 0;
int threads_cons_terminadas = 0;


void producer(int id){
    int index_frase = rand() % frases.size();
    string frase = frases.at(index_frase);

    stringstream ss(frase);

    std::string token;

    printf("--- Produtor de id %d vai mandar frase completa: %s ---\n\n", id, frase.c_str());


    while(ss >> token){
        printf("Produtor de id %d: %s\n\n", id, token.c_str());

        sem_wait(&mutexBuffer);
            buffer.push_back(token);
        sem_post(&mutexBuffer);

        sleep(1);
    }
    
    sem_wait(&mutexThreadsProds);
        threads_prods_terminadas++;
    sem_post(&mutexThreadsProds);

}

void consumer(int id){
    printf("--- Consumidor de id %d vai começar a ler --- \n\n", id);

    while(threads_prods_terminadas < QTD_PRODUTORES || !buffer.empty()){
        
        sem_wait(&mutexBuffer);

            if (threadConLeu[id] == true){
                continue;
            }

            string palavra = buffer.front();
            threadConLeu[id] = true;
            
        sem_post(&mutexBuffer);

        printf("Consumidor de id %d: %s \n\n", id, palavra.c_str());
    }

    sem_wait(&mutexThreadsCons);
        threads_cons_terminadas++;
    sem_post(&mutexThreadsCons);
}



int main(void){

    vector<thread> producers;
    vector<thread> consumers;

    sem_init(&mutexBuffer, 0, 1); //Iniciando o semáforo (que funcionará com um mutex) com 1 para o buffer
    sem_init(&mutexThreadsProds, 0, 1);
    sem_init(&mutexThreadsCons, 0, 1);


    for(int i = 0; i < QTD_PRODUTORES; i++){
        cout << " /// Thread produtora de id " << i << " iniciando ///\n\n";
        thread prod(producer, i);
        producers.emplace_back(std::move(prod));
    }

    for(int i = 0; i < QTD_CONSUMIDORES; i++){
        cout << " /// Thread consumidora de id " << i << " iniciando ///\n\n";
        thread cons(consumer, i);
        producers.emplace_back(std::move(cons));
    }

    while(threads_prods_terminadas < QTD_PRODUTORES || threads_cons_terminadas < QTD_CONSUMIDORES){
        sem_wait(&mutexBuffer);
            bool tem_thread_faltando = false; 
            for(int i = 0; i < QTD_CONSUMIDORES; i++){
                if (threadConLeu[i] == false){
                    tem_thread_faltando = true;
                    break;
                }
            }

            if (tem_thread_faltando){
                sem_post(&mutexBuffer);
                continue;
            }

            for(int i = 0; i < QTD_CONSUMIDORES; i++){
                threadConLeu[i]  = false;
            }
            buffer.pop_front();
        sem_post(&mutexBuffer);
    }


    for (thread &prod: producers){
        prod.join();
    }

    for (thread &cons: consumers){
        cons.join();
    }

    return 0;
}

