#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include "farm.hpp"
#include "touro.hpp"

Touro::Touro(std::string id, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais, double vDS)
: Animal(id, 'M', p, iM, qtdF, qtdV, filhosIniciais), valorDoseSemen(vDS), dosesEstoque(0), qtdCoberturas(0) {
    if (vDS < 0.0) throw std::invalid_argument("O valor da dose de semen nao pode ser negativo!");
}

Touro::Touro() : Animal(), valorDoseSemen(0.0), dosesEstoque(0), qtdCoberturas(0) {
    sexo = 'M';
}

Touro::~Touro(){}

//gets
double Touro:: getValorDoseSemen()const {return valorDoseSemen;}
int Touro:: getDosesEstoque()const {return dosesEstoque;}
int Touro:: getQtdCoberturas()const {return qtdCoberturas;}
std::string Touro::getTipo() const {return "touro";}
bool Touro::podeSerPromovido() const {return false;} 
//sets
void Touro::setValorDoseSemen(double v) {
    if (v < 0.0) throw std::invalid_argument("O valor da dose de semen nao pode ser negativo!");
    valorDoseSemen = v;
}

void Touro::setDosesEstoque(int d) {
    if (d < 0) throw std::invalid_argument("O estoque de doses nao pode ser negativo!");
    dosesEstoque = d;
}

void Touro::setQtdCoberturas(int q) {
    if (q < 0) throw std::invalid_argument("A quantidade de coberturas nao pode ser negativa!");
    qtdCoberturas = q;
}

std::string Touro::getTipoRacao() const {
    return "Racao Avulsa";
}

double Touro::calcularSacosDiarios(SacaRacao** relatorioRacao) const {
    if (relatorioRacao == nullptr) return 0.0;
    double pesoSaco = 0.0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i] != nullptr && relatorioRacao[i]->name == "Racao Avulsa") {
            pesoSaco = relatorioRacao[i]->peso;
            break;
        }
    }
    if (pesoSaco <= 0.0) return 0.0;

    return (peso * 0.020) / pesoSaco;
}

double Touro::calcularGastoDiario([[maybe_unused]] double precoLeite, SacaRacao** relatorioRacao) {
    if (relatorioRacao == nullptr) return 0.0;
    double precoRacao = 0.0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i] != nullptr && relatorioRacao[i]->name == "Racao Avulsa") {
            precoRacao = relatorioRacao[i]->preco;
            break;
        }
    }
    return calcularSacosDiarios(relatorioRacao) * precoRacao;
}

bool Touro::cobrirFemea(Vaca* femea) {
    if (femea == nullptr) throw std::invalid_argument("Vaca invalida para cobertura!");
    if (idadeMeses < 15) throw std::runtime_error("O touro ainda nao atingiu a idade minima para reproducao (15 meses).");

    femea->emprenhar(this);
    qtdCoberturas++;
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

double Touro::calcularReceitaDiaria([[maybe_unused]] double valorMercado) {
    return 0.0;
}

void Touro::salvarEmArquivo() {
    if(_id.empty()) throw std::invalid_argument("ID do touro invalido!");
    std::filesystem::create_directories("./data");
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
                  (leitura >> aux.valorDoseSemen >> aux.dosesEstoque >> aux.qtdCoberturas)) {
                if(aux._id == _id) {
                    escreverBase(temp);
                    temp << " " << valorDoseSemen << " " << dosesEstoque << " " << qtdCoberturas << "\n";
                    touroAtualizado = true;
                } else {
                    aux.escreverBase(temp);
                    temp << " " << aux.valorDoseSemen << " " << aux.dosesEstoque << " " << aux.qtdCoberturas << "\n";
                }
            }
            leitura.close();
        }

        if(!touroAtualizado) {
            escreverBase(temp);
            temp << " " << valorDoseSemen << " " << dosesEstoque << " " << qtdCoberturas << "\n";
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
              (arquivo >> touroCarregado->valorDoseSemen >> touroCarregado->dosesEstoque >> touroCarregado->qtdCoberturas)) {
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

void Touro::imprimirFichaParcial() const{
    std :: cout << "===Animal:" << getTipo() << "\n"
    << "ID:" << _id << "\nSexo:" << sexo << "\nPeso:" << peso
    << "\nIdade:" << idadeMeses << "\nScore genetico:" << scoreGenetico <<"\nValor Dose Material Genetico:"
    << valorDoseSemen << "\n";
}

double Touro::calcularValorVendaMercado(double precoArroba) {
    if (precoArroba < 0) throw std::invalid_argument("O preco do arroba nao pode ser negativo!");
    double valorCarne = (peso / 30.0) * precoArroba;
    double valorBancoSemen = (dosesEstoque * valorDoseSemen) * 0.7;

    if (idadeMeses > 84) {
        return valorCarne + valorBancoSemen;
    }
    return (valorCarne * getFatorPrecoGenetico()) + valorBancoSemen;
}