#ifndef AREA_HPP
#define AREA_HPP

#include <iostream>
#include <string>
#include "animal.hpp"
#include "sacaRacao.hpp"

using std::string;

struct ContagemRebanho {
    int vacas = 0;
    int touros = 0;
    int boisCorte = 0;
    int bezerros = 0;

    int total() const {
        return vacas + touros + boisCorte + bezerros;
    }
};

class Area {
    protected:
        string nome; // nome da area, ex: pasto1, pasto2, curral
        string estadoPasto; // seco, ruim, intermediario, bom, muito bom
        double area; // em hectares
        int capacidadeMaxima;
        Animal** rebanho;
        int qtdAtualDeAnimais;

        void atualizarCapacidadeMaxima();

    public:
        Area(string nome, string estadoPasto, double area);
        virtual ~Area();
        // Dentro de public na class Area:

        // funcoes de calculo
        double calcularLotacaoArea() const;
        double calcularFatorSuperlotacao() const;
        double getSacoGastoIndividual(string id, SacaRacao** relatorioRacoes);
        ConsumoRacao getSacosGastosNaArea(SacaRacao** relatorioRacoes);

        // manejo de animais
        void adicionarAnimal(Animal* a, bool silencioso = false);
        void escreverNoArquivo(std::ofstream& out, std::string idDono, std::string nomeFazenda);
        void removerAnimal(string idAnimal);
        Animal* extrairAnimal(string idAnimal);
        int promoverAnimal();
        Animal* buscarAnimal(string id);
        double getGastoIndividual(string id, double precoLeite, SacaRacao** relatorioRacoes);
        virtual double getGastoDaArea(double precoLeite, SacaRacao** relatorioRacoes);
        void listarAnimais() const;

        // getters
        string getNomeArea();
        string getEstadoPasto();
        double getArea();
        int getCapacidadeMaxima();
        int getQtdAnimais();
        Animal** getRebanho();
        ContagemRebanho getContagemPorTipo() const;

        // setters
        void setNomeArea(string n);
        void setEstadoPasto(string estado);
        void setArea(double a);
};

#endif