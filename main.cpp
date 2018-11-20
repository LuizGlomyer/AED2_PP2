#include <iostream>
#include <limits>
#include "ListaLigada.hpp"
#include "Fila.hpp"
#include "Pilha.hpp"
#include "Grafo.hpp"


int main(){
    int ordemCerebro, numSinapses;
    cin >> ordemCerebro;
    cin >> numSinapses;
    Grafo_MA cerebro(ordemCerebro);

    for(int i = 1; i <= numSinapses; i++){ //cada aresta do cerebro
        int neuronio1, neuronio2; float distancia;
        cin >> neuronio1;
        cin >> neuronio2;
        cin >> distancia;
        cerebro.inserirAresta(neuronio1, neuronio2, distancia);
    }
    int blocoEntrada, blocoSaida;
    cin >> blocoEntrada;
    cin >> blocoSaida;

    Grafo_MA** blocosNeuronios = new Grafo_MA*[ordemCerebro+1];
    for(int i = 1; i <= ordemCerebro; i++){ //cada subgrafo/bloco de neuronio
        int ordemBloco, numSinapses, qtdDoentes;
        cin >> ordemBloco;
        cin >> numSinapses;
        cin >> qtdDoentes;

        blocosNeuronios[i] = new Grafo_MA(ordemBloco); //marcar os neuronios que sao doentes dentro dos blocos
        for(int j = 0; j < qtdDoentes; j++){
            int neuronioDoente;
            cin >> neuronioDoente;
            blocosNeuronios[i]->marcarDoente(neuronioDoente); //marcar os neuronios dentro de um bloco  que estão doentes
            cerebro.marcarDoente(i); //diz que o bloco i está doente, ou seja, pelo menos um dos neuronios la dentro está doente
        }

        for(int j = 1; j <= numSinapses; j++){ //ligações entre neuronios dentro de um bloco
            int sinapse1, sinapse2; float distancia;
            cin >> sinapse1;
            cin >> sinapse2;
            cin >> distancia;
            blocosNeuronios[i]->inserirAresta(sinapse1, sinapse2, distancia);
        }

    }

    Dijkstra_MA calcular;
    calcular.menorCaminho(cerebro, blocoEntrada); //calcula a distancia do bloco de entrada até todos os outros blocos
    calcular.mostrarDistancia();

    Pilha<Vertice> blocosVisitados; //colocar todo o percurso percorrido numa pilha
    int blocoAtual = blocoSaida;
    blocosVisitados.adicionar(blocoAtual);
    while(blocoAtual != blocoEntrada){
        blocoAtual = calcular.getPredecessor()[blocoAtual];
        blocosVisitados.adicionar(blocoAtual);
    }
    //blocosVisitados.adicionar(blocoEntrada); //o bloco de entrada aparentemente não é calculado

    float somatorio = 0;
    while(!blocosVisitados.estaVazia()){
        MST_MA calcularArvore = MST_MA();
        Vertice u = blocosVisitados.pop();
        if(blocosNeuronios[u]->estaDoente()){
            calcularArvore.MST_Prim(*blocosNeuronios[u], 1);
            somatorio += calcularArvore.getSomatorio(*blocosNeuronios[u]);
        }

    }
    cout << "\n\n" << somatorio;


    //desoloca memoria

    return 0;
}
