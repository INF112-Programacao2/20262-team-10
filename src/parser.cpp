#include "parserplanilha.hpp"
#include "farm.hpp"
#include "area.hpp"
#include "vaca.hpp"
#include "touro.hpp"
#include "boicorte.hpp"
#include "bezerro.hpp"
#include <fstream>
#include <stdexcept>
#include <cstdlib>


std::string ParserPlanilha::splitManual(std::string linha, char delim, int tamOut){
    int colunaAtual = 0;
    std::string token = "";

    for(size_t i = 0; i < linha.length(); i++) {
        if(linha[i] == '\r' || linha[i] == '\n') continue;

        if (linha[i] == delim) {
            if (colunaAtual == tamOut) return token;
            colunaAtual++;
            token = "";
        }else {
            token += linha[i];
        }

    }
    if(colunaAtual == tamOut) return token;
    return "";
}
int ParserPlanilha::importarCSV(std::string arquivo, Farm* f) 
{
    if (f == nullptr) {
        throw std::invalid_argument("Ptr nulo de farm ao importar");
    }
    std::ifstream in(arquivo);
    if(!in.is_open()){
        throw std::runtime_error("Nn abriu o csv" + arquivo);
    }
    std::string linha;
    int registrosImportados = 0;
    Area* areaAtual = nullptr;
    while (std::getline(in, linha)) {
        if(linha.empty() || linha[0] == '#') continue;
        std::string tipo = splitManual(linha, ';',0);
        if (tipo == "AREA") {
            // Formato: AREA;nome;estadoPasto;area -----> ps:"manter em nome da paz mundial" <------
            std::string nome = splitManual(linha, ';', 1);
            std::string estadoPasto = splitManual(linha, ';', 2);
            double areaHectares = std::stod(splitManual(linha, ';', 3));

            Area* novaArea = new Area(nome, estadoPasto, areaHectares);
            try {
                f->adicionarArea(novaArea);
                areaAtual = novaArea;
                registrosImportados++;
            } catch (...) {
                delete novaArea; // sem vazamento se area nn passar na farm
                throw;
            }
        } 
        else {
            if (areaAtual == nullptr) {
                throw std::runtime_error("Animal declarado antes da AREA.");
            }

            std::string id = splitManual(linha, ';', 1);
            std::string sexoStr = splitManual(linha, ';', 2);
            if (sexoStr.empty()) {
                throw std::runtime_error("Sexo nao informado para o animal: " + id);
            }
            char sexo = sexoStr[0];
            double peso = std::stod(splitManual(linha, ';', 3));
            Animal* novoAnimal = nullptr;

            if (tipo == "VACA") {
                // Formato: VACA;id;sexo;peso;idadeMeses;qtdFilhos;qtdVacinas
                int idadeMeses = std::stoi(splitManual(linha, ';', 4));
                int qtdFilhos = std::stoi(splitManual(linha, ';', 5));
                int qtdVacinas = std::stoi(splitManual(linha, ';', 6));
                novoAnimal = new Vaca(id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas);
            } 
            else if (tipo == "TOURO") {
                // Formato: TOURO;id;sexo;peso;idadeMeses;qtdFilhos;qtdVacinas;valorDoseSemen
                int idadeMeses = std::stoi(splitManual(linha, ';', 4));
                int qtdFilhos = std::stoi(splitManual(linha, ';', 5));
                int qtdVacinas = std::stoi(splitManual(linha, ';', 6));
                double valorDoseSemen = std::stod(splitManual(linha, ';', 7));
                novoAnimal = new Touro(id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas, valorDoseSemen);
            }
            else if (tipo == "BOI" || tipo == "BOICORTE") {
                // Formato: BOICORTE;id;sexo;peso;idadeMeses;qtdFilhos;qtdVacinas
                int idadeMeses = std::stoi(splitManual(linha, ';', 4));
                int qtdFilhos = std::stoi(splitManual(linha, ';', 5));
                int qtdVacinas = std::stoi(splitManual(linha, ';', 6));
                novoAnimal = new Boicorte(id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas);
            }
            else if (tipo == "BEZERRO") {
                // importante --> tem 2 constructs de babyboi
                // 1) Nascido:   BEZERRO;id;sexo;peso
                // 2) Adquirido: BEZERRO;id;sexo;peso;idadeMeses;qtdFilhos;qtdVacinas;consumoLeite;ganhoPeso;idadeDesmame;desmamado
                std::string colIdade = splitManual(linha, ';', 4);
                if (colIdade.empty()) {
                    novoAnimal = new Bezerro(id, sexo, peso);
                } else {
                    int idadeMeses = std::stoi(colIdade);
                    int qtdFilhos = std::stoi(splitManual(linha, ';', 5));
                    int qtdVacinas = std::stoi(splitManual(linha, ';', 6));
                    double consumoLeite = std::stod(splitManual(linha, ';', 7));
                    double ganhoPeso = std::stod(splitManual(linha, ';', 8));
                    int idadeDesmame = std::stoi(splitManual(linha, ';', 9));
                    std::string desmStr = splitManual(linha, ';', 10);
                    bool desmamado = (desmStr == "1" || desmStr == "true" || desmStr == "TRUE");
                    novoAnimal = new Bezerro(
                        id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas,
                        consumoLeite, ganhoPeso, idadeDesmame, desmamado
                    );
                }
            } 
            else {
                throw std::runtime_error("Tipo de registro desconhecido no CSV: " + tipo);
            }
            try {
                areaAtual->adicionarAnimal(novoAnimal);
                registrosImportados++;
            } catch (...) {
                delete novoAnimal;
                throw;
            }
        }
    }

    in.close();
    return registrosImportados;
}

bool ParserPlanilha::exportarCSV(std::string arquivo, Farm* f) {
    if (f == nullptr) return false;

    std::ofstream out(arquivo);
    if (!out.is_open()) {
        throw std::runtime_error("Erro ao criar arquivo de exportacao: " + arquivo);
    }

    out << "# RELATORIO EXPORTADO FARMMANAGER\n";
    out << "TOTAL_ANIMAIS;" << f->getQuantidadeTotalAnimais() << "\n";
    out << "RECEITA_DIARIA;" << f->calcularReceitaTotalDiariaFazenda() << "\n";
    out << "GASTO_DIARIO;" << f->calcularGastoTotalDiarioFazenda() << "\n";
    out << "LUCRO_DIARIO;" << f->calcularLucroGeralDiario() << "\n";

    out.close();
    return true;
}