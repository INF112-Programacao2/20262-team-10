#ifndef FARM_HPP
#define FARM_HPP

#include "animal.hpp"
#include <iostream>


class Farm {
    private:
        std::string nome;
        std::string localizacao;
        double area;
        int capacidadeMaxima;
        //a pessoa nao deve passar a quantidade maxima, mas sim ser calculado com base em caracteristicas
        //da fazenda
        int qtdAtualDeAnimais;
        Animal** rebanho;
    
    public:
        //aqui estao os valores de inicializacao, o qa vai ser removido
        //s - nome, l - localizacao, a-area, c-capacidade maxima, qa-quantidade maxima de animais
        Farm(std::string s, std::string l, double a, int c, int qa);
        void adicionarAnimal(Animal* a);
        void removerAnimal(Animal* a);
        double calcularLotacaoArea();
        double calcularLucroGeralDiario();
        ~Farm();
};

#endif