#include <iostream>
#include <fstream>
#include "vaca.hpp"

Vaca::Vaca(std::string id, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais)
: Animal(id, 'F', p, iM, qtdF, qtdV, filhosIniciais), producaoLeiteDia(0.0), emLactacao(false), prenha(false) {}

Vaca::Vaca() : Animal(), producaoLeiteDia(0.0), emLactacao(false), prenha(false) {
    sexo = 'F';
}

Vaca::~Vaca(){}

//gets
double Vaca:: getProducaoLeiteDia()const {return producaoLeiteDia;}
bool Vaca:: getEmLactacao()const {return emLactacao;}
std::string Vaca::getTipo() const {return "vaca";}
std::string Vaca::getIdTouroPai() const {return idTouroPai;}
bool Vaca::podeSerPromovido() const {return false;}
bool Vaca::getPrenha()const {return prenha;}
//sets
void Vaca::setProducaoLeiteDia(double p) {
    if (p < 0.0) throw std::invalid_argument("A producao de leite nao pode ser negativa!");
    producaoLeiteDia = p;
}

void Vaca::setEmLactacao(bool l) {
    emLactacao = l;
    if (emLactacao && producaoLeiteDia <= 0.0) {
        producaoLeiteDia = 15.0; // producao media base em litros/dia
    } else if (!emLactacao) {
        producaoLeiteDia = 0.0;
    }
}

double Vaca::calcularGastoDiario([[maybe_unused]] double precoLeite, SacaRacao** relatorioRacao) {
    if (relatorioRacao == nullptr) return 0.0;
    double precoRacao = 0.0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == "Racao Avulsa") {
            precoRacao = relatorioRacao[i]->preco;
            break;
        }
    }
    return calcularSacosDiarios(relatorioRacao) * precoRacao;
}

double Vaca::calcularReceitaDiaria(double valorMercado) {
    if (emLactacao && !EmQuarentena) {
        return producaoLeiteDia * valorMercado;
    }
    return 0.0;
}

std::string Vaca::getTipoRacao() const {
    return "Racao Avulsa";
}

double Vaca::calcularSacosDiarios(SacaRacao** relatorioRacao) const {
    if (relatorioRacao == nullptr) return 0.0;
    double pesoSaco = 0.0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == "Racao Avulsa") {
            pesoSaco = relatorioRacao[i]->peso;
            break;
        }
    }
    if (pesoSaco <= 0.0) return 0.0;

    if (emLactacao) { // vacas em lactacao tem consumo por kilo corporal mais elevado
        return (peso * 0.028) / pesoSaco;
    }
    return (peso * 0.022) / pesoSaco;
}

bool Vaca::emprenhar(Touro* macho) {
    if (macho == nullptr) throw std::invalid_argument("Touro invalido!");
    if (EmQuarentena || macho->getQuarentena()) throw std::runtime_error("Animal em quarentena nao pode reproduzir!");
    if (prenha) throw std::runtime_error("Esta vaca ja esta prenha!");
    if (idadeMeses < 15) throw std::runtime_error("A vaca ainda nao atingiu a idade minima para reproducao (15 meses).");

    if ((!idPai.empty() && idPai == macho->getPai()) ||
        (!idMae.empty() && idMae == macho->getMae()) ||
        idPai == macho->getId() || macho->getMae() == _id) {
        throw std::runtime_error("Cobertura bloqueada: parentesco proximo detectado!");
    }

    prenha = true;
    idTouroPai = macho->getId();
    return true;
}

std::string Vaca::parir() {
    if (!prenha) throw std::runtime_error("Esta vaca nao esta prenha!");
    prenha = false;
    std::string PaiRegistrado = idTouroPai;
    idTouroPai = "";
    setEmLactacao(true);
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

void Vaca::salvarEmArquivo() {
    if(_id.empty()) throw std::invalid_argument("ID da vaca invalido!");

    std::ifstream leitura("./data/vacas.txt");
    std::ofstream temp("./data/vacas_temp.txt");
    if(!temp.is_open()) {
        throw std::runtime_error("Falha ao criar arquivo temporario de vacas.");
    }

    bool vacaAtualizada = false;

    try {
        if(leitura.is_open()) {
            Vaca aux;
            while(aux.carregarBase(leitura) &&
                  (leitura >> aux.producaoLeiteDia >> aux.emLactacao >> aux.prenha >> aux.idTouroPai)) {
                aux.idTouroPai = formatarTextoCarregar(aux.idTouroPai);
                if(aux._id == _id) {
                    escreverBase(temp);
                    temp << " " << producaoLeiteDia << " " << emLactacao << " " << prenha << " " << formatarTextoSalvar(idTouroPai) << "\n";
                    vacaAtualizada = true;
                } else {
                    aux.escreverBase(temp);
                    temp << " " << aux.producaoLeiteDia << " " << aux.emLactacao << " " << aux.prenha << " " << formatarTextoSalvar(aux.idTouroPai) << "\n";
                }
            }
            leitura.close();
        }

        if(!vacaAtualizada) {
            escreverBase(temp);
            temp << " " << producaoLeiteDia << " " << emLactacao << " " << prenha << " " << formatarTextoSalvar(idTouroPai) << "\n";
        }
    } catch (...) {
        if(leitura.is_open()) leitura.close();
        temp.close();
        std::remove("./data/vacas_temp.txt");
        throw;
    }

    temp.close();
    std::remove("./data/vacas.txt");
    std::rename("./data/vacas_temp.txt", "./data/vacas.txt");
}

Vaca* Vaca::carregarVaca(std::string id) {
    if(id.empty()) throw std::invalid_argument("ID invalido para carregar vaca!");

    std::ifstream arquivo("./data/vacas.txt");
    if(!arquivo.is_open()) {
        return nullptr;
    }

    Vaca* vacaCarregada = new Vaca();
    try {
        while(vacaCarregada->carregarBase(arquivo) &&
              (arquivo >> vacaCarregada->producaoLeiteDia >> vacaCarregada->emLactacao >> vacaCarregada->prenha >> vacaCarregada->idTouroPai)) {
            vacaCarregada->idTouroPai = vacaCarregada->formatarTextoCarregar(vacaCarregada->idTouroPai);
            if(vacaCarregada->_id == id) {
                arquivo.close();
                return vacaCarregada;
            }
        }
    } catch (...) {
        arquivo.close();
        delete vacaCarregada;
        throw;
    }

    arquivo.close();
    delete vacaCarregada;
    return nullptr;
}

void Vaca::imprimirFichaParcial() const {
    std::cout << "===Animal:" << getTipo() << "\n"
    << "ID:" << _id << "\nSexo:" << sexo << "\nPeso:" << peso
    << "\nIdade:" << idadeMeses << "\nEm Lactacao:";
    (emLactacao) ? std::cout << "SIM" : std::cout << "NAO";
    std::cout << "\n";
}