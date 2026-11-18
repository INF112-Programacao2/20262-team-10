#ifndef REGISTROBAIXA_HPP
#define REGISTROBAIXA_HPP

#include <string>

enum TipoBaixa { VENDA, MORTE };   //baixa     

class RegistroBaixa {
    private:
        std::string idAnimal_;
        std::string tipoAnimal_;   
        TipoBaixa tipoBaixa_;      
        double valorRecebido_;     // dinheiro recebido na venda 
        double custoInvestido_;    // dinheiro total gasto com o animal ate a baixa
        int diaBaixa_;             // dia da simulacao em que ocorreu

    public:
        RegistroBaixa(std::string idAnimal, std::string tipoAnimal, TipoBaixa tipoBaixa,
                      double valorRecebido, double custoInvestido, int diaBaixa);

        std::string get_IdAnimal() const;
        std::string get_TipoAnimal() const;
        TipoBaixa get_TipoBaixa() const;
        double get_ValorRecebido() const;
        double get_CustoInvestido() const;
        int get_DiaBaixa() const;

        bool foiVenda() const;
        bool foiMorte() const;

        double get_Resultado() const;

        void exibir() const;
};

#endif