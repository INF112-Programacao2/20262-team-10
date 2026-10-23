#ifndef FARM_HPP
#define FARM_HPP

#include "animal.hpp"
#include "area.hpp"
#include <iostream>


class Farm {
    private:
        std::string nome;
        std::string localizacao;
        //valor em hectare
        Area** area;
        int qtdAreas;
        //a pessoa nao deve passar a quantidade maxima, mas sim ser calculado com base em caracteristicas
        //da fazenda
        int qtdTotalDeAnimais;

    
    public:
        //construtor
        //       s - nome, l - localizacao  outros atributos são calculados;
        Farm(std::string s, std::string l);

        double calcularLucroGeralDiario();

        void adicionarArea(Area* a);
        void removerArea(Area* a);

        //gets
        int getQuantidadeTotalAnimais();


        ~Farm();
};

#endif