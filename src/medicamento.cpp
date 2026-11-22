#include "medicamento.hpp"

#include <stdexcept>

Medicamento::Medicamento(std::string nome, CategoriaMedicamento categoria, int intervaloDoses, double custo, int diasCarencia)
    :nome_(nome), categoria_(categoria), intervaloDoses_(intervaloDoses), custo_(custo), diasCarencia_(diasCarencia){

    if (nome_.empty()) throw std::invalid_argument("O nome do medicamento não pode ser vazio!");
    if (intervaloDoses_ < 0) throw std::invalid_argument("O intervalo entre doses não pode ser negativo!");
    if (custo_ < 0.0) throw std::invalid_argument("O custo do medicamento não pode ser negativo!");
    if (diasCarencia_ < 0) throw std::invalid_argument("Os dias de carencia não podem ser negativos!");
}

std::string Medicamento::get_Nome() const{
    return nome_;
}

std::string Medicamento::get_Tipo() const{
    switch (categoria_){
        case VACINA:            return "Vacina";
        case ANTIBIOTICO:       return "Antibiotico";
        case VERMIFUGO:         return "Vermifugo";
        case ANTI_INFLAMATORIO: return "Anti-inflamatorio";
        default:                return "Outro";
    }
}

CategoriaMedicamento Medicamento::get_Categoria() const{
    return categoria_;
}

int Medicamento::get_Intervalo() const{
    return intervaloDoses_;
}

double Medicamento::get_custo() const{
    return custo_;
}

int Medicamento::get_diasCarencia() const{
    return diasCarencia_;
}

bool Medicamento::ehVacina() const{
    return categoria_ == VACINA;
}