#ifndef AREA_QUARENTENA_HPP
#define AREA_QUARENTENA_HPP

#include "area.hpp"
#include "animal.hpp"
#include "sacaRacao.hpp"

#include <string>

class AreaQuarentena : public Area {
    private:

         struct FichaIsolamento{

            std::string idAnimal_;
            std::string motivo_;
            std::string areaOrigem_;         //nome da area de onde o animal veio
            int diasRestantes_;
            double receitaPerdidaDiaria_;     //receita que o animal deixou de gerar
        };

        FichaIsolamento* fichas_;
        int qtdFichas_;
        int nivelBioseguranca_;
        std::string crmvResponsavel_;
        double custoDiarioIsolamento_;

        int buscarFicha(const std::string& idAnimal) const;
        void adicionarFicha(const FichaIsolamento& ficha);
        void removerFicha(int indice);

    
    public:
        //----
        AreaQuarentena(std::string nome, double hectares, int nivelBioseguranca, std::string crmvResponsavel);
        ~AreaQuarentena();
        //----


        //calcula o custo com base no lucro que o animal deixa de gerar, gasto com racao, tratamento entre outros custos que o animal gera ou prejuizos mais amplos
        double custoDiario(double precoLeite, SacaRacao** relatorioRacoes);
        void entradaEmQuarentena(Animal* a, Area* origem, std::string motivo, int dias, double receitaDiariaPerdida);


        void avancarDia();
        bool prontoAlta(std::string idAnimal) const;

        //com base no id do animal retorna ele para a area de origem e remove da quarentena
        //apenas mudar o estado bool->quarentena para falso e remover da quarentena;

        Animal* darAlta(std::string idAnimal);
        

        double estimativaPrejuizoIsolamento();
        double calculaPrecoHospitalarDiarioTotal();
        double calculaPrecoHospitalarDiarioAnimal(std::string id);
        void listarPacientesNestaQuarentena() const;

        //getters

        std::string getAreaOrigem(std::string idAnimal) const;
        int getNivelBioseguranca() const;
        std::string getCrmvResponsavel() const;
        int getQtdPacientes() const;
        int getDiasRestantes(std::string idAnimal) const;
        double getGastoDaArea(double precoLeite, SacaRacao** relatorioRacoes) override;
        
};


        

#endif