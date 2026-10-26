#include "animal.hpp"

Animal::Animal(std::string id, char s, double p) : _id(id), sexo(s), peso(p) {
    idadeMeses = qtdFilhos = qtdVacinas = 0;
    EmQuarentena = false;
}

//inicializador overload 
Animal::Animal(std::string id, char s, double p, int iM, int qtdF, int qtdV) : _id(id), sexo(s), peso(p), idadeMeses(iM), qtdFilhos(qtdF), qtdVacinas(qtdV) {}

//gets
std::string Animal::getId() const{return _id;}
std::string Animal::getEstadoSaude() const {return estadoSaude;}
std::string Animal::getTemperamento() const {return temperamento;}
char Animal::getSexo() const {return sexo;}
double Animal::getPeso() const {return peso;}
int Animal::getIdadeMeses() const {return idadeMeses;}    
int Animal::getQtdFilhos() const {return qtdFilhos;}
int Animal::getQtdvacinas() const {return qtdVacinas;}
bool Animal::getQuarentena() const{return EmQuarentena;}
Animal* Animal::getPai() const{return pai;} 
Animal* Animal::getMae() const{return mae;}
Animal** Animal::getFilhos() const{return filhos;}

Animal::~Animal() {
    
}
