#ifndef VACINA_HPP
#define VACINA_HPP

#include <string>

class Vacina{
    private:

        std::string nomeVacina;
        std::string tipoVacina; 
        int intervalo_dosesVacina;


    public:

        Vacina(std::string nome, std::string tipo, int intervalo_doses);

        std::string getNome();
        std::string getTipo();
        int getIntervalo();
};

#endif