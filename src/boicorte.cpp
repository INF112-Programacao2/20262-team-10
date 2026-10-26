#include <iostream>
#include "boicorte.hpp"

Boicorte::Boicorte(std::string id, char s, double p, int iM, int qtdF, int qtdV) : Animal(id, s, p, iM, qtdF, qtdV) {} 

Boicorte::~Boicorte() {}

//gets
double Boicorte:: getrendimentoCarcaca()const {return rendimentoCarcaca;}
double Boicorte:: getmetaAbatePeso()const {return metaAbatePeso;}
std::string Boicorte:: getFaseEngorda()const {return faseEngorda;}
//sets
void Boicorte:: setrendimentoCarcaca(double r) {rendimentoCarcaca=r;}
void Boicorte:: setmetaAbatePeso(double m) {metaAbatePeso=m;}
void Boicorte:: setFaseEngorda(std::string f) {faseEngorda=f;}