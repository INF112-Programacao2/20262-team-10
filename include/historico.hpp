#ifndef HISTORICO_HPP
#define HISTORICO_HPP

#include <string>
#include "registro.hpp"
#include "vacina.hpp"

class HistoricoSanitario{
    private:

        RegistroSanitario** registros;
        int capacidadeTotal;
        int quantidadeAtual;

        void aumentarCapacidade();

    public: 

        HistoricoSanitario();

        ~HistoricoSanitario();

        bool registrarVacina(int idAnimal, const Vacina& vacina, int dia);


};

#endif

