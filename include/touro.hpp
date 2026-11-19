#ifndef TOURO_HPP
#define TOURO_HPP
#include "animal.hpp"
#include "vaca.hpp"

class Vaca;

class Touro : public Animal{
private:
    double valorDoseSemen, meritoGeneticoPTA;
    int dosesEstoque, qtdCoberturas;
public:
    //construtor animal adquirido
    Touro(std::string id, char s, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais, double vDS);
    //construtor bezerro promovido
    Touro(Animal *bezerroPromovido);
    ~Touro();
    //gets
    double getValorDoseSemen()const;
    double getMerito()const;
    int getDosesEstoque()const;
    int getQtdCoberturas()const;
    //sets
    void setValorDoseSemen(double v);
    void setMerito(double m);
    void setDosesEstoque(int d);
    void setQtdCoberturas(int q);
    //metodos herança
    double calcularGastoDiario(double precoLeite,  SacaRacao** relatorioRacao) override;
    double calcularReceitaDiaria(double valorMercado) override;
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
    //metodos exclusivos
    double calcularConsumoSacaDia();
    double calcularValorVendaMercado();
    void imprimirFichaCompleta() const;
    bool cobrirFemea(Vaca* femea);
    void adicionarFilho(std::string idFilho);
};

#endif