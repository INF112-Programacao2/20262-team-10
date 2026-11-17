#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include "vacina.hpp"

#include <string>

class RegistroSanitario{
    private:

        Vacina vacina_;
        int diaAplicacao_;
        int numeroDose_;
        int diasCarenciaRestantes_;

    public: 

        RegistroSanitario(const Vacina& vacina, int diaAplicacao, int numeroDose);

        std::string get_nomeVacina() const;
        int get_DiaAplic() const;
        int get_NumeroDose() const;
        double get_Custo() const;
        int get_DiasCarenciaRestantes() const;
        std::string get_Tipo() const;

        void avancarDia();
        bool emCarencia() const;

        

};
#endif

