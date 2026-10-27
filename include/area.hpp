#ifndef AREA_HPP
#define AREA_HPP

#include "animal.hpp"

#include <iostream>

class Area {
    protected:
        std::string nome; //nome da area, ex-pasto1,pasto2,curral
        std::string estadoPasto; //seco, ruim, intermediario, bom, muito bom
        double area; //em hectares
        int capacidadeMaxima; //calculado com base na qualidade do pasto, usado para recomendar compra de racao
        Animal** rebanho;
        int qtdAtualDeAnimais; //quantidade atual de animais de determinada area

    public:
        Area(std::string nome, std::string estadoPasto, double area);

        //manejo de animais
        
        //funcoes de calculo
        double calcularLotacaoArea();

        void adicionarAnimal(Animal* a);
        void removerAnimal(Animal* a);
        //gets
        int getCapacidadeMaxima();
        std::string getNomeArea();
        int getQtdAnimais();
};


#endif 