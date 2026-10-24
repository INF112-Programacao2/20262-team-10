#include <iostream>
#include "touro.hpp"

Touro::Touro(std::string id, char s, double p, int iM, int qtdF, int qtdV, double vDS) : Animal(id, s, p, iM, qtdF, qtdV) , valorDoseSemen(vDS) {}

Touro::~Touro(){}