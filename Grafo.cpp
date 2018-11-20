#include "Grafo.hpp"
#include <iostream>
#include <limits>
using namespace std;


Grafo_MA::Grafo_MA(int numVertices){
    inicializar(numVertices);
}

Grafo_MA::~Grafo_MA(){
    destruir();
}

void Grafo_MA::inicializar(int numVertices){
    n = numVertices;
    status = new Status[n+1];
    mat = new int*[n+1];
    mat[0] = nullptr;
    for(int i = 1; i <= n; i++){
        mat[i] = new int[n+1];
        for(int j = 1; j <= n; j++)
            mat[i][j] = 0;
    }
    pesos = new float*[n+1];
    for(int i = 1; i <= n; i++){
        pesos[i] = new float[n+1];
        for(int j = 1; j <= n; j++)
            pesos[i][j] = -1;
    }
}

void Grafo_MA::destruir(){
    for(int i = 1; i <= n; i++){
        delete [] mat[i];
        delete [] pesos[i];
    }

    delete [] mat;
    delete [] pesos;
    delete [] status;
}

void Grafo_MA::reinicializar(int numVertices){
    destruir();
    inicializar(numVertices);
}

void Grafo_MA::inserirAresta(Vertice u, Vertice v, Peso p){
    mat[u][v]++;
    mat[v][u]++;/*
    if(pesos[u][v] != -1 || pesos[v][u] != -1){ //o peso já existir é um erro, estamos trabalhando com grafos simples
        cout << "ERRO";
        exit(123);
    }*/
    pesos[u][v] = p;
    pesos[v][u] = p;
    m++;
}

void Grafo_MA::inserirArestaDirecionada(Vertice u, Vertice v, Peso p){
    mat[u][v]++;
    if(pesos[u][v] != -1){ //o peso já existir é um erro, estamos trabalhando com grafos simples
        cout << "ERRO";
        exit(123);
    }
    pesos[u][v] = p;
    m++;
}

void Grafo_MA::removerAresta(Vertice u, Vertice v){
    mat[u][v]--;
    mat[v][u]--;
    m--;
}

void Grafo_MA::marcarDoente(Vertice u){
    status[u] = Doente;
}

bool Grafo_MA::estaDoente() const{
    for(int i = 1; i <= n; i++)
        if(status[i] == Doente)
            return true;
    return false;
}

void Grafo_MA::mostrar(){
//    for(int i = 0; i <= n; i++)
    cout << '\t';
    for(int i = 1; i<= n; i++)
        cout << '[' << i << ']' << '\t';
    cout << '\n';

    for(int i = 1; i <= n; i++){
        cout << '[' << i << ']' << '\t';
        for(int j = 1; j <= n; j++)
            cout << mat[i][j] << '\t';
        cout << '\n';
    }
}

int Grafo_MA::getTamanho(){
    return n;
}

int** Grafo_MA::getMatriz() const{
    return mat;
}

float** Grafo_MA::getPesos() const{
    return pesos;
}





















MST_MA::MST_MA(){
    chave = pai = nullptr;
    //fila = nullptr;
}

MST_MA::~MST_MA(){
    destruir();
}

void MST_MA::destruir(){
    if(chave != nullptr)
        delete [] chave;
    if(pai != nullptr)
        delete [] pai;
    //if(fila != nullptr)
     //   delete fila;
}

void MST_MA::reinicializar(){
    destruir();
    pai = chave = nullptr;
    //fila = nullptr;
}

void MST_MA::MST_Prim(Grafo_MA& G, Vertice r){
    int n = G.getTamanho();
    chave = new int[n+1];
    pai = new int[n+1];
    //fila = new FilaPrioridades(n);
    FilaPrioridades<int> fila;
    for(int i = 1; i <= n; i++){
        chave[i] = INT_MAX;
        pai[i] = -1;
    }
    chave[r] = 0;
    for(int i = 1; i <= n; i++)
        fila.inserir(i, chave[i]);
    int** a = G.getMatriz();
    while(!fila.estaVazia()){
        Vertice u = fila.extrairElementoMinimo();
        for(Vertice v = 1; v <= n; v++)
            if(a[u][v] != 0)
                if(fila.estaNaFila(v) && G.getPesos()[u][v] < chave[v]){
                    pai[v] = u;
                    chave[v] = G.getPesos()[u][v];
                    fila.atualizarPrioridade(v, chave[v]);
                }

    }
}



void MST_MA::mostrar(Grafo_MA& G){
    int n = G.getTamanho();
    for(int i = 1; i <= n; i++){
        cout << "Vertice: " << i << '\n';
        if(pai[i] == -1){
            cout << "Este vertice eh a raiz" << "\n\n";
            continue;
        }
        cout << "Pai: " << pai[i] << '\n';
        cout << "Peso: " << G.getPesos()[i][pai[i]] << '\n';
        cout << '\n';
    }
    mostrarSomatorio(G);
}

void MST_MA::mostrarSomatorio(Grafo_MA& G){
    int n = G.getTamanho();
    int soma = 0;
    for(int i = 1; i <= n; i++)
        soma += chave[i];
    cout << "O somatorio das chaves eh: " << soma << '\n';
}

float MST_MA::getSomatorio(Grafo_MA& G) const{
    int n = G.getTamanho();
    int soma = 0;
    for(int i = 1; i <= n; i++)
        soma += chave[i];
    return soma;
}







Dijkstra_MA::Dijkstra_MA(){
    pai = distancia = nullptr;
    peso = nullptr;;
    //fila = nullptr;
}

Dijkstra_MA::~Dijkstra_MA(){
    if(pai != nullptr)
        delete [] pai;
    if(distancia != nullptr)
        delete [] distancia;
    //if(fila != nullptr)
    //    delete fila;
}

int* Dijkstra_MA::getPredecessor() const{
    return pai;
}

void Dijkstra_MA::menorCaminho(Grafo_MA& G, Vertice r){
    raiz = r;
    n = G.getTamanho();
    peso = G.getPesos();
    pai = new int[n+1];
    distancia = new int[n+1];
    int** a = G.getMatriz();
    //fila = new FilaPrioridades(n);
    FilaPrioridades<int> fila;
    for(Vertice u = 1; u <= n; u++){
        pai[u] = -1;
        distancia[u] = INT_MAX;
    }
    distancia[r] = 0;
    for(Vertice u = 1; u <= n; u++)
        fila.inserir(u, distancia[u]);

    while(!fila.estaVazia()){
        Vertice u = fila.extrairElementoMinimo();
        for(Vertice v = 1; v <= n; v++){
            if(a[u][v] != 0){
            //relaxa(u, a[v]); por algum motivo não funciona -  as filas aparecem vazias dentro do método
                int m = u, n = v;
                if(distancia[n] > distancia[m] + peso[m][n]){
                    distancia[n] = distancia[m] + peso[m][n];
                    fila.atualizarPrioridade(n, distancia[n]);
                    pai[n] = m;
                }
            }
        }
    }
}


void Dijkstra_MA::relaxa(Vertice u, Vertice v){
    int a = distancia[v], b = distancia[u] + peso[u][v];
    if(distancia[v] > distancia[u] + peso[u][v]){
        distancia[v] = distancia[u] + peso[u][v];
        fila.atualizarPrioridade(v, distancia[v]);
        pai[v] = u;
    }
}

void Dijkstra_MA::mostrarDistancia(){
    cout << "Raiz: " << raiz << '\n';
    for(int i = 1; i <= n; i++){
        cout << "Distancia ate o vertice " << i << ": " << distancia[i] << '\n';
    }
    cout << '\n';
}
