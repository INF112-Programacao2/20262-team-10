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
    //metodo bezerro promovido
    //static Vaca* PromoverParaVaca(Animal *a); vou reconsiderar a implementação deste metodo 
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
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
};

#endif