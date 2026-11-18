#include <iostream>
#include "vaca.hpp"

Vaca::Vaca(std::string id, char s, double p, int iM, int qtdF, int qtdV) : Animal(id, s, p, iM, qtdF, qtdV) {}

Vaca::~Vaca(){

}

//gets
double Vaca:: getProducaoLeiteDia()const {return producaoLeiteDia;}
bool Vaca:: getEmLactacao()const {return emLactacao;}
std::string Vaca::getTipo() const {return "vaca";}
std::string Vaca::getIdTouroPai() const {return idTouroPai;}
bool Vaca::podeSerPromovido() const {return false;}
bool Vaca::getPrenha()const {return prenha;}
//sets
void Vaca:: setProducaoLeiteDia(double p) {producaoLeiteDia=p;}
void Vaca:: setEmLactacao(bool l) {emLactacao=l;}

double Vaca::calcularGastoDiario([[maybe_unused]] double precoLeite, SacaRacao** relatorioRacao) {
    double precoRacao = 0;
    double pesoSaco = 0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == "Racao Avulsa") {
            precoRacao = relatorioRacao[i]->preco;
            pesoSaco = relatorioRacao[i]->peso;
        }
    }
    if(emLactacao){ //vacas em lactacao tem consumo por kilo corporal mais elevado
        return (peso * 0.028)*precoRacao/pesoSaco;
    }
    return (peso*0.022)*precoRacao/pesoSaco;
}

double Vaca::calcularReceitaDiaria(double valorMercado) {
    if(emLactacao && !EmQuarentena){
        return producaoLeiteDia * valorMercado;
    }
    return 0.0;
}

bool Vaca::emprenhar(Touro* macho) {
    if(macho==nullptr) throw std::invalid_argument("Touro invalido!");
    if(prenha) throw std::runtime_error("Esta vaca ja esta prenha!");
    if(idadeMeses < 15) throw std::runtime_error("A vaca ainda nao atingiu a idade minima para reproducao (15 meses).");
    prenha = true;
}

std::string Vaca::parir() {
    if(!prenha) throw std::runtime_error("Esta vaca nao esta prenha!");
    prenha = false;
    std::string PaiRegistrado = idTouroPai;
    idTouroPai = "";
    emLactacao = true;
    return PaiRegistrado;
 }

void Vaca::adicionarFilho(std::string idFilho){
    std::string* novoArray = new std::string[qtdFilhos + 1];
    for (int i = 0; i <qtdFilhos;i++) {
        novoArray[i] = idFilhos[i];
    }
    novoArray[qtdFilhos] = idFilho;
    delete[] idFilhos;
    idFilhos = novoArray;
    qtdFilhos++;
}


