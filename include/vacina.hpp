#ifndef VACINA_HPP
#define VACINA_HPP

#include <string>

class Vacina{
    private:

        std::string nomeVacina;
        std::string tipoVacina; 
        int intervaloDosesVacina;


    public:

        Vacina(std::string nome, std::string tipo, int intervalo_doses);

        std::string get_Nome() const;
        std::string get_Tipo() const;
        int get_Intervalo() const;
};

#endif