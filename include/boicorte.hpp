#ifndef BOICORTE_HPP
#define BOICORTE_HPP
#include "animal.hpp"

class Boicorte : public Animal{
private:
    double rendimentoCarcaca;
    double metaAbatePeso;
    std::string faseEngorda;
public:
    //construtor para animal adquirido
    Boicorte(std::string id, char s, double p, int iM, int qtdF, int qtdV);
    //construtor para bezerro promovido
    Boicorte(Animal *bezerropromovido);
    ~Boicorte();
    //gets
    double getrendimentoCarcaca()const;
    double getmetaAbatePeso()const;
    std::string getFaseEngorda()const;
    //sets
    void setrendimentoCarcaca(double r);
    void setmetaAbatePeso(double m);
    void setFaseEngorda(std::string f);
    //metodos herança
    //double calcularGastoDiario() override;
    //double calcularReceitaDiaria() override;
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
    //metodos exclusivos
    bool prontoAbate();
    double calcularValorVendaMercado();
    double calcularArrobaLiquidas();
    void imprimirFichaCompleta()const;
};

#endif