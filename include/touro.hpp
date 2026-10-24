#ifndef TOURO_HPP
#define TOURO_HPP
#include "animal.hpp"
#include "vaca.hpp"
class Touro : public Animal{
private:
    double valorDoseSemen, meritoGeneticoPTA;
    int dosesEstoque, qtdCoberturas;
public:
    Touro(std::string id, char s, double p, int iM, int qtdF, int qtdV, double vDS);
    Touro(Animal *bezerroPromovido);
    ~Touro();
    double calcularConsumoSacaDia();
    double calcularGastoDiario() override;
    double calcularReceitaDiaria() override;
    double calcularValorVendaMercado();
    void imprimirFichaCompleta() const;
    bool cobrirFemea(Vaca *v);
};

#endif