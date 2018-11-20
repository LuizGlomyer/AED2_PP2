#ifndef ELEMENTO_HPP_INCLUDED
#define ELEMENTO_HPP_INCLUDED

template<typename T>
class ELEMENTO{
public:
    T getDados();
    ELEMENTO* prox,* antc;
    T dados;
};

template<typename T>
T ELEMENTO<T>::getDados(){
    return dados;
}


#endif // ELEMENTO_HPP_INCLUDED
