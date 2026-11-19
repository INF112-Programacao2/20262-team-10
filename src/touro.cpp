#include <iostream>
#include <fstream>
#include "farm.hpp"
#include "touro.hpp"

Touro::Touro(std::string id, char s, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais, double vDS) 
: Animal(id, s, p, iM, qtdF, qtdV, filhosIniciais), valorDoseSemen(vDS), meritoGeneticoPTA(0.0), dosesEstoque(0), qtdCoberturas(0) {}

Touro::Touro() : Animal(), valorDoseSemen(0.0), meritoGeneticoPTA(0.0), dosesEstoque(0), qtdCoberturas(0) {}

Touro::~Touro(){}

//gets
double Touro:: getValorDoseSemen()const {return valorDoseSemen;}
double Touro:: getMerito()const {return meritoGeneticoPTA;}
int Touro:: getDosesEstoque()const {return dosesEstoque;}
int Touro:: getQtdCoberturas()const {return qtdCoberturas;}
std::string Touro::getTipo() const {return "touro";}
bool Touro::podeSerPromovido() const {return false;} 
//sets
void Touro:: setValorDoseSemen(double v) {valorDoseSemen=v;}
void Touro:: setMerito(double m) {meritoGeneticoPTA=m;}
void Touro:: setDosesEstoque(int d) {dosesEstoque=d;}
void Touro:: setQtdCoberturas(int q) {qtdCoberturas=q;}

double Touro::calcularGastoDiario([[maybe_unused]] double precoLeite, SacaRacao** relatorioRacao) {
    double precoRacao = 0;
    double pesoSaco = 0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == "Racao Avulsa") {
            precoRacao = relatorioRacao[i]->preco;
            pesoSaco = relatorioRacao[i]->peso;
        }
    }
    return (peso * 0.020)*precoRacao/pesoSaco;
}

bool Touro::cobrirFemea(Vaca* femea) {
    if(femea==nullptr) throw std::invalid_argument("Vaca invalida para cobertura!");
    if(femea->getPrenha()) throw std::runtime_error("Esta vaca ja esta prenha!");
    if((!this->getPai().empty() && this->getPai() == femea->getPai()) || (!this->getMae().empty() && this->getMae() == femea->getMae())){
        throw std::runtime_error("Cobertura bloqueada: parentesco proximo detectado!");
    } 
    qtdCoberturas++;
    femea->emprenhar(this);
    return true;
}

void Touro::adicionarFilho(std::string idFilho) {
    std::string* novoArray = new std::string[qtdFilhos + 1];
    for (int i = 0; i <qtdFilhos;i++) {
        novoArray[i] = idFilhos[i];
    }
    novoArray[qtdFilhos] = idFilho;
    delete[] idFilhos;
    idFilhos = novoArray;
    qtdFilhos++;
}

double Touro::calcularReceitaDiaria(double valorMercado) {return 0.0;}

void Touro::salvarEmArquivo() {
    if(_id.empty()) throw std::invalid_argument("ID do touro invalido!");

    std::ifstream leitura("./data/touros.txt");
    std::ofstream temp("./data/touros_temp.txt");
    if(!temp.is_open()) {
        throw std::runtime_error("Falha ao criar arquivo temporario de touros.");
    }

    bool touroAtualizado = false;

    try {
        if(leitura.is_open()) {
            Touro aux;
            while(aux.carregarBase(leitura) &&
                  (leitura >> aux.valorDoseSemen >> aux.meritoGeneticoPTA >> aux.dosesEstoque >> aux.qtdCoberturas)) {
                if(aux._id == _id) {
                    escreverBase(temp);
                    temp << " " << valorDoseSemen << " " << meritoGeneticoPTA << " " << dosesEstoque << " " << qtdCoberturas << "\n";
                    touroAtualizado = true;
                } else {
                    aux.escreverBase(temp);
                    temp << " " << aux.valorDoseSemen << " " << aux.meritoGeneticoPTA << " " << aux.dosesEstoque << " " << aux.qtdCoberturas << "\n";
                }
            }
            leitura.close();
        }

        if(!touroAtualizado) {
            escreverBase(temp);
            temp << " " << valorDoseSemen << " " << meritoGeneticoPTA << " " << dosesEstoque << " " << qtdCoberturas << "\n";
        }
    } catch (...) {
        if(leitura.is_open()) leitura.close();
        temp.close();
        std::remove("./data/touros_temp.txt");
        throw;
    }

    temp.close();
    std::remove("./data/touros.txt");
    std::rename("./data/touros_temp.txt", "./data/touros.txt");
}

Touro* Touro::carregarTouro(std::string id) {
    if(id.empty()) throw std::invalid_argument("ID invalido para carregar touro!");

    std::ifstream arquivo("./data/touros.txt");
    if(!arquivo.is_open()) {
        return nullptr;
    }

    Touro* touroCarregado = new Touro();
    try {
        while(touroCarregado->carregarBase(arquivo) &&
              (arquivo >> touroCarregado->valorDoseSemen >> touroCarregado->meritoGeneticoPTA >> touroCarregado->dosesEstoque >> touroCarregado->qtdCoberturas)) {
            if(touroCarregado->_id == id) {
                arquivo.close();
                return touroCarregado;
            }
        }
    } catch (...) {
        arquivo.close();
        delete touroCarregado;
        throw;
    }

    arquivo.close();
    delete touroCarregado;
    return nullptr;
}