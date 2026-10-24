#include <iostream>
#include "bezerro.hpp"

Bezerro::Bezerro(std::string id, char s, double p) : Animal(id, s, p) {}

Bezerro::Bezerro(std::string id, char s, double p, int iM, int qtdF, int qtdV) : Animal(id, s, p, iM, qtdF, qtdV) {}

Bezerro::~Bezerro() {}