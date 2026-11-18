#include "parserplanilha.hpp"
#include "farm.hpp"
#include "area.hpp"
#include "vaca.hpp"
#include "touro.hpp"
#include "boicorte.hpp"
#include "bezerro.hpp"
#include <fstream>
#include <stdexcept>

std::string ParserPlanilha::splitManual(std::string linha, char delim, int tamOut) {
    int colunaAtual = 0;
    std::string token = "";

    for (size_t i = 0; i < linha.length(); i++) {
        if (linha[i] == '\r' || linha[i] == '\n') continue;

        if (linha[i] == delim) {
            if (colunaAtual == tamOut) return token;
            colunaAtual++;
            token = "";
        } else {
            token += linha[i];
        }
    }
    if (colunaAtual == tamOut) return token;
    return "";
}

int ParserPlanilha::importarCSV(std::string arquivo, Farm& f) {
    std::ifstream in(arquivo);
    if (!in.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo CSV: " + arquivo);
    }

    std::string linha;
    int registrosImportados = 0;
    Area* areaAtual = nullptr;

    while (std::getline(in, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        std::string tipo = splitManual(linha, ';', 0);

        if (tipo == "AREA") {
            // Formato: AREA;nome;estadoPasto;area
            std::string nome = splitManual(linha, ';', 1);
            std::string estadoPasto = splitManual(linha, ';', 2);
            double areaHectares = std::stod(splitManual(linha, ';', 3));

            Area* novaArea = new Area(nome, estadoPasto, areaHectares); 
            try {
                f.adicionarArea(novaArea); 
                areaAtual = novaArea;
                registrosImportados++;
            } catch (...) {
                delete novaArea;
                throw;
            }
        } 
        else {
            if (areaAtual == nullptr) {
                throw std::runtime_error("Animal declarado no CSV antes de definir uma AREA.");
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
                // Formato: VACA;id;sexo;peso;idadeMeses;qtdFilhos;qtdVacinas[cite: 13]
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
                // Formato: BOICORTE;id;sexo;peso;idadeMeses;qtdFilhos;qtdVacinas[cite: 10]
                int idadeMeses = std::stoi(splitManual(linha, ';', 4));
                int qtdFilhos = std::stoi(splitManual(linha, ';', 5));
                int qtdVacinas = std::stoi(splitManual(linha, ';', 6));

                novoAnimal = new Boicorte(id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas); //[cite: 10]
            } 
            else if (tipo == "BEZERRO") {
                // Construtor nascido: BEZERRO;id;sexo;peso[cite: 9]
                // Construtor adquirido: BEZERRO;id;sexo;peso;idadeMeses;qtdFilhos;qtdVacinas;consumoLeite;ganhoPeso;idadeDesmame;desmamado
                std::string colIdade = splitManual(linha, ';', 4);
                if (colIdade.empty()) {
                    novoAnimal = new Bezerro(id, sexo, peso); //[cite: 9]
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
                    ); //[cite: 9]
                }
            } 
            else {
                throw std::runtime_error("Tipo de registro desconhecido no CSV: " + tipo);
            }

            try {
                areaAtual->adicionarAnimal(novoAnimal); //[cite: 8]
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

bool ParserPlanilha::exportarCSV(std::string arquivo, Farm& f) {
    std::ofstream out(arquivo);
    if (!out.is_open()) {
        throw std::runtime_error("Erro ao criar arquivo de exportacao: " + arquivo);
    }

    out << "# RELATORIO EXPORTADO FARMMANAGER\n";
    out << "TOTAL_ANIMAIS;" << f.getQuantidadeTotalAnimais() << "\n"; //[cite: 11]
    //out << "RECEITA_DIARIA;" << f.calcularReceitaTotalDiariaFazenda() << "\n"; //[cite: 11]
    //out << "GASTO_DIARIO;" << f.calcularGastoTotalDiarioFazenda() << "\n"; //[cite: 11]
    //out << "LUCRO_DIARIO;" << f.calcularLucroGeralDiario() << "\n"; //[cite: 11]

    out.close();
    return true;
}
}