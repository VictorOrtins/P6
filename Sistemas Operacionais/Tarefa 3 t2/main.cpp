#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <bits/stdc++.h>

#include <semaphore.h>
#include <unistd.h>

using namespace std;

const int QTD_THREADS_CONSUMIDORAS = 14; //Quantidade de threads consumidoras
const int QTD_THREADS_PRODUTORAS = 16; //Quantidade de threads produtoras

const vector<string> frases{"Arsenal Liverpool Manchester City", "Aston Villa Tottenham Manchester United",
                            "West Ham Brighton Wolves", "Newcastle Chelsea Fulham Bournemouth", 
                            "Crystal Palace Brentford Everton",
                            "Nottingham Forest Luton Town Burnley Sheffield United"};
                            //Frases a serem enviadas (tabela da Premier League quando enviei a primeira atv)

vector<string> buffer; //Vector que serve como um buffer
bool consumidor_leu[QTD_THREADS_CONSUMIDORAS]; //Vetor para indicar qual consumidor leu a mensagem atual (a primeira) do buffer

sem_t mutex_buffer; //Mutex para o acesso ao buffer
sem_t mutex_leu; //Mutex para o acesso a variável consumidor_leu

void producer(int i){
    int index_frase = i % frases.size(); //Pega a frase que será mandada pelo produtor
    //Como a quantidade de threads produtoras pode ser maior que a quantidade de frases possíveis, utilizo
    //o mod para garantir que sempre será pego alguma frase válida no vector

    string frase = frases[index_frase]; //Pegar a frase

    stringstream ss(frase); //Inicializando varíavel que fará basicamente o split da mensagem em palavras

    string token; //Token que armazenará cada palavra da mensagem

    int contador = 0; //Inicialização do contador para saber se será feito um push bakc no buffer
    //Ou se será apenas alterado a string no buffer


    sem_wait(&mutex_buffer); //Mutex para garantir que o buffer está apenas sendo acessado por uma thread
    //Ele é colocado mesmo antes do while para impedir que as mensagens das threada fiquem embaralhadas
    cout << "Thread produtora de id " << i << " enviará mensagem: " << frase << "\n";

    while(ss >> token){ //Enquanto ainda houver palavras na stringstream
        if (contador == 0){ //Se for a primeira palavra
            cout << "Thread produtora de id " << i << " colocou no buffer: " << token << "\n";
            buffer.push_back(token); //Push back no buffer (nova mensagem está sendo inserida)
            contador++; //Indicando que a primeira palavra já foi
            continue;
        }
        cout << "Thread produtora de id " << i << " colocou no buffer: " << token << "\n";

        buffer.at(buffer.size() - 1) = buffer.at(buffer.size() - 1) + ' ' + token; //Pegando a última palavra
        //adicionada (que foi adicionada por essa thread) e adicionada a próxima palavra
        sleep(1);
    }
    sem_post(&mutex_buffer); //Liberando a seção crítica
}

void consumer(int i){
    int id = gettid(); //Pegando o thread id

    while(!buffer.empty()){ //Enquanto ainda existirem palavras a serem lidas
        sem_wait(&mutex_leu); //Entrando na seção crítica da variável consumidor_leu

        bool terminou = true; //Se todos os consumidores leram, terminou
        for(int i = 0; i < QTD_THREADS_CONSUMIDORAS; i++){
            if(consumidor_leu[i] == false){ //Se algum consumidor não leu
                terminou = false; //Não terminou
                break;
            }
        }

        if(terminou){ //Se terminou
            sem_wait(&mutex_buffer); //Entrando na seção crítica do buffer
            //Ou seja, não dá pra dar esse erase se algum produtor estiver produzindo
            //Ou se algum consumidor estiver consumindo
            buffer.erase(buffer.begin()); //Apaga a primeira mensagem do buffer
            sem_post(&mutex_buffer); //Saindo da seção crítica do buffer

            //Além disso, setando todos os consumidor_leu para falso a fim de liberar os consumidores
            for(int i = 0; i < QTD_THREADS_CONSUMIDORAS; i++){
                consumidor_leu[i] = false;
            }
        }
        else{
            if(consumidor_leu[i] == true){ //Se o consumidor atual já leu
                sem_post(&mutex_leu); //Libera o mutex da variável consumidor_leu
                continue; //Não vai conseguir ler a mensagem no buffer
            }
        }
        sem_post(&mutex_leu); //Liberando o mutex de consumidor_leu

        sem_wait(&mutex_buffer); //Entrando na seção crítica do buffer
        if(!buffer.empty()){ //Se o estiver vazio
            string mensagem = buffer.front(); //Pegando a primeira mensagem do buffer
            cout << "Thread de tid " << id << " leu mensagem: " << mensagem << "\n";

            sem_wait(&mutex_leu); //Entrando na seção crítica de consumidor_leu
            //Isso garante que não vai haver nenhuma thread que está consumindo do buffer alterando
            //a variável consumidor_leu enquanto algum consumidor está checando se todas as threads já leram do
            //buffer
            consumidor_leu[i] = true;
            sem_post(&mutex_leu); //Saindo da seção crítica de consumidor_leu

            sleep(1);
        }

        sem_post(&mutex_buffer); //Saindo da seção crítica do buffer

    }
}

int main(void){
   vector<thread> threads_produtoras; //Criação do vector de threads produtoras
   vector<thread> threads_consumidoras; //Criação do vector de threads consumidoras

   //Como os 2 são mutexes, preciso inicializá-los com 1, para que o acesso seja mutualmente exclusivo
   sem_init(&mutex_buffer, 0, 1); //Inicialização do mutex do buffer
   sem_init(&mutex_leu, 0, 1); //Inicialização do mutex do consumidor_leu

   for(int i = 0; i < QTD_THREADS_PRODUTORAS; i++){
        std::cout << "Thread produtora de id " << i << " iniciando\n";
        thread produtora(producer, i); //Início da thread produtora
        threads_produtoras.emplace_back(std::move(produtora)); //Colocando no vector de threads produtoras
   }

   for(int i = 0; i < QTD_THREADS_CONSUMIDORAS; i++){
        consumidor_leu[i] = false; //Inicializando todos os consumidores como não tendo lido a última msg
        //O mutex não é necessário ainda já que nenhuma thread produtora vai mexer com essas variáveis e as threads
        //Consumidoras ainda não foram inicializadas
   }

   for(int i = 0; i < QTD_THREADS_CONSUMIDORAS; i++){
        std::cout << "Thread consumidora de id " << i << " iniciando\n";
        thread consumidora(consumer, i); //Iniciando as threads consumidoras
        threads_consumidoras.emplace_back(std::move(consumidora)); //Colocando no vector de threads consumidoras
   }

    for(auto &thread_prod: threads_produtoras){
        thread_prod.join(); //Join nas threads produtoras
    }

    for(auto &thread_cons : threads_consumidoras){
        thread_cons.join(); //Join nas threads consumidoras
    }

}