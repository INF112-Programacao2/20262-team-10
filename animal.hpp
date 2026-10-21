#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>

class Animal{
protected:
    std::string _id, estadoSaude, temperamento;
    char sexo;
    double peso;
    int idadeMeses, qtdFilhos, qtdVacinas;
    Animal* pai;
    Animal* mae;
    Animal** filhos;
public:
    virtual double calcularGastoDiario() = 0;
    virtual double calcularReceitaDiaria() = 0;
    virtual ~Animal();
};

#endif