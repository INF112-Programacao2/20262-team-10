#ifndef FARM_HPP
#define FARM_HPP

#include "animal.hpp"
#include <iostream>


class Farm {
    private:
        std::string nome;
        std::string localizacao;
        //valor em hectare
        double area;
        int capacidadeMaxima;
        //a pessoa nao deve passar a quantidade maxima, mas sim ser calculado com base em caracteristicas
        //da fazenda
        int qtdAtualDeAnimais;
        Animal** rebanho;
    
    public:
        //construtor
        //       s - nome, l - localizacao, a-area,  outros atributos são calculados;
        Farm(std::string s, std::string l, double a);


        //manejo de animais
        void adicionarAnimal(Animal* a);
        void removerAnimal(Animal* a);

        //funcoes de calculo
        double calcularLotacaoArea();
        double calcularLucroGeralDiario();

        //gets
        int getQuantidadeAnimais();


        ~Farm();
};

#endif