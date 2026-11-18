#ifndef VACA_HPP
#define VACA_HPP
#include "animal.hpp"
#include "touro.hpp"

class Vaca : public Animal{
private:
    double producaoLeiteDia;
    bool emLactacao, prenha;
    std::string idTouroPai;
public:
    //construtor animal adquirido
    Vaca(std::string id, char s, double p, int iM, int qtdF, int qtdV);
    //metodo bezerro promovido
    //static Vaca* PromoverParaVaca(Animal *a); vou reconsiderar a implementação deste metodo 
    ~Vaca();
    //gets
    double getProducaoLeiteDia()const;
    bool getEmLactacao()const;
    bool getPrenha()const;
    //sets
    void setProducaoLeiteDia(double p);
    void setEmLactacao(bool l);
    //metodos herança
    double calcularGastoDiario(double precoLeite,  SacaRacao** relatorioRacao) override;
    double calcularReceitaDiaria(double valorMercado) override;
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
    //demais metodos
    bool emprenhar(Touro* macho);
    std::string getIdTouroPai() const;
    std::string parir();
    void adicionarFilho(std::string idFilho);
};

#endif