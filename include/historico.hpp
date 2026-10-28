#ifndef HISTORICO_HPP
#define HISTORICO_HPP

#include <string>
#include "registro.hpp"
class HistoricoSanitario{
    private:

        RegistroSanitario** registros;

    public: 

        HistoricoSanitario(int id, std::string nome, int dia_aplic, int numero_d);

        int getId();
        std::string get_nomeVacina();
        int getDiaAplic();
        int getNumeroDose();
 
};

#endif

