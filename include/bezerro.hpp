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
    Bezerro(std::string id, char s, double p);
    Bezerro(std::string id, char s, double p, int iM, int qtdF, int qtdV);
    ~Bezerro();
    double calculoGastoDiario();
    double calcularReceitaDiaria();
    double calcularValorVendaMercado();
    void imprimirFichaCompleta() const;
    void realizarDesmame();
    bool atingiuIdadeAdulta();
    std::string recomendarDestinoMaturidade();
};

#endif