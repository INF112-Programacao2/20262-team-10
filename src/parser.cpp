// #include "parserplanilha.hpp"
// #include "farm.hpp"
// #include "area.hpp"
// #include "vaca.hpp"
// #include "touro.hpp"
// #include "boicorte.hpp"
// #include "bezerro.hpp"
// #include <fstream>
// #include <stdexcept>

// std::string ParserPlanilha::splitManual(std::string linha, char delim, int tamOut) {
//     int colunaAtual = 0;
//     std::string token = "";

//     for (size_t i = 0; i < linha.length(); i++) {
//         if (linha[i] == '\r' || linha[i] == '\n') continue;

//         if (linha[i] == delim) {
//             if (colunaAtual == tamOut) return token;
//             colunaAtual++;
//             token = "";
//         } else {
//             token += linha[i];
//         }
//     }
//     if (colunaAtual == tamOut) return token;
//     return "";
// }

// // Helper local para montar o array dinamico de IDs de filhos (separados por ',')
// static std::string* extrairArrayFilhos(const std::string& listaFilhos, int qtdFilhos) {
//     if (qtdFilhos <= 0 || listaFilhos.empty() || listaFilhos == "_") {
//         return nullptr;
//     }
//     std::string* arr = new std::string[qtdFilhos];
//     for (int i = 0; i < qtdFilhos; i++) {
//         arr[i] = ParserPlanilha::splitManual(listaFilhos, ',', i);
//     }
//     return arr;
// }

// int ParserPlanilha::importarCSV(std::string arquivo, Farm& f) {
//     std::ifstream in(arquivo);
//     if (!in.is_open()) {
//         throw std::runtime_error("Nao foi possivel abrir o arquivo CSV: " + arquivo);
//     }

//     std::string linha;
//     int registrosImportados = 0;
//     Area* areaAtual = nullptr;

//     while (std::getline(in, linha)) {
//         if (linha.empty() || linha[0] == '#') continue;

//         std::string tipo = splitManual(linha, ';', 0);

//         if (tipo == "AREA") {
//             // AREA;nome;estadoPasto;area
//             std::string nome = splitManual(linha, ';', 1);
//             std::string estadoPasto = splitManual(linha, ';', 2);
//             double areaHectares = std::stod(splitManual(linha, ';', 3));

//             Area* novaArea = new Area(nome, estadoPasto, areaHectares);
//             try {
//                 f.adicionarArea(novaArea);
//                 areaAtual = novaArea;
//                 registrosImportados++;
//             } catch (...) {
//                 delete novaArea;
//                 throw;
//             }
//         } 
//         else {
//             if (areaAtual == nullptr) {
//                 throw std::runtime_error("Animal declarado no CSV antes de definir uma AREA.");
//             }

//             // Campos base comuns de Animal:
//             // col 1: id | col 2: sexo | col 3: peso | col 4: idadeMeses
//             // col 5: qtdFilhos | col 6: idsFilhos (ex: F1,F2 ou _) | col 7: qtdVacinas
//             // col 8: idPai (ou _) | col 9: idMae (ou _)
//             std::string id = splitManual(linha, ';', 1);
//             std::string sexoStr = splitManual(linha, ';', 2);
//             if (sexoStr.empty()) {
//                 throw std::runtime_error("Sexo nao informado para o animal: " + id);
//             }
//             char sexo = sexoStr[0];
//             double peso = std::stod(splitManual(linha, ';', 3));

//             Animal* novoAnimal = nullptr;

//             if (tipo == "BEZERRO" && splitManual(linha, ';', 4).empty()) {
//                 // Construtor curto (nascido): BEZERRO;id;sexo;peso
//                 novoAnimal = new Bezerro(id, sexo, peso);
//             } 
//             else {
//                 int idadeMeses = std::stoi(splitManual(linha, ';', 4));
//                 int qtdFilhos = std::stoi(splitManual(linha, ';', 5));
//                 std::string strFilhos = splitManual(linha, ';', 6);
//                 int qtdVacinas = std::stoi(splitManual(linha, ';', 7));
//                 std::string idPai = splitManual(linha, ';', 8);
//                 std::string idMae = splitManual(linha, ';', 9);

//                 std::string* arrFilhos = extrairArrayFilhos(strFilhos, qtdFilhos);

//                 try {
//                     if (tipo == "VACA") {
//                         // VACA;id;sexo;peso;idade;qtdF;idsF;qtdV;idPai;idMae
//                         novoAnimal = new Vaca(id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas);
//                     } 
//                     else if (tipo == "TOURO") {
//                         // TOURO;id;sexo;peso;idade;qtdF;idsF;qtdV;idPai;idMae;valorSemen;meritoPTA;dosesEstoque;qtdCoberturas
//                         double valorSemen = std::stod(splitManual(linha, ';', 10));
//                         double meritoPTA = std::stod(splitManual(linha, ';', 11));
//                         int dosesEstoque = std::stoi(splitManual(linha, ';', 12));
//                         int qtdCoberturas = std::stoi(splitManual(linha, ';', 13));

//                         Touro* t = new Touro(id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas, arrFilhos, valorSemen);
//                         t->setMerito(meritoPTA);
//                         t->setDosesEstoque(dosesEstoque);
//                         t->setQtdCoberturas(qtdCoberturas);
//                         novoAnimal = t;
//                     } 
//                     else if (tipo == "BOI" || tipo == "BOICORTE") {
//                         // BOICORTE;id;sexo;peso;idade;qtdF;idsF;qtdV;idPai;idMae;rendimentoCarcaca;metaAbatePeso;faseEngorda
//                         double rendimento = std::stod(splitManual(linha, ';', 10));
//                         double metaAbate = std::stod(splitManual(linha, ';', 11));
//                         std::string engordaStr = splitManual(linha, ';', 12);
//                         bool faseEngorda = (engordaStr == "1" || engordaStr == "true" || engordaStr == "TRUE");

//                         Boicorte* b = new Boicorte(id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas);
//                         b->setrendimentoCarcaca(rendimento);
//                         b->setmetaAbatePeso(metaAbate);
//                         b->setFaseEngorda(faseEngorda);
//                         novoAnimal = b;
//                     } 
//                     else if (tipo == "BEZERRO") {
//                         // BEZERRO;id;sexo;peso;idade;qtdF;idsF;qtdV;idPai;idMae;consumoLeite;ganhoPeso;idadeDesmame;desmamado
//                         double consumoLeite = std::stod(splitManual(linha, ';', 10));
//                         double ganhoPeso = std::stod(splitManual(linha, ';', 11));
//                         int idadeDesmame = std::stoi(splitManual(linha, ';', 12));
//                         std::string desmStr = splitManual(linha, ';', 13);
//                         bool desmamado = (desmStr == "1" || desmStr == "true" || desmStr == "TRUE");

//                         novoAnimal = new Bezerro(
//                             id, sexo, peso, idadeMeses, qtdFilhos, qtdVacinas, arrFilhos,
//                             consumoLeite, ganhoPeso, idadeDesmame, desmamado
//                         );
//                     } 
//                     else {
//                         delete[] arrFilhos;
//                         throw std::runtime_error("Tipo de registro desconhecido no CSV: " + tipo);
//                     }

//                     // Aplica genealogia da superclasse Animal
//                     if (!idPai.empty() && idPai != "_") novoAnimal->setIdPai(idPai);
//                     if (!idMae.empty() && idMae != "_") novoAnimal->setIdMae(idMae);

//                     delete[] arrFilhos; // O construtor de Animal ja copiou o array
//                 } catch (...) {
//                     delete[] arrFilhos;
//                     delete novoAnimal;
//                     throw;
//                 }
//             }

//             try {
//                 areaAtual->adicionarAnimal(novoAnimal);
//                 registrosImportados++;
//             } catch (...) {
//                 delete novoAnimal;
//                 throw;
//             }
//         }
//     }

//     in.close();
//     return registrosImportados;
// }

// bool ParserPlanilha::exportarCSV(std::string arquivo, Farm& f) {
//     std::ofstream out(arquivo);
//     if (!out.is_open()) {
//         throw std::runtime_error("Erro ao criar arquivo de exportacao: " + arquivo);
//     }

//     out << "# RELATORIO EXPORTADO FARMMANAGER\n";
//     out << "TOTAL_ANIMAIS;" << f.getQuantidadeTotalAnimais() << "\n";
//     out << "RECEITA_DIARIA;" << f.calcularReceitaTotalDiariaFazenda() << "\n";
//     out << "GASTO_DIARIO;" << f.calcularGastoTotalDiarioFazenda() << "\n";
//     out << "LUCRO_DIARIO;" << f.calcularLucroGeralDiario() << "\n";

//     out.close();
//     return true;
// }