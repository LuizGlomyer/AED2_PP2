#ifndef FILA_HPP_INCLUDED
#define FILA_HPP_INCLUDED

#include "ELEMENTO.hpp"
#include "ListaLigada.hpp"


/*
struct PRIORIDADE{
    int vertice;
    int prioridade;
};

class FilaPrioridades{
private:
public:
    FilaPrioridades(int tam){
        elemento = new PRIORIDADE[tam];
        qtd = 0;
    }

    ~FilaPrioridades(){
        delete [] elemento;
    }

    PRIORIDADE* elemento;
    int qtd;

    void inserir(int vertice, int prioridade){
        elemento[qtd].vertice = vertice;
        elemento[qtd].prioridade = prioridade;
        qtd++;
    }
    bool estaVazia(){
        return qtd == 0 ? true : false;
    }

    bool estaNaFila(int n){
        for(int i = 0; i < qtd; i++)
            if(elemento[i].vertice == n)
                return true;
        return false;
    }

    PRIORIDADE extrairMinimo(){
        if(qtd == 0)
            exit(123);
        int min = 0; //pos
        for(int i = 0; i < qtd; i++)
            if(elemento[i].prioridade < elemento[min].prioridade)
                min = i;

        PRIORIDADE a;
        a.prioridade = elemento[min].prioridade;
        a.vertice = elemento[min].vertice;

        for(int i = min; i < qtd-1; i++){
            elemento[i] = elemento[i+1];
        }
        qtd--;
        return a;
    }
    void atualizarPrioridade(int v, int novaPrio){
        for(int i = 0; i < qtd; i++)
            if(elemento[i].vertice == v){
                elemento[i].prioridade = novaPrio;
                return;
            }
    }
};*/


template<typename T>
class Fila{
    public:
        Fila();
        ~Fila();
        void adicionar(T);
        void remover();
        T extrairMinimo();
        T pop();
        bool estaVazia() const;
        bool estaNaFila(T) const;
        void esvaziar();
        void mostrarFila() const;
    private:
        ELEMENTO<T>* primeiro,* ultimo;
};


template<typename T>
class FilaPrioridades{
public:
    FilaPrioridades();
    ~FilaPrioridades();
    void inserir(T, int);
    bool estaVazia() const;
    bool estaNaFila(T) const;
    void atualizarPrioridade(T, int);
    T extrairElementoMinimo();
private:
    ListaLigada<T> elemento;
    ListaLigada<int> chave;
};

template<typename T>
FilaPrioridades<T>::FilaPrioridades(){

}

template<typename T>
FilaPrioridades<T>::~FilaPrioridades(){

}

template<typename T>
void FilaPrioridades<T>::inserir(T elem, int prioridade){
    elemento.inserir(elem);
    chave.inserir(prioridade);
}

template<typename T>
void FilaPrioridades<T>::atualizarPrioridade(T elem, int novaPrioridade){
    int a = this->chave.getTamanho()+1;
    for(int i = 0; i < elemento.getTamanho()+1; i++)
        if(elemento[i] == elem){
            chave[i] = novaPrioridade;
            return;
        }
}

template<typename T>
T FilaPrioridades<T>::extrairElementoMinimo(){
    if(elemento.getTamanho()+1 == 0)
        exit(123);
    int min = 0;
    for(int i = 0; i < elemento.getTamanho()+1; i++)
        if(chave[min] > chave[i])
            min = i;
    T retorno = elemento[min];
    elemento.remover(min);
    chave.remover(min);
    return retorno;
}


template<typename T>
bool FilaPrioridades<T>::estaVazia() const{
    return elemento.estaVazia();
}

template<typename T>
bool FilaPrioridades<T>::estaNaFila(T dado) const{
    return elemento.estaNaLista(dado);
}






template<typename T>
Fila<T>::Fila(){
    primeiro = nullptr;
    ultimo = nullptr;
}

template<typename T>
Fila<T>::~Fila(){
    esvaziar();
}

template<typename T>
void Fila<T>::adicionar(T dado){
    if(primeiro == nullptr){
        primeiro = new ELEMENTO<T>;
        primeiro->dados = dado;
        primeiro->prox = nullptr;
        ultimo = primeiro;
        return;
    }
    ELEMENTO<T>* e = ultimo;
    e->prox = new ELEMENTO<T>;
    e->prox->dados = dado;
    e->prox->prox = nullptr;
    ultimo = e->prox;
}

template<typename T>
void Fila<T>::remover(){
    if(primeiro == nullptr){
        return;
    }
    if(primeiro == ultimo){
        delete primeiro;
        primeiro = ultimo = nullptr;
        return;
    }
    ELEMENTO<T>* e = primeiro->prox;
    delete primeiro;
    primeiro = e;
}

template<typename T>
T Fila<T>::extrairMinimo(){

}


template<typename T>
T Fila<T>::pop(){
    if(primeiro == nullptr){
        throw bad_alloc();
        //return * new T;
    }
    if(primeiro == ultimo){
        T dado = primeiro->dados;
        delete primeiro;
        primeiro = ultimo = nullptr;
        return dado;
    }
    T dado = primeiro->dados;
    ELEMENTO<T>* e = primeiro->prox;
    delete primeiro;
    primeiro = e;
    return dado;
}

template<typename T>
bool Fila<T>::estaVazia() const{
    return primeiro == nullptr ? true : false;
}
template<typename T>
bool Fila<T>::estaNaFila(T dado) const{
    if(primeiro == nullptr)
        return false;
    ELEMENTO<T>* e = primeiro;
    while(e != nullptr){
        if(e->dados == dado)
            return true;
        e = e->prox;
    }
    return false;
}

template<typename T>
void Fila<T>::esvaziar(){
    ELEMENTO<T>* e = primeiro,* excluir;
    while(e != nullptr){
       excluir = e;
       e = e->prox;
       delete excluir;
    }
}

template<typename T>
void Fila<T>::mostrarFila() const{
    int i = 0;
    ELEMENTO<T>* e = primeiro;
    while(e != nullptr){
        cout << "Elemento " << i << " : \t" << e->dados << endl;
        e = e->prox;
        i++;
    }
}


#endif // FILA_HPP_INCLUDED
