#include <iostream>
#include <limits>
using namespace std;

typedef int Vertice;
typedef int Peso;
enum Cor {Branco, Cinza, Preto};
enum Status {Sadio, Doente};


template<typename T>
class ELEMENTO;

template<typename T>
class ListaLigada{
public:
    template<typename Y>
    friend ostream& operator<<(ostream&, const ListaLigada&);
    T& operator[](int);
    ListaLigada();
    ~ListaLigada();
    void inserir(T);
    void remover(int = 0);
    void esvaziar();
    bool estaVazia() const;
    bool estaNaLista(T) const;
    T& posicao(int);
    int getTamanho() const;
private:
    ELEMENTO<T>* inicio;
    int tamanho;
};

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
private:
    ELEMENTO<T>* topo;
};

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
    private:
        ELEMENTO<T>* primeiro,* ultimo;
};

template<typename T>
class ELEMENTO{
public:
    T getDados();
    void setDados(T);
    ELEMENTO* getProx();
    ELEMENTO* getAntc();
    void setProx(ELEMENTO*);
    void setAntc(ELEMENTO*);
    friend T& ListaLigada<T>::posicao(int i);
private:
    ELEMENTO* prox,* antc;
    T dados;
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

class Grafo_MA{
public:
    Grafo_MA(int);
    ~Grafo_MA();
    void inserirAresta(Vertice, Vertice, Peso = 0);
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



int main(){
    int ordemCerebro, numSinapses;
    cin >> ordemCerebro;
    cin >> numSinapses;
    Grafo_MA cerebro(ordemCerebro);

    for(int i = 1; i <= numSinapses; i++){
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
    for(int i = 1; i <= ordemCerebro; i++){
        int ordemBloco, numSinapses, qtdDoentes;
        cin >> ordemBloco;
        cin >> numSinapses;
        cin >> qtdDoentes;

        blocosNeuronios[i] = new Grafo_MA(ordemBloco);
        for(int j = 0; j < qtdDoentes; j++){
            int neuronioDoente;
            cin >> neuronioDoente;
            blocosNeuronios[i]->marcarDoente(neuronioDoente);
            cerebro.marcarDoente(i);
        }

        for(int j = 1; j <= numSinapses; j++){
            int sinapse1, sinapse2; float distancia;
            cin >> sinapse1;
            cin >> sinapse2;
            cin >> distancia;
            blocosNeuronios[i]->inserirAresta(sinapse1, sinapse2, distancia);
        }

    }

    Dijkstra_MA calcular;
    calcular.menorCaminho(cerebro, blocoEntrada);

    Pilha<Vertice> blocosVisitados;
    int blocoAtual = blocoSaida;
    blocosVisitados.adicionar(blocoAtual);
    while(blocoAtual != blocoEntrada){
        blocoAtual = calcular.getPredecessor()[blocoAtual];
        blocosVisitados.adicionar(blocoAtual);
    }

    float somatorio = 0;
    while(!blocosVisitados.estaVazia()){
        MST_MA calcularArvore = MST_MA();
        Vertice u = blocosVisitados.pop();
        if(blocosNeuronios[u]->estaDoente()){
            calcularArvore.MST_Prim(*blocosNeuronios[u], 1);
            somatorio += calcularArvore.getSomatorio(*blocosNeuronios[u]);
        }

    }
    cout << somatorio;

    for(int i = 1; i <= ordemCerebro; i++)
        delete blocosNeuronios[i];
    delete [] blocosNeuronios;


    return 0;
}





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
    mat[v][u]++;
    pesos[u][v] = p;
    pesos[v][u] = p;
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
}

MST_MA::~MST_MA(){
    destruir();
}

void MST_MA::destruir(){
    if(chave != nullptr)
        delete [] chave;
    if(pai != nullptr)
        delete [] pai;
}

void MST_MA::reinicializar(){
    destruir();
    pai = chave = nullptr;
}

void MST_MA::MST_Prim(Grafo_MA& G, Vertice r){
    int n = G.getTamanho();
    chave = new int[n+1];
    pai = new int[n+1];
    FilaPrioridades<int> fila;
    for(int i = 1; i <= n; i++){
        chave[i] = numeric_limits<int>::max();
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
}

Dijkstra_MA::~Dijkstra_MA(){
    if(pai != nullptr)
        delete [] pai;
    if(distancia != nullptr)
        delete [] distancia;
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
    FilaPrioridades<int> fila;
    for(Vertice u = 1; u <= n; u++){
        pai[u] = -1;
        distancia[u] = numeric_limits<int>::max();
    }
    distancia[r] = 0;
    for(Vertice u = 1; u <= n; u++)
        fila.inserir(u, distancia[u]);

    while(!fila.estaVazia()){
        Vertice u = fila.extrairElementoMinimo();
        for(Vertice v = 1; v <= n; v++){
            if(a[u][v] != 0){
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


void Dijkstra_MA::mostrarDistancia(){
    cout << "Raiz: " << raiz << '\n';
    for(int i = 1; i <= n; i++){
        cout << "Distancia ate o vertice " << i << ": " << distancia[i] << '\n';
    }
    cout << '\n';
}

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
T Fila<T>::pop(){
    if(primeiro == nullptr){
        throw bad_alloc();
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
        topo->setDados(dado);
        topo->setProx(nullptr);
        return;
    }
    ELEMENTO<T>* e = topo;
    topo = new ELEMENTO<T>;
    topo->setDados(dado);
    topo->setProx(e);
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
    T retorno = topo->getDados();
    ELEMENTO<T>* e = topo->getProx();
    delete topo;
    topo = e;
    return retorno;
}

template<typename T>
void Pilha<T>::esvaziar(){
    ELEMENTO<T>* e = topo,* excluir;
    while(e != nullptr){
        excluir = e;
        e = e->getProx();
        delete excluir;
    }
}

template<typename T>
bool Pilha<T>::estaVazia() const{
    return topo == nullptr ? true : false;
}




template<typename T>
ListaLigada<T>::ListaLigada(){
    inicio = nullptr;
    tamanho = -1;
}

template<typename T>
ListaLigada<T>::~ListaLigada(){
    esvaziar();
}

template<typename T>
void ListaLigada<T>::inserir(T dado){
    if(inicio == nullptr){
        inicio = new ELEMENTO<T>;
        inicio->setDados(dado);
        inicio->setProx(nullptr);
        tamanho++;
        return;
    }
    ELEMENTO<T>* e = inicio;
    while(e->getProx() != nullptr)
        e = e->getProx();
    e->setProx(new ELEMENTO<T>);
    e = e->getProx();
    e->setDados(dado);
    e->setProx(nullptr);
    tamanho++;
}


template<typename T>
void ListaLigada<T>::remover(int indice){
    if(inicio == nullptr)
        return;
    if(inicio->getProx() == nullptr){
        delete inicio;
        inicio = nullptr;
        tamanho--;
        return;
    }
    if(indice > tamanho)
        return;
    if(indice > 0){
        ELEMENTO<T>* excluir = inicio,* antecessor;
        for(int i = 0; i < indice; i++){
            antecessor = excluir;
            excluir = excluir->getProx();
        }
        antecessor->setProx(excluir->getProx());
        //antecessor->prox = excluir->getProx();
        delete excluir;
        tamanho--;
        return;
    }
    ELEMENTO<T>* e = inicio->getProx();
    delete inicio;
    inicio = e;
    tamanho--;
}

template<typename T>
void ListaLigada<T>::esvaziar(){
    ELEMENTO<T>* e = inicio, * excluir;
    while(e != nullptr){
        excluir = e;
        e = e->getProx();
        if(excluir != nullptr){
            delete excluir;
            excluir = nullptr;
        }
    }
    inicio = nullptr;
    tamanho = -1;
}



template<typename T>
bool ListaLigada<T>::estaVazia() const{
    return inicio == nullptr ? true : false;
}

template<typename T>
bool ListaLigada<T>::estaNaLista(T elemento) const{
    ELEMENTO<T>* e = inicio;
    while(e != nullptr){
        if(e->getDados() == elemento)
            return true;
        e = e->getProx();
    }
    return false;
}

template<typename T>
int ListaLigada<T>::getTamanho() const{
    return tamanho;
}

template<typename T>
T& ListaLigada<T>::posicao(int i){
    ELEMENTO<T>* e = inicio;
    for(int j = 0; j < i; j++)
        e = e->getProx();

    return e->dados;
}

template<typename Y>
ostream& operator<<(ostream& saida, const ListaLigada<Y>& lista){
    for(int i = 0; i <= lista.getTamanho(); i++)
        cout << "Indice " << i << ": " << lista[i] << '\n';
    cout << '\n';
    return saida;
}

template<typename T>
T& ListaLigada<T>::operator[](int i){
    T& a = posicao(i);
    return a;
}


template<typename T>
T ELEMENTO<T>::getDados(){
    return dados;
}

template<typename T>
void ELEMENTO<T>::setDados(T d){
    dados = d;
}
template<typename T>
ELEMENTO<T>* ELEMENTO<T>::getProx(){
    return prox;
}
template<typename T>
ELEMENTO<T>* ELEMENTO<T>::getAntc(){
    return antc;
}
template<typename T>
void ELEMENTO<T>::setProx(ELEMENTO* p){
    prox = p;
}
template<typename T>
void ELEMENTO<T>::setAntc(ELEMENTO* a){
    antc = a;
}
