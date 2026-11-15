#ifndef VACINA_HPP
#define VACINA_HPP

#include <string>

class Vacina{
    private:

        std::string nomeVac_;
        std::string tipoVac_; 
        int intervaloDoses_;
        double custo_;
        int diasCarencia_;


    public:

        Vacina(std::string nomeVac, std::string tipoVac, int intervaloDoses, double custo, int diasCarencia);

        std::string get_Nome() const;
        std::string get_Tipo() const;
        int get_Intervalo() const;
        double get_custo() const;
        int get_diasCarencia() const;
};

#endif