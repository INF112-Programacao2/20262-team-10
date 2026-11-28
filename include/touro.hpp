#ifndef TOURO_HPP
#define TOURO_HPP
#include "animal.hpp"
#include "vaca.hpp"

class Vaca;
class Touro : public Animal{
private:
    double valorDoseSemen;
    int dosesEstoque, qtdCoberturas;
    Touro();
public:
    //construtor animal adquirido
    Touro(std::string id, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais = nullptr, double vDS = 0.0);
    //construtor bezerro promovido
    Touro(Animal *bezerroPromovido);
    ~Touro();
    //gets
    double getValorDoseSemen()const;
    int getDosesEstoque()const;
    int getQtdCoberturas()const;
    std::string getTipoRacao() const override;
    double calcularSacosDiarios(SacaRacao** relatorioRacao) const override;
    //sets
    void setValorDoseSemen(double v);
    void setDosesEstoque(int d);
    void setQtdCoberturas(int q);
    //metodos herança
    double calcularGastoDiario(double precoLeite,  SacaRacao** relatorioRacao) override;
    double calcularReceitaDiaria(double valorMercado) override;
    double calcularValorVendaSugerido(double precoArroba, double precoLeite = 0.0) override;
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
    void imprimirFichaParcial()const override;
    //metodos exclusivos
    double calcularConsumoSacaDia();
    double calcularValorVendaMercado(double precoArroba);
    bool cobrirFemea(Vaca* femea);
    void salvarEmArquivo() override;
    void escreverLinhaArquivo(std::ofstream& out) const override;
    static Touro* carregarTouro(std::string id);
};

#endif