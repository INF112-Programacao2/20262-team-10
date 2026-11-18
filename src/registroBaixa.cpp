#include <iostream>
#include "registrobaixa.hpp"

RegistroBaixa::RegistroBaixa(std::string idAnimal, std::string tipoAnimal, TipoBaixa tipoBaixa,
    double valorRecebido, double custoInvestido, int diaBaixa)
    :idAnimal_(idAnimal), tipoAnimal_(tipoAnimal), tipoBaixa_(tipoBaixa),
    valorRecebido_(valorRecebido), custoInvestido_(custoInvestido), diaBaixa_(diaBaixa) {
        
    // morte nao gera dinheiro
    if (tipoBaixa_ == MORTE) {
        valorRecebido_ = 0;
    }
}

std::string RegistroBaixa::get_IdAnimal() const{ 
    return idAnimal_; 
}
std::string RegistroBaixa::get_TipoAnimal() const{
    return tipoAnimal_; 
}
TipoBaixa RegistroBaixa::get_TipoBaixa() const{
    return tipoBaixa_; 
}
double RegistroBaixa::get_ValorRecebido() const{
    return valorRecebido_; }

double RegistroBaixa::get_CustoInvestido() const{
    return custoInvestido_; }

int RegistroBaixa::get_DiaBaixa() const{
    return diaBaixa_; }

bool RegistroBaixa::foiVenda() const{
    return tipoBaixa_ == VENDA; }

bool RegistroBaixa::foiMorte() const{
    return tipoBaixa_ == MORTE; }

double RegistroBaixa::get_Resultado() const {
    return valorRecebido_ - custoInvestido_;
}

void RegistroBaixa::exibir() const{
    std::cout << "Dia " << diaBaixa_ << " | "
              << (foiVenda() ? "VENDA" : "MORTE") << " | "
              << tipoAnimal_ << " " << idAnimal_
              << " | recebido R$ " << valorRecebido_
              << " | investido R$ " << custoInvestido_
              << " | resultado R$ " << get_Resultado()
              << (get_Resultado() >= 0 ? " (lucro)" : " (prejuizo)")
              << std::endl;
}