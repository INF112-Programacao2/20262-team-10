#ifndef AREA_HPP
#define AREA_HPP

#include "animal.hpp"
using std::string;

#include <iostream>

class Area {
    protected:
        std::string nome; // nome da area, ex: pasto1, pasto2, curral
        std::string estadoPasto; // seco, ruim, intermediario, bom, muito bom
        double area; // em hectares
        int capacidadeMaxima;
        Animal** rebanho;
        int qtdAtualDeAnimais;

    public:
        Area(std::string nome, std::string estadoPasto, double area);
        ~Area();

        // funcoes de calculo
        double calcularLotacaoArea();

        // manejo de animais
        void adicionarAnimal(Animal* a);
        void removerAnimal(std::string idAnimal);
        Animal* extrairAnimal(std::string idAnimal);
        void promoverAnimal();
        Animal* buscarAnimal(std::string id);
        double getGastoIndividual(std::string id, double precoLeite, SacaRacao** relatorioRacoes);
        double getGastoDaArea(double precoLeite, SacaRacao** relatorioRacoes);

        // gets
        int getCapacidadeMaxima();
        std::string getNomeArea();
        std::string getEstadoPasto();
        int getQtdAnimais();
};

#endif