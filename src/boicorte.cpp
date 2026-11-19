#include <iostream>
#include <fstream>
#include "boicorte.hpp"

Boicorte::Boicorte(std::string id, char s, double p, int iM, int qtdF, int qtdV) 
: Animal(id, s, p, iM, qtdF, qtdV), rendimentoCarcaca(0.0), metaAbatePeso(0.0), faseEngorda(false) {}

Boicorte::Boicorte() : Animal(), rendimentoCarcaca(0.0), metaAbatePeso(0.0), faseEngorda(false) {}
Boicorte::~Boicorte() {}

//gets
double Boicorte:: getrendimentoCarcaca()const {return rendimentoCarcaca;}
double Boicorte:: getmetaAbatePeso()const {return metaAbatePeso;}
bool Boicorte:: getFaseEngorda()const {return faseEngorda;}
std::string Boicorte:: getTipo() const {return "boicorte";}
bool Boicorte:: podeSerPromovido() const {return false;}
//sets
void Boicorte:: setrendimentoCarcaca(double r) {rendimentoCarcaca=r;}
void Boicorte:: setmetaAbatePeso(double m) {metaAbatePeso=m;}
void Boicorte:: setFaseEngorda(bool f) {faseEngorda=f;}

std::string Boicorte::getTipoRacao() const {
    if (faseEngorda) {
        return "Racao Engorda";
    }
    return "Racao Avulsa";
}

double Boicorte::calcularSacosDiarios(SacaRacao** relatorioRacao) const {
    if (relatorioRacao == nullptr) return 0.0;
    std::string tipo = getTipoRacao();
    double pesoSaco = 0.0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == tipo) {
            pesoSaco = relatorioRacao[i]->peso;
            break;
        }
    }
    if (pesoSaco <= 0.0) return 0.0;

    if (faseEngorda) {
        return (peso * 0.026) / pesoSaco;
    }
    return (peso * 0.021) / pesoSaco;
}

double Boicorte::calcularGastoDiario([[maybe_unused]] double precoLeite, SacaRacao** relatorioRacao) {
    if (relatorioRacao == nullptr) return 0.0;
    std::string tipo = getTipoRacao();
    double precoRacao = 0.0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == tipo) {
            precoRacao = relatorioRacao[i]->preco;
            break;
        }
    }
    return calcularSacosDiarios(relatorioRacao) * precoRacao;
}

double Boicorte::calcularReceitaDiaria(double valorMercado) { //precoLeite alias precoArroba
    if(faseEngorda){
        return ((rendimentoCarcaca*1.4)/15) * valorMercado;
    }
    return ((rendimentoCarcaca*0.7)/15) * valorMercado;
}

void Boicorte::salvarEmArquivo() {
    if(_id.empty()) throw std::invalid_argument("ID do boi de corte invalido!");

    std::ifstream leitura("./data/boicortes.txt");
    std::ofstream temp("./data/boicortes_temp.txt");
    if(!temp.is_open()) {
        throw std::runtime_error("Falha ao criar arquivo temporario de bois de corte.");
    }

    bool boiAtualizado = false;

    try {
        if(leitura.is_open()) {
            Boicorte aux;
            while(aux.carregarBase(leitura) &&
                  (leitura >> aux.rendimentoCarcaca >> aux.metaAbatePeso >> aux.faseEngorda)) {
                if(aux._id == _id) {
                    escreverBase(temp);
                    temp << " " << rendimentoCarcaca << " " << metaAbatePeso << " " << faseEngorda << "\n";
                    boiAtualizado = true;
                } else {
                    aux.escreverBase(temp);
                    temp << " " << aux.rendimentoCarcaca << " " << aux.metaAbatePeso << " " << aux.faseEngorda << "\n";
                }
            }
            leitura.close();
        }

        if(!boiAtualizado) {
            escreverBase(temp);
            temp << " " << rendimentoCarcaca << " " << metaAbatePeso << " " << faseEngorda << "\n";
        }
    } catch (...) {
        if(leitura.is_open()) leitura.close();
        temp.close();
        std::remove("./data/boicortes_temp.txt");
        throw;
    }

    temp.close();
    std::remove("./data/boicortes.txt");
    std::rename("./data/boicortes_temp.txt", "./data/boicortes.txt");
}

Boicorte* Boicorte::carregarBoicorte(std::string id) {
    if(id.empty()) throw std::invalid_argument("ID invalido para carregar boi de corte!");

    std::ifstream arquivo("./data/boicortes.txt");
    if(!arquivo.is_open()) {
        return nullptr;
    }

    Boicorte* boiCarregado = new Boicorte();
    try {
        while(boiCarregado->carregarBase(arquivo) &&
              (arquivo >> boiCarregado->rendimentoCarcaca >> boiCarregado->metaAbatePeso >> boiCarregado->faseEngorda)) {
            if(boiCarregado->_id == id) {
                arquivo.close();
                return boiCarregado;
            }
        }
    } catch (...) {
        arquivo.close();
        delete boiCarregado;
        throw;
    }

    arquivo.close();
    delete boiCarregado;
    return nullptr;
}