#ifndef FARM_HPP
#define FARM_HPP

#include "animal.hpp"
#include "area.hpp"
#include "user.hpp"
#include <iostream>


class Farm {
    private:
        //identificadores
        std::string nome;
        std::string localizacao;
        std::string areaAtual;
        User* proprietario;
        Area** area; //valor em hectares
        int qtdAreas;

        //resumo de dados
        int qtdTotalDeAnimais;
        int qtdBaixas;

        //setor financeiro
        double saldoCaixa;

        double lucroAcumuladoVendas;
        double prejuizoAcumuladoMortes;

        //simulacao futuras
        int diaAtualSimulacao;
    
    public:
        Farm(std::string nome, std::string localizacao, User* proprietario);
        ~Farm();

        //financeiro
        double calcularGastoTotalDiarioFazenda();
        double calcularReceitaTotalDiariaFazenda();
        double calcularLucroGeralDiario();
        void emitirRelatorioBreakEvenEVendas();
        //vai passar o valor de venda? Precisa calcular quanto vai vende?
        bool venderAnimal(std::string id);


        //area-gado
        void adicionarArea(Area* a);
        void removerArea(Area* a);
        void selecionarArea(std::string n); //passa o nome da area que deseja selecionar
        Animal* buscarAnimalGlobal(Animal* a);
        bool transferirAnimalEntreAreas(Animal* a, Area ar);
        bool enviarParaQuarentena(std::string motivo, std::string idAnimal, int dias);
        bool liberarDaQuarentena(std::string idAnimal);


        //gets
        int getQuantidadeTotalAnimais();
};

#endif