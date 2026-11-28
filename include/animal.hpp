#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <string>
#include "sacaRacao.hpp"
#include "snapshotancestral.hpp"

class Animal {
protected:
    std::string _id, estadoSaude, temperamento;
    char sexo;
    double peso;
    int idadeMeses, qtdFilhos, qtdVacinas;
    bool EmQuarentena;
    std::string idPai;
    std::string idMae;
    std::string* idFilhos;

    // Score genetico
    double pesoNascimento;
    double ganhoMedioPeso;
    double scoreBase;             // 0 a 100
    double ajusteDescendentes;
    double scoreGenetico;

    Animal();
    std::string formatarTextoSalvar(std::string texto) const;
    std::string formatarTextoCarregar(std::string texto) const;
    void escreverBase(std::ofstream& out) const;
    bool carregarBase(std::ifstream& in);

    virtual bool obterRazaoProducao(double& razao) const;

public:
    Animal(std::string id, char s, double p);

    // Inicializador sobrecarga ex: animal comprado
    Animal(std::string id, char s, double p, int iM, int qtdF,
           int qtdV, std::string* filhosIniciais = nullptr);

    virtual double calcularGastoDiario(double precoLeite, SacaRacao** relatorioRacao) = 0;
    virtual double calcularReceitaDiaria(double valorMercado) = 0;
    virtual std::string getTipoRacao() const = 0;
    virtual double calcularSacosDiarios(SacaRacao** relatorioRacao) const = 0;
    virtual void salvarEmArquivo() = 0;
    virtual void escreverLinhaArquivo(std::ofstream& out) const = 0;
    virtual ~Animal();

    // gets
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
    double getPesoNascimento() const;
    double getGanhoMedioPeso() const;
    double getScoreBase() const;
    double getAjusteDescendentes() const;
    double getScoreGenetico() const;

    // sets
    void setIdPai(std::string idPai);
    void setIdMae(std::string idMae);
    void setPesoNascimento(double p);
    void setAjusteDescendentes(double pontos);

    // score genetico
    virtual void registrarPesagem(double novoPeso, int novaIdadeMeses);
    double calcularScoreBase();
    void recalcularScoreGenetico(int qtdAncestrais, double ajusteAncestrais);
    double getFatorPrecoGenetico() const;
    void copiarDadosGeneticosDe(const Animal& outro);
    void copiarEstadoParaPromocao(const Animal& outro);

    // cria um registro plano para a tabela compartilhada de ancestrais
    SnapshotAncestral criarSnapshot(const std::string& status = "ativo") const;

    // metodos usados para verificar promocao
    virtual std::string getTipo() const = 0;
    virtual bool podeSerPromovido() const = 0;

    // demais metodos
    virtual void imprimirFichaParcial() const = 0;
};

#endif