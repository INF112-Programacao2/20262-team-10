#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include "medicamento.hpp"

#include <string>

class RegistroSanitario{
    private:

        Medicamento medicamento_;
        int diaAplicacao_;
        int numeroDose_;
        int diasCarenciaRestantes_;

    public: 

        RegistroSanitario(const Medicamento& vacina, int diaAplicacao, int numeroDose);

        std::string get_nomeMedicamento() const;
        int get_DiaAplic() const;
        int get_NumeroDose() const;
        double get_Custo() const;
        int get_DiasCarenciaRestantes() const;
        std::string get_Tipo() const;
        CategoriaMedicamento get_Categoria() const;

        void avancarDia();
        bool emCarencia() const;

        

};
#endif

