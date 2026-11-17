#ifndef AREA_HPP
#define AREA_HPP

#include "animal.hpp"
using std::string;

#include <iostream>

class Area {
    protected:
        string nome; //nome da area, ex-pasto1,pasto2,curral
        string estadoPasto; //seco, ruim, intermediario, bom, muito bom
        double area; //em hectares
        int capacidadeMaxima; //calculado com base na qualidade do pasto, usado para recomendar compra de racao
        Animal** rebanho;
        int qtdAtualDeAnimais; //quantidade atual de animais de determinada area

    public:
        Area(string nome, string estadoPasto, double area);

        //manejo de animais
        
        //funcoes de calculo
        double calcularLotacaoArea();

        void adicionarAnimal(Animal* a);
        void removerAnimal(string idAnimal);
        void promoverAnimal();
        Animal* buscarAnimal(string id);

        //gets
        int getCapacidadeMaxima();
        std::string getNomeArea();
        int getQtdAnimais();
};


#endif 