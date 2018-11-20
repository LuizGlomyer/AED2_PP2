#ifndef PILHA_HPP_INCLUDED
#define PILHA_HPP_INCLUDED

#include "ELEMENTO.hpp"

template<typename T>
class Pilha{
public:
    Pilha();
    ~Pilha();
    void adicionar(T);
    void remover();
    T pop();
    void esvaziar();
    bool estaVazia() const;
    void mostrarPilha() const;
private:
    ELEMENTO<T>* topo;
};

template<typename T>
Pilha<T>::Pilha(){
    topo = nullptr;
}

template<typename T>
Pilha<T>::~Pilha(){
    esvaziar();
}

template<typename T>
void Pilha<T>::adicionar(T dado){
    if(topo == nullptr){
        topo = new ELEMENTO<T>;
        topo->dados = dado;
        topo->prox = nullptr;
        return;
    }
    ELEMENTO<T>* e = topo;
    topo = new ELEMENTO<T>;
    topo->dados = dado;
    topo->prox = e;
}

template<typename T>
void Pilha<T>::remover(){
    if(topo == nullptr)
        return;
    if(topo->prox == nullptr){
        delete topo;
        topo = nullptr;
        return;
    }
    ELEMENTO<T>* e = topo->prox;
    delete topo;
    topo = e;
    return;
}

template<typename T>
T Pilha<T>::pop(){
    T retorno = topo->dados;
    ELEMENTO<T>* e = topo->prox;
    delete topo;
    topo = e;
    return retorno;
}

template<typename T>
void Pilha<T>::esvaziar(){
    ELEMENTO<T>* e = topo,* excluir;
    while(e != nullptr){
        excluir = e;
        e = e->prox;
        delete excluir;
    }
}

template<typename T>
bool Pilha<T>::estaVazia() const{
    return topo == nullptr ? true : false;
}

template<typename T>
void Pilha<T>::mostrarPilha() const{
    int i = 0;
    ELEMENTO<T>* e = topo;
    while(e != nullptr){
        cout << "Elemento " << i << " : \t" << e->dados << endl;
        e = e->prox;
        i++;
    }
}

#endif // PILHA_HPP_INCLUDED
