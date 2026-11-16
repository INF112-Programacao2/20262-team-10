#include "vacina.hpp"

Vacina::Vacina(std::string nomeVac, std::string tipoVac, int intervaloDoses, double custo, int diasCarencia)
    :nomeVac_(nomeVac), tipoVac_(tipoVac), intervaloDoses_(intervaloDoses), custo_(custo), diasCarencia_(diasCarencia) {}

std::string Vacina::get_Nome() const{
    return nomeVac_;
}

std::string Vacina::get_Tipo() const{
    return tipoVac_;
}

int Vacina::get_Intervalo() const{
    return intervaloDoses_;
}

double Vacina::get_custo() const{
    return custo_;
}

int Vacina::get_diasCarencia() const{
    return diasCarencia_;
}