#ifndef VACA_HPP
#define VACA_HPP
#include "animal.hpp"
#include "touro.hpp"

class Touro;

class Vaca : public Animal{
private:
    double producaoLeiteDia;
    double ultimaProducaoLeiteDia;
    bool emLactacao, prenha;
    std::string idTouroPai;
    Vaca();
public:
    //construtor animal adquirido
    Vaca(std::string id, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais = nullptr);
    //metodo bezerro promovido
    //static Vaca* PromoverParaVaca(Animal *a); vou reconsiderar a implementação deste metodo 
    ~Vaca();
    //gets
    double getProducaoLeiteDia()const;
    double getUltimaProducaoLeiteDia()const;
    bool getEmLactacao()const;
    std::string getTipoRacao() const override;
    double calcularSacosDiarios(SacaRacao** relatorioRacao) const override;
    bool getPrenha()const;
    bool obterRazaoProducao(double& razao) const override;
    //sets
    void setProducaoLeiteDia(double p);
    void setEmLactacao(bool l);
    //metodos herança
    double calcularGastoDiario(double precoLeite,  SacaRacao** relatorioRacao) override;
    double calcularReceitaDiaria(double valorMercado) override;
    double calcularValorVendaSugerido(double precoArroba, double precoLeite = 0.0) override;
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
    void imprimirFichaParcial()const override;
    //demais metodos
    bool emprenhar(Touro* macho);
    std::string getIdTouroPai() const;
    std::string parir();
    void salvarEmArquivo() override;
    void escreverLinhaArquivo(std::ofstream& out) const override;
    static Vaca* carregarProximo(std::ifstream& arquivo);
    static Vaca* carregarVaca(std::string id);
};

#endif