#ifndef FARM_HPP
#define FARM_HPP

#include <iostream>
#include <string>
#include "animal.hpp"
#include "area.hpp"
#include "user.hpp"
#include "sacaRacao.hpp"
#include "snapshotancestral.hpp"

class Farm {
    private:
        // identificadores
        std::string nome;
        std::string localizacao;
        std::string areaAtual;
        User* proprietario;
        Area** area;
        int proximoIdAnimal;
        int qtdAreas;

        // registro compartilhado de ancestrais
        SnapshotAncestral* ancestrais;
        int qtdAncestrais;
        int capacidadeAncestrais;

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
        // media do score base de todos os animais 
        double calcularMediaScoreBase() const; 
        int coletarAnimaisVivosOrdenados(Animal** destino);
        void atualizarAncestraisComVinculo(Animal** vivos, int qtdVivos);
        void sincronizarFilhosNosPais(Animal** vivos, int qtdVivos);
        void calcularAjusteDeDescendentes(Animal** vivos, int qtdVivos, double mediaRebanho, double* ajusteDesc) const;
        void aplicarScoresFinais(Animal** vivos, int qtdVivos, double mediaRebanho, const double* ajusteDesc);
       

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
        Area* buscarArea(const std::string& nomeArea) const;
        void listarAreas() const;
        std::string getAreaAtual() const;
        void adicionarArea(Area* a, bool silencioso = false);
        void salvarFazenda();
        static Farm* carregarFazenda(User* proprietario, std::string nomeFazenda = "");
        void removerArea(Area* a);
        void selecionarArea(std::string n);
        Animal* buscarAnimalGlobal(std::string id);
        Area* buscarAreaDoAnimal(const std::string& id) const;
        bool transferirAnimalEntreAreas(std::string idAnimal, std::string nomeArea);
        bool transferirLoteDeAnimaisEntreAreas(std::string* ids, int qtdAnimais, std::string areaDestino);
        bool enviarParaQuarentena(std::string motivo, std::string idAnimal, int dias);
        bool liberarDaQuarentena(std::string idAnimal);
        int promoverAnimalGlobal();
        std::string registrarNascimento(const std::string& idFemea, const std::string& nomeArea, char sexo, double peso, std::string id = "");
        void adicionarAnimalNaArea(const std::string& nomeArea, Animal* a, bool silencioso = false);
        std::string gerarProximoId(const std::string& tipo);
        void realizarCobertura(const std::string& idMacho, const std::string& idFemea);
        void registrarNascimento(const std::string& idFemea, const std::string& nomeArea, std::string id, char sexo, double peso);
        void recalcularScoresGeneticos();
        void recalcularValoresVenda();
        void listarTopAnimaisPorScore(int quantidade);
        void listarTopAnimaisPorValorVenda(int quantidade);
        void emitirRelatorioOperacional();

        // ancestrais compartilhados
        void carregarAncestrais();
        void salvarAncestrais() const;
        void registrarOuAtualizarAncestral(const SnapshotAncestral& snap);
        void registrarAncestralDeAnimal(Animal* a, const std::string& status = "ativo");
        void marcarAncestralRemovido(const std::string& id, const std::string& status = "removido");
        void limpezaDeAncestrais();
        const SnapshotAncestral* buscarAncestral(const std::string& id) const;
        void exibirAncestrais(const std::string& idAnimal, int maxGeracoes = -1) const;
        void exibirFilhos(const std::string& idAnimal) const;

        // gets
        int getQuantidadeTotalAnimais();
        ContagemRebanho getContagemGlobalPorTipo() const;

        static int listarFazendasDoUsuario(User* proprietario);
        static bool existeFazendaDoUsuario(User* proprietario, const std::string& nomeFazenda);
};

#endif