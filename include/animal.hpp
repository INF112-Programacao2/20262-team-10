#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <string>

class Animal{
protected:
    std::string _id, estadoSaude, temperamento;
    char sexo;
    double peso;
    int idadeMeses, qtdFilhos, qtdVacinas;
    bool EmQuarentena;
    Animal* pai;
    Animal* mae;
    Animal** filhos;
public:
    Animal(std::string id, char s, double p);
    //inicializador sobrecarga ex: animal comprado
    Animal(std::string id, char s, double p, int iM, int qtdF, int qtdV);
    virtual double calcularGastoDiario() = 0;
    virtual double calcularReceitaDiaria() = 0;
    virtual ~Animal();

    //gets
    std::string getId() const;
    bool getQuarentena() const;
};

#endif

