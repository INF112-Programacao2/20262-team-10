#include "animal.hpp"

Animal::Animal(std::string id, char s, double p) {
    _id = id;
    sexo = s;
    peso = p;
    idadeMeses = qtdFilhos = qtdVacinas = 0;
    EmQuarentena = false;
}

//inicializador overload 
Animal::Animal(std::string id, char s, double p, int iM, int qtdF, int qtdV){
    _id = id;
    sexo = s;
    peso = p;
    idadeMeses = iM;
    qtdFilhos = qtdF;
    qtdVacinas = qtdV;
}

std::string Animal::getId() const{
    return _id;
}

bool Animal::getQuarentena() const{
    return EmQuarentena;
} 

Animal::~Animal() {
    
}
