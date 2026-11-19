#include <iostream>
#include <fstream>
#include "vaca.hpp"

Vaca::Vaca(std::string id, char s, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais) 
: Animal(id, s, p, iM, qtdF, qtdV, filhosIniciais), producaoLeiteDia(0.0), emLactacao(false), prenha(false) {}

Vaca::Vaca() : Animal(), producaoLeiteDia(0.0), emLactacao(false), prenha(false) {}

Vaca::~Vaca(){}

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
    idTouroPai = macho->getId();
    return true;
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

