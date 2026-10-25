#ifndef AREA_QUARENTENA_HPP
#define AREA_QUARENTENA_HPP
#include "area.hpp"

class AreaQuarentena: public Area {
    private:
        int nivelBioseguranca;
        std::string crmvResponsavel;
        double custoDiarioIsolamento;
        std::string* motivoIsolamento;
        Area** areasOrigem; //dentro dos atributos da area podemos
        //encontrar de onde o animal veio para encontrar seu destino ao sair da quarentena
        Animal** animaisEmQuarentena;
    
    public:
        //----
        AreaQuarentena(std::string n, double a, int bio, 
            std::string crmv);
        ~AreaQuarentena();
        //----

        double custoDiario(); //calcula o custo com base
        //no lucro que o animal deixa de gerar, gasto com racao, tratamento
        //entre outros custos que o animal gera ou prejuizos mais amplos
        void entradaEmQuarentena(Animal* a, Area* origem, 
        std::string motivo, int dias);

        Animal* darAlta(std::string idAnimal);
        //com base no id do animal retorna ele para a area de origem e remove da quarentena
        //apenas mudar o estado bool->quarentena para falso e remover da quarentena;

        void estimativaPrejuizoIsolamento();
        void calculaPrecoHospitalarDiarioTotal();
        void calculaPrecoHospitalarDiarioAnimal(std::string id);

        void listarPacientesNestaQuarentena();
};

#endif