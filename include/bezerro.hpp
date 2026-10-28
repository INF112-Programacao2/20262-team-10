#ifndef BEZERRO_H
#define BEZERRO_H
#include "animal.hpp"

class Bezerro : public Animal{
private:
    double consumoLeiteLitros;
    double ganhoPesoMedioDiario;
    int idadeDesmameMeses;
    bool desmamado;
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
    //sets
    void setConsumoLeiteLitros(double c);
    void setGanhoPesoMedioDiario(double g);
    void setIdadeDesmameMeses(int i);
    void setDesmamado(bool d);
    //metodos herança
    //double calcularGastoDiario() override;
    //double calcularReceitaDiaria() override;
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