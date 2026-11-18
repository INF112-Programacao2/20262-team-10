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
        Area** area;
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

        //financeiro precisa de funcoes de Rhuan
        double calcularGastoTotalDiarioFazenda();
        double calcularReceitaTotalDiariaFazenda();
        double calcularLucroGeralDiario();
        void emitirRelatorioBreakEvenEVendas();
        //vai passar o valor de venda? Precisa calcular quanto vai vende?
        bool venderAnimal(std::string id); //precisa funcoes de valor do animal -> genetico + valor investido (Rhuan vai implementar)

        //area-gado
        void adicionarArea(Area* a);
        void removerArea(Area* a);
        void selecionarArea(std::string n); //passa o nome da area que deseja selecionar
        Animal* buscarAnimalGlobal(string id);
        bool transferirAnimalEntreAreas(string idAnimal, string nomeArea);
        bool transferirLoteDeAnimaisEntreAreas(string* ids, int qtdAnimais, string areaDestino);
        bool enviarParaQuarentena(std::string motivo, std::string idAnimal, int dias);
        bool liberarDaQuarentena(std::string idAnimal);
        void promoverAnimalGlobal();

        //gets
        int getQuantidadeTotalAnimais();
        std::string getAreaAtual() const;
};

#endif