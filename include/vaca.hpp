#ifndef VACA_HPP
#define VACA_HPP
#include "animal.hpp"

class Vaca : public Animal{
private:
    double producaoLeiteDia;
    bool emLactacao;
public:
    //construtor animal adquirido
    Vaca(std::string id, char s, double p, int iM, int qtdF, int qtdV);
    //construtor bezerro promovido
    Vaca(Animal *a);
    ~Vaca();
    //gets
    double getProducaoLeiteDia()const;
    bool getEmLactacao()const;
    //sets
    void setProducaoLeiteDia(double p);
    void setEmLactacao(bool l);
    //metodos herança
    double calcularGastoDiario () override;
    double calcularReceitaDiaria() override;
};

#endif