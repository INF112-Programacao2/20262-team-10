#ifndef BEZERRO_H
#define BEZERRO_H
#include "animal.hpp"

class Bezerro : public Animal{
private:
    double consumoLeiteLitros;
    double ganhoPesoMedioDiario;
    int idadeDesmameMeses;
    bool desmamado;
    std :: string idPai, idMae;
public:
    //construtor para animal nascido
    Bezerro(std::string id, char s, double p);
    //construtor para animal adquirido
    Bezerro(std::string id, char s, double p, int iM, int qtdF, int qtdV, double cLL, double gPMD, int iDM, bool d);
    ~Bezerro();
    //gets
    double getConsumoLeiteLitros()const;
    double getGanhoPesoMedioDiario()const;
    int getIdadeDesmameMeses()const;
    bool getDesmamado()const;
    std :: string getIdPai()const;
    std :: string getIdMae()const;
    //sets
    void setConsumoLeiteLitros(double c);
    void setGanhoPesoMedioDiario(double g);
    void setIdadeDesmameMeses(int i);
    void setDesmamado(bool d);
    void setIdPai(std::string id);
    void setIdMae(std::string id);
    //metodos herança
    double calcularGastoDiario(double precoLeite, double precoRacao) override;
    double calcularReceitaDiaria(double valorMercado) override;
    std::string getTipo() const override;
    bool podeSerPromovido() const override;
    //metodos exclusivos
    double calcularValorVendaMercado();
    void imprimirFichaCompleta() const;
    void realizarDesmame();
    bool atingiuIdadeAdulta() const;
    std::string recomendarDestinoMaturidade();
};

#endif