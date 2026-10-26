#ifndef HISTORICO_HPP
#define HISTORICO_HPP

#include <string>

class HistoricoSanitario{
    private:

        std::RegistroSanitario** registros



    public: 

        HistoricoSanitario(int id, std::string nome; int dia_aplic, int numero_d);

        int getId();
        std::string get_nomeVacina();
        int getDiaAplic();
        int getNumeroDose();

      
};
#endif

