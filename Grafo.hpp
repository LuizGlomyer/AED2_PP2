#ifndef GRAFO_HPP_INCLUDED
#define GRAFO_HPP_INCLUDED

#include "ListaLigada.hpp"
#include "Fila.hpp"

typedef int Vertice;
typedef int Peso;
enum Cor {Branco, Cinza, Preto};
enum Status {Sadio, Doente};


//Grafo por matriz de adjacências
class Grafo_MA{
public:
    Grafo_MA(int);
    ~Grafo_MA();
    void inserirAresta(Vertice, Vertice, Peso = 0);
    void inserirArestaDirecionada(Vertice, Vertice, Peso = 0);
    void removerAresta(Vertice, Vertice);
    void mostrar();
    void reinicializar(int);
    void marcarDoente(Vertice);
    bool estaDoente() const;
    int getTamanho();
    int** getMatriz() const;
    float** getPesos() const;
    Status* getStatus() const;
private:
    Status* status;
    void inicializar(int);
    void destruir();
    int n, m;
    int** mat;
    float** pesos;
};


class MST_MA{
public:
    MST_MA();
    ~MST_MA();
    void destruir();
    void reinicializar();
    void MST_Prim(Grafo_MA&, Vertice);
    void MST_Kruskal(Grafo_MA&);
    void mostrar(Grafo_MA&);
    void mostrarSomatorio(Grafo_MA&);
    float getSomatorio(Grafo_MA& G) const;
private:
    int* chave;
    Vertice* pai;
    FilaPrioridades<int> fila;
};



class Dijkstra_MA{
public:
    Dijkstra_MA();
    ~Dijkstra_MA();
    void menorCaminho(Grafo_MA&, Vertice);
    void relaxa(Vertice, Vertice);
    void mostrarDistancia();
    int* getPredecessor() const;
private:
    int raiz;
    int n;
    float** peso;
    int* pai;
    int* distancia;
    FilaPrioridades<int> fila;
};


#endif // GRAFO_HPP_INCLUDED
