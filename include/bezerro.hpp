#ifndef BEZERRO_H
#define BEZERRO_H
#include "animal.hpp"

class Bezerro : public Animal{
protected:
    double consumoLeiteLitros;
    double ganhoPesoMedioDiario;
    int idadeDesmameMeses;
    bool desmamado;
    Bezerro();
public:
    //construtor para animal nascido
    Bezerro(std::string id, char s, double p);
    //construtor para animal adquirido
    Bezerro(std::string id, char s, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais, double cLL, double gPMD, int iDM, bool d);
    ~Bezerro();
    //gets
    double getConsumoLeiteLitros()const;
    double getGanhoPesoMedioDiario()const;
    int getIdadeDesmameMeses()const;
    std::string getTipoRacao() const override;
    double calcularSacosDiarios(SacaRacao** relatorioRacao) const override;
    bool getDesmamado()const;
    //sets
    void setConsumoLeiteLitros(double c);
    void setGanhoPesoMedioDiario(double g);
    void setIdadeDesmameMeses(int i);
    void setDesmamado(bool d);
    //metodos herança
    double calcularGastoDiario(double precoLeite, SacaRacao** relatorioRacao) override;
    double calcularReceitaDiaria(double precoLeite) override;
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
    void imprimirFichaParcial()const override;
    //metodos exclusivos
    double calcularValorVendaMercado();
    void realizarDesmame();
    bool atingiuIdadeAdulta() const;
    std::string recomendarDestinoMaturidade();
    void salvarEmArquivo();
    static Bezerro* carregarBezerro(std::string id);
};

#endif