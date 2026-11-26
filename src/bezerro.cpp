#include <iostream>
#include <fstream>
#include <filesystem>
#include "bezerro.hpp"

Bezerro::Bezerro(std::string id, char s, double p) : Animal(id, s, p) {
    consumoLeiteLitros = 4.0;
    ganhoPesoMedioDiario = 0.7;
    desmamado = false;
    idadeDesmameMeses = 0;
}

Bezerro::Bezerro() : Animal(), consumoLeiteLitros(0.0), ganhoPesoMedioDiario(0.0), idadeDesmameMeses(0), desmamado(false) {}

Bezerro::Bezerro(std::string id, char s, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais, double cLL, double gPMD, int iDM, bool d) 
: Animal(id, s, p, iM, qtdF, qtdV, filhosIniciais), consumoLeiteLitros(cLL), ganhoPesoMedioDiario(gPMD), idadeDesmameMeses(iDM), desmamado(d) {}

Bezerro::~Bezerro() {}

std::string Bezerro::getTipoRacao() const {
    if (desmamado) {
        return "Racao Recria";
    }
    return "";
}

double Bezerro::calcularSacosDiarios(SacaRacao** relatorioRacao) const {
    if (!desmamado || relatorioRacao == nullptr) return 0.0;
    double pesoSaco = 0.0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == "Racao Recria") {
            pesoSaco = relatorioRacao[i]->peso;
            break;
        }
    }
    if (pesoSaco <= 0.0) return 0.0;

    return (peso * 0.028) / pesoSaco;
}

//gets
double Bezerro::getConsumoLeiteLitros() const { return consumoLeiteLitros; }
double Bezerro::getGanhoPesoMedioDiario() const { return ganhoPesoMedioDiario; }
int Bezerro::getIdadeDesmameMeses() const { return idadeDesmameMeses; }
bool Bezerro::getDesmamado() const { return desmamado; }
std::string Bezerro::getTipo() const { return "bezerro"; }
bool Bezerro::podeSerPromovido() const { return atingiuIdadeAdulta(); }
//sets
void Bezerro::setConsumoLeiteLitros(double c) {
    if (c < 0.0) throw std::invalid_argument("O consumo de leite nao pode ser negativo!");
    consumoLeiteLitros = c;
}

void Bezerro::setGanhoPesoMedioDiario(double g) {
    if (g < 0.0) throw std::invalid_argument("O ganho de peso diario nao pode ser negativo!");
    ganhoPesoMedioDiario = g;
}

void Bezerro::setIdadeDesmameMeses(int i) {
    if (i < 0) throw std::invalid_argument("A idade de desmame nao pode ser negativa!");
    idadeDesmameMeses = i;
}

void Bezerro::setDesmamado(bool d) {
    desmamado = d;
    if (desmamado) {
        consumoLeiteLitros = 0.0;
    } else if (consumoLeiteLitros <= 0.0) {
        consumoLeiteLitros = 4.0;
    }
}

void Bezerro::realizarDesmame() {
    if (desmamado) {
        std::cout << "Este bezerro ja foi desmamado aos " << idadeDesmameMeses << " meses.\n";
        return;
    }
    if (idadeMeses >= 7) {
        desmamado = true;
        idadeDesmameMeses = idadeMeses;
        consumoLeiteLitros = 0.0;
    } else {
        std::cout << "O bezerro nao esta com a idade adequada para o desmame.\n";
    }
}

bool Bezerro::atingiuIdadeAdulta() const {
    if(idadeMeses >=12){
        return true;
    }
    else{
        return false;
    }
}
double Bezerro::calcularGastoDiario(double precoLeite, SacaRacao** relatorioRacao) {
    if (!desmamado) { // por hora 1.5 e um custo fixo operacional puramente especulativo
        if (precoLeite <= 0.0) return 1.5;
        return (consumoLeiteLitros * precoLeite) + 1.5;
    }

    if (relatorioRacao == nullptr) return 0.0;
    double precoRacao = 0.0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == "Racao Recria") {
            precoRacao = relatorioRacao[i]->preco;
            break;
        }
    }
    if (precoRacao <= 0.0) return 0.0;

    return calcularSacosDiarios(relatorioRacao) * precoRacao;
}

double Bezerro::calcularReceitaDiaria(double valorMercado) {
    double agio = 0.05; // calculo com base na idade do animal ganho de peso diario
    if(idadeMeses<=8) (sexo=='M') ? agio=0.22 : agio=0.15;
    else if(idadeMeses<=14) (sexo=='M') ? agio=0.12 : agio=0.08;
    return (ganhoPesoMedioDiario/30.0) * valorMercado * (1+agio);
}

// macho vira touro se o score for >=60, senao vira boi de corte
std::string Bezerro::recomendarDestinoMaturidade() {
    const double SCORE_MINIMO_TOURO = 60.0;
    if(sexo=='F') return "vaca";
    return (scoreGenetico >= SCORE_MINIMO_TOURO) ? "touro" : "boicorte";
}

void Bezerro::salvarEmArquivo() {
    if(_id.empty()) throw std::invalid_argument("ID do bezerro invalido!");
    std::filesystem::create_directories("./data");
    std::ifstream leitura("./data/bezerros.txt");
    std::ofstream temp("./data/bezerros_temp.txt");
    if(!temp.is_open()) {
        throw std::runtime_error("Falha ao criar arquivo temporario de bezerros.");
    }

    bool bezerroAtualizado = false;

    try {
        if(leitura.is_open()) {
            Bezerro aux;
            while(aux.carregarBase(leitura) &&
                  (leitura >> aux.consumoLeiteLitros >> aux.ganhoPesoMedioDiario >> aux.idadeDesmameMeses >> aux.desmamado)) {
                if(aux._id == _id) {
                    escreverBase(temp);
                    temp << " " << consumoLeiteLitros << " " << ganhoPesoMedioDiario << " " << idadeDesmameMeses << " " << desmamado << "\n";
                    bezerroAtualizado = true;
                } else {
                    aux.escreverBase(temp);
                    temp << " " << aux.consumoLeiteLitros << " " << aux.ganhoPesoMedioDiario << " " << aux.idadeDesmameMeses << " " << aux.desmamado << "\n";
                }
            }
            leitura.close();
        }

        if(!bezerroAtualizado) {
            escreverBase(temp);
            temp << " " << consumoLeiteLitros << " " << ganhoPesoMedioDiario << " " << idadeDesmameMeses << " " << desmamado << "\n";
        }
    } catch (...) {
        if(leitura.is_open()) leitura.close();
        temp.close();
        std::remove("./data/bezerros_temp.txt");
        throw;
    }

    temp.close();
    std::remove("./data/bezerros.txt");
    std::rename("./data/bezerros_temp.txt", "./data/bezerros.txt");
}

Bezerro* Bezerro::carregarBezerro(std::string id) {
    if(id.empty()) throw std::invalid_argument("ID invalido para carregar bezerro!");

    std::ifstream arquivo("./data/bezerros.txt");
    if(!arquivo.is_open()) {
        return nullptr;
    }

    Bezerro* bezerroCarregado = new Bezerro();
    try {
        while(bezerroCarregado->carregarBase(arquivo) &&
              (arquivo >> bezerroCarregado->consumoLeiteLitros >> bezerroCarregado->ganhoPesoMedioDiario >> bezerroCarregado->idadeDesmameMeses >> bezerroCarregado->desmamado)) {
            if(bezerroCarregado->_id == id) {
                arquivo.close();
                return bezerroCarregado;
            }
        }
    } catch (...) {
        arquivo.close();
        delete bezerroCarregado;
        throw;
    }

    arquivo.close();
    delete bezerroCarregado;
    return nullptr;
}

void Bezerro::imprimirFichaParcial() const{
    std :: cout << "===Animal:" << getTipo() << "\n"
    << "ID:" << _id << "\nSexo:" << sexo << "\nPeso:" << peso 
    << "\nIdade:" << idadeMeses << "\nScore genetico:" << scoreGenetico << "\nDesmamado:";
    (desmamado) ? std::cout << "SIM" : std::cout << "NAO";
    std::cout <<"\n";
}