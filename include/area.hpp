#ifndef AREA_HPP
#define AREA_HPP

#include <iostream>

class Area {
    private:
        std::string estadoPasto;
        double area;
        int capacidadeMaxima;
        Animal** rebanho;
        int qtdAtualDeAnimais;

    public:
        Area();

        //manejo de animais
        
        //funcoes de calculo
        double calcularLotacaoArea();

        void adicionarAnimal(Animal* a);
        void removerAnimal(Animal* a);
        //gets
        int getCapacidadeMaxima();

};


#endif AREA_HPP