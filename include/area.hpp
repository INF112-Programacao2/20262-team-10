#ifndef AREA_HPP
#define AREA_HPP

#include <iostream>
#include <string>
#include "animal.hpp"
#include "sacaRacao.hpp"

using std::string;

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
        ~Area();

        // funcoes de calculo
        double calcularLotacaoArea();
        double getSacoGastoIndividual(string id, SacaRacao** relatorioRacoes);
        ConsumoRacao getSacosGastosNaArea(SacaRacao** relatorioRacoes);

        // manejo de animais
        void adicionarAnimal(Animal* a, bool silencioso = false);
        void escreverNoArquivo(std::ofstream& out, std::string idDono, std::string nomeFazenda);
        void removerAnimal(string idAnimal);
        Animal* extrairAnimal(string idAnimal);
        void promoverAnimal();
        Animal* buscarAnimal(string id);
        double getGastoIndividual(string id, double precoLeite, SacaRacao** relatorioRacoes);
        double getGastoDaArea(double precoLeite, SacaRacao** relatorioRacoes);

        // getters
        string getNomeArea();
        string getEstadoPasto();
        double getArea();
        int getCapacidadeMaxima();
        int getQtdAnimais();
        Animal** getRebanho();

        // setters
        void setNomeArea(string n);
        void setEstadoPasto(string estado);
        void setArea(double a);
};

#endif