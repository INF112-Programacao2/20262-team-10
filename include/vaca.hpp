#ifndef VACA_HPP
#define VACA_HPP
#include "animal.hpp"

class Vaca : public Animal{
private:
    double producaoLeiteDia;
    bool emLactacao;
public:
    Vaca(std::string id, char s, double p);
    Vaca(std::string id, char s, double p, int iM, int qtdF, int qtdV);
    double calcularGastoDiario () override;
    double calcularReceitaDiaria() override;

    //gets
    bool getLactacao() const;
    double getProducao() const;
    ~Vaca();
};

#endif