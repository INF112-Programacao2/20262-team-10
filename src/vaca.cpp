#include <iostream>
#include "vaca.hpp"

Vaca::Vaca(std::string id, char s, double p, int iM, int qtdF, int qtdV) : Animal(id, s, p, iM, qtdF, qtdV) {}

Vaca::~Vaca(){

}

//gets
double Vaca:: getProducaoLeiteDia()const {return producaoLeiteDia;}
bool Vaca:: getEmLactacao()const {return emLactacao;}
//sets
void Vaca:: setProducaoLeiteDia(double p) {producaoLeiteDia=p;}
void Vaca:: setEmLactacao(bool l) {emLactacao=l;}
