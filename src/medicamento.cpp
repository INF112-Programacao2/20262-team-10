#include "medicamento.hpp"

#include <stdexcept>

Medicamento::Medicamento(std::string nome, CategoriaMedicamento categoria, int intervaloDoses, double custo, int diasCarencia)
    :nome_(nome), categoria_(categoria), intervaloDoses_(intervaloDoses), custo_(custo), diasCarencia_(diasCarencia){

    if (nome_.empty()) throw std::invalid_argument("O nome do medicamento nao pode ser vazio!");
    if (intervaloDoses_ < 0) throw std::invalid_argument("O intervalo entre doses nao pode ser negativo!");
    if (custo_ < 0.0) throw std::invalid_argument("O custo do medicamento nao pode ser negativo!");
    if (diasCarencia_ < 0) throw std::invalid_argument("Os dias de carencia nao podem ser negativos!");
}

std::string Medicamento::get_Nome() const{
    return nome_;
}

std::string Medicamento::get_Tipo() const{
    return categoriaParaTexto(categoria_);
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