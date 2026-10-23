#include <iostream>
#include "vaca.hpp"

Vaca::Vaca(std::string id, char s, double p) : Animal(id, s, p) {}
Vaca::Vaca(std::string id, char s, double p, int iM, int qtdF, int qtdV) : Animal(id, s, p, iM, qtdF, qtdV) {}

Vaca::~Vaca(){

}

bool Vaca::getLactacao() const{
    return emLactacao;
}

double Vaca::getProducao() const{
    return producaoLeiteDia;
}