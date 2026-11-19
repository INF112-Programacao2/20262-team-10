#ifndef FARM_HPP
#define FARM_HPP

#include <iostream>
#include <string>
#include "animal.hpp"
#include "area.hpp"
#include "user.hpp"
#include "sacaRacao.hpp"

class Farm {
    private:
        // identificadores
        std::string nome;
        std::string localizacao;
        std::string areaAtual;
        User* proprietario;
        Area** area;
        int qtdAreas;

        // resumo de dados
        int qtdTotalDeAnimais;
        int qtdBaixas;

        // setor financeiro
        double saldoCaixa;
        double lucroAcumuladoVendas;
        double prejuizoAcumuladoMortes;

        // simulacao futura
        int diaAtualSimulacao;

        Animal* encontrarAnimalOpcional(const std::string& identificador) const;

    public:
        Farm(std::string nome, std::string localizacao, User* proprietario);
        ~Farm();

        // financeiro e consumo
        double calcularGastoTotalDiarioFazenda();
        double calcularReceitaTotalDiariaFazenda();
        double calcularLucroGeralDiario();
        ConsumoRacao calcularConsumoRacaoDiarioFazenda();
        void emitirRelatorioBreakEvenEVendas();
        double venderAnimal(std::string id);
        double comprarAnimal(Animal* animalComprado);

        // area-gado
        void adicionarArea(Area* a, bool silencioso = false);
        void salvarFazenda();
        static Farm* carregarFazenda(User* proprietario, std::string nomeFazenda = "");
        void removerArea(Area* a);
        void selecionarArea(std::string n);
        Animal* buscarAnimalGlobal(std::string id);
        bool transferirAnimalEntreAreas(std::string idAnimal, std::string nomeArea);
        bool transferirLoteDeAnimaisEntreAreas(std::string* ids, int qtdAnimais, std::string areaDestino);
        bool enviarParaQuarentena(std::string motivo, std::string idAnimal, int dias);
        bool liberarDaQuarentena(std::string idAnimal);
        void promoverAnimalGlobal();
        void realizarCobertura(const std::string& idMacho, const std::string& idFemea);
        void registrarNascimento(const std::string& idFemea, const std::string& nomeArea, std::string id, char sexo, double peso);
        void calcularMeritoTouro(std::string idTouro);
        // gets
        int getQuantidadeTotalAnimais();
};

#endif