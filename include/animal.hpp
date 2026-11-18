#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <string>
#include "sacaRacao.hpp"
#include "snapshotancestral.hpp"

class Animal{
protected:
    std::string _id, estadoSaude, temperamento;
    char sexo;
    double peso;
    int idadeMeses, qtdFilhos, qtdVacinas;
    bool EmQuarentena;
    std::string idPai;
    std::string idMae;
    std::string* idFilhos;

    //snapshot 
    SnapshotAncestral snapshotPai;
    SnapshotAncestral snapshotMae;

public:
    Animal(std::string id, char s, double p);
    //inicializador sobrecarga ex: animal comprado
    Animal(std::string id, char s, double p, int iM, int qtdF, int qtdV);
    virtual double calcularGastoDiario(double precoLeite, SacaRacao** relatorioRacao) = 0;
    virtual double calcularReceitaDiaria(double valorMercado) = 0;
    virtual ~Animal();

    //gets
    std::string getId() const;
    std::string getEstadoSaude() const;
    std::string getTemperamento() const;
    char getSexo() const;
    double getPeso() const;
    int getIdadeMeses() const;
    int getQtdFilhos() const;
    int getQtdvacinas() const;
    bool getQuarentena() const;
    std::string getPai() const;
    std::string getMae() const;
    std::string* getFilhos() const;

    //get do snapshot
    SnapshotAncestral getSnapshotPai() const { return snapshotPai; }
    SnapshotAncestral getSnapshotMae() const { return snapshotMae; }

    //set do snapshot
    void setSnapshotPai(const SnapshotAncestral& salva) { snapshotPai = salva; }
    void setSnapshotMae(const SnapshotAncestral& salva) { snapshotMae = salva; }


    //metodos usados para verificar promoção
    virtual std::string getTipo() const = 0;
    virtual bool podeSerPromovido() const = 0;
};

#endif

