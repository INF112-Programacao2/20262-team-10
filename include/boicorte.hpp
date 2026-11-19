#ifndef BOICORTE_HPP
#define BOICORTE_HPP
#include "animal.hpp"

class Boicorte : public Animal{
private:
    double rendimentoCarcaca;
    double metaAbatePeso;
    bool faseEngorda;
    Boicorte();
public:
    //construtor para animal adquirido
    Boicorte(std::string id, char s, double p, int iM, int qtdF, int qtdV);
    //construtor para bezerro promovido
    Boicorte(Animal *bezerropromovido);
    ~Boicorte();
    //gets
    double getrendimentoCarcaca()const;
    double getmetaAbatePeso()const;
    bool getFaseEngorda()const;
    //sets
    void setrendimentoCarcaca(double r);
    void setmetaAbatePeso(double m);
    void setFaseEngorda(bool f);
    std::string getTipoRacao() const override;
    double calcularSacosDiarios(SacaRacao** relatorioRacao) const override;
    //metodos herança
    double calcularGastoDiario(double precoLeite,  SacaRacao** relatorioRacao) override;
    double calcularReceitaDiaria(double valorMercado) override;
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
    //metodos exclusivos
    bool prontoAbate();
    double calcularValorVendaMercado();
    double calcularArrobaLiquidas();
    void imprimirFichaCompleta()const;
    void salvarEmArquivo();
    static Boicorte* carregarBoicorte(std::string id);
};

#endif