#include <iostream>
#include <stdexcept>
#include <fstream>
#include <limits>
#include "area.hpp"
#include "farm.hpp"
#include "animal.hpp"
#include "touro.hpp"
#include "bezerro.hpp"
#include "vaca.hpp"
#include "user.hpp"
#include "sacaRacao.hpp"

Area::Area(string n, string estado, double a) {
    if (a <= 0) {
        throw std::runtime_error("A area deve ser maior que zero.");
    }
    nome = n;
    estadoPasto = estado;
    area = a;
    qtdAtualDeAnimais = 0;
    rebanho = new Animal*[1];
    rebanho[0] = nullptr;
    atualizarCapacidadeMaxima();
}

Area::~Area() {
    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        delete rebanho[i];
    }
    delete[] rebanho;
}

static std::string formatarTextoAreaSalvar(std::string texto) {
    std::string formatado = texto.empty() ? "_" : texto;
    for (size_t i = 0; i < formatado.length(); i++) {
        if (formatado[i] == ' ') formatado[i] = '_';
    }
    return formatado;
}

void Area::atualizarCapacidadeMaxima() {
    double cabecasPorHectare = 2.0; // padrao intermediario
    if (estadoPasto == "seco") cabecasPorHectare = 0.5;
    else if (estadoPasto == "ruim") cabecasPorHectare = 1.0;
    else if (estadoPasto == "intermediario") cabecasPorHectare = 2.0;
    else if (estadoPasto == "bom") cabecasPorHectare = 3.0;
    else if (estadoPasto == "muito bom") cabecasPorHectare = 4.0;

    capacidadeMaxima = static_cast<int>(area * cabecasPorHectare);
    if (capacidadeMaxima < 1) {
        capacidadeMaxima = 1;
    }
}

double Area::calcularLotacaoArea() {
    if (capacidadeMaxima <= 0) {
        return 0.0;
    }
    return (qtdAtualDeAnimais * 100.0) / capacidadeMaxima;
}

void Area::adicionarAnimal(Animal* a, bool silencioso) {
    if (a == nullptr) {
        throw std::runtime_error("Nao e possivel adicionar um animal nulo.");
    }

    if (buscarAnimal(a->getId()) != nullptr) {
        throw std::runtime_error("Ja existe um animal com este ID nesta area.");
    }

    Animal** rebanhoNovaAlocacao = new Animal*[qtdAtualDeAnimais + 1];

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        rebanhoNovaAlocacao[i] = rebanho[i];
    }

    rebanhoNovaAlocacao[qtdAtualDeAnimais] = a;
    qtdAtualDeAnimais++;

    delete[] rebanho;
    rebanho = rebanhoNovaAlocacao;

    if (!silencioso) {
        std::cout << "Animal de indice --" << a->getId() << "-- foi adicionado com sucesso!" << std::endl;
        std::cout << "A quantidade atual de animais e de: " << qtdAtualDeAnimais << std::endl;

        if (qtdAtualDeAnimais >= capacidadeMaxima) {
            std::cout << "Atencao, verifique a lotacao atual do pasto, ela e atualmente de: " << calcularLotacaoArea() << "%" << std::endl;
        }
    }
}

void Area::escreverNoArquivo(std::ofstream& out, std::string idDono, std::string nomeFazenda) {
    out << formatarTextoAreaSalvar(idDono) << " "
        << formatarTextoAreaSalvar(nomeFazenda) << " "
        << formatarTextoAreaSalvar(nome) << " "
        << formatarTextoAreaSalvar(estadoPasto) << " "
        << area << " "
        << qtdAtualDeAnimais;

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] != nullptr) {
            out << " " << formatarTextoAreaSalvar(rebanho[i]->getTipo())
                << " " << formatarTextoAreaSalvar(rebanho[i]->getId());
        }
    }
    out << "\n";
}

void Area::removerAnimal(string idAnimal) {
    Animal* animalParaAbate = extrairAnimal(idAnimal);

    if (animalParaAbate == nullptr) {
        std::cout << "Erro: Animal nao encontrado no rebanho." << std::endl;
        return;
    }

    delete animalParaAbate;
    std::cout << "Animal removido e deletado com sucesso!" << std::endl;
}

void Area::promoverAnimal() {
    if (rebanho == nullptr) throw std::runtime_error("Rebanho da area nao foi alocado!");

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] == nullptr)
            continue;

        if (rebanho[i]->podeSerPromovido() && rebanho[i]->getTipo() == "bezerro") {
            std::cout << "O animal " << rebanho[i]->getId() << " ja esta na idade apropriada para promocao.\n";
            Bezerro* bezSugestao = dynamic_cast<Bezerro*>(rebanho[i]);
            if (bezSugestao != nullptr) {
                std::cout << "Score genetico: " << rebanho[i]->getScoreGenetico()
                          << " | destino sugerido: " << bezSugestao->recomendarDestinoMaturidade() << "\n";
            }
            std::cout << "Gostaria de realizar a promocao [Y/N]? ";
            string resposta;
            std::cin >> resposta;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (resposta == "Y" || resposta == "y") {
                if (rebanho[i]->getSexo() == 'M') {
                    Touro* t = new Touro(rebanho[i]->getId(),
                        rebanho[i]->getPeso(), rebanho[i]->getIdadeMeses(), 0, rebanho[i]->getQtdvacinas(), nullptr, 0.0);
                    t->setIdPai(rebanho[i]->getPai());
                    t->setIdMae(rebanho[i]->getMae());
                    t->copiarDadosGeneticosDe(*rebanho[i]);

                    delete rebanho[i];
                    rebanho[i] = t;
                    std::cout << "Bezerro promovido a touro com sucesso!\n";
                } else if (rebanho[i]->getSexo() == 'F') {
                    Vaca* v = new Vaca(rebanho[i]->getId(),
                        rebanho[i]->getPeso(), rebanho[i]->getIdadeMeses(), 0, rebanho[i]->getQtdvacinas(), nullptr);
                    v->setIdPai(rebanho[i]->getPai());
                    v->setIdMae(rebanho[i]->getMae());
                    v->copiarDadosGeneticosDe(*rebanho[i]);

                    delete rebanho[i];
                    rebanho[i] = v;
                    std::cout << "Bezerro promovido a vaca com sucesso!\n";
                }
            }
        }
    }
}

ContagemRebanho Area::getContagemPorTipo() const {
    ContagemRebanho contagem;
    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] == nullptr) continue;

        std::string tipo = rebanho[i]->getTipo();
        if (tipo == "vaca") contagem.vacas++;
        else if (tipo == "touro") contagem.touros++;
        else if (tipo == "boicorte") contagem.boisCorte++;
        else if (tipo == "bezerro") contagem.bezerros++;
    }
    return contagem;
}

void Area::listarAnimais() const {
    if (qtdAtualDeAnimais == 0) {
        std::cout << "Nenhum animal nesta area.\n";
        return;
    }

    ContagemRebanho c = getContagemPorTipo();
    std::cout << "Total na area: " << qtdAtualDeAnimais
              << " (Vacas: " << c.vacas
              << " | Touros: " << c.touros
              << " | Bois de corte: " << c.boisCorte
              << " | Bezerros: " << c.bezerros << ")\n\n";

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] != nullptr) {
            rebanho[i]->imprimirFichaParcial();
        }
    }
}

Animal* Area::extrairAnimal(string idAnimal) {
    int indexAnimalRemovido = -1;
    Animal* animalExtraido = nullptr;

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] != nullptr && rebanho[i]->getId() == idAnimal) {
            indexAnimalRemovido = i;
            animalExtraido = rebanho[i];
            break;
        }
    }

    if (indexAnimalRemovido == -1) {
        return nullptr;
    }

    int novoTamanho = (qtdAtualDeAnimais - 1 > 0) ? (qtdAtualDeAnimais - 1) : 1;
    Animal** rebanhoNovaAlocacao = new Animal*[novoTamanho];
    rebanhoNovaAlocacao[0] = nullptr;

    for (int i = 0; i < indexAnimalRemovido; i++) {
        rebanhoNovaAlocacao[i] = rebanho[i];
    }
    for (int i = indexAnimalRemovido + 1; i < qtdAtualDeAnimais; i++) {
        rebanhoNovaAlocacao[i - 1] = rebanho[i];
    }

    delete[] rebanho;
    rebanho = rebanhoNovaAlocacao;
    qtdAtualDeAnimais--;

    return animalExtraido;
}

Animal* Area::buscarAnimal(string id) {
    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] != nullptr && rebanho[i]->getId() == id) {
            return rebanho[i];
        }
    }
    return nullptr;
}

double Area::getSacoGastoIndividual(string id, SacaRacao** relatorioRacoes) {
    Animal* animalAchado = buscarAnimal(id);
    if (animalAchado == nullptr) throw std::runtime_error("Esse animal nao foi encontrado");

    double sacos = animalAchado->calcularSacosDiarios(relatorioRacoes);
    if (calcularLotacaoArea() > 100) {
        return sacos * (calcularLotacaoArea() / 100.0);
    }
    return sacos;
}

ConsumoRacao Area::getSacosGastosNaArea(SacaRacao** relatorioRacoes) {
    ConsumoRacao total;

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] == nullptr) continue;

        string tipoRacao = rebanho[i]->getTipoRacao();
        double sacos = rebanho[i]->calcularSacosDiarios(relatorioRacoes);

        if (tipoRacao == "Racao Engorda") total.engorda += sacos;
        else if (tipoRacao == "Racao Avulsa") total.avulsa += sacos;
        else if (tipoRacao == "Racao Recria") total.recria += sacos;
    }

    if (calcularLotacaoArea() > 100) {
        double fator = calcularLotacaoArea() / 100.0;
        total.engorda *= fator;
        total.avulsa *= fator;
        total.recria *= fator;
    }

    return total;
}

double Area::getGastoIndividual(string id, double precoLeite, SacaRacao** relatorioRacoes) {
    Animal* animalAchado = buscarAnimal(id);
    if (animalAchado == nullptr) throw std::runtime_error("Esse animal nao foi encontrado");

    double gastoBase = animalAchado->calcularGastoDiario(precoLeite, relatorioRacoes);
    if (calcularLotacaoArea() > 100 && !animalAchado->getTipoRacao().empty()) {
        return gastoBase * (calcularLotacaoArea() / 100.0);
    }
    return gastoBase;
}

double Area::getGastoDaArea(double precoLeite, SacaRacao** relatorioRacoes) {
    double total = 0.0;
    double fatorLotacao = (calcularLotacaoArea() > 100) ? (calcularLotacaoArea() / 100.0) : 1.0;

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] != nullptr) {
            double gastoAnimal = rebanho[i]->calcularGastoDiario(precoLeite, relatorioRacoes);
            if (!rebanho[i]->getTipoRacao().empty()) {
                gastoAnimal *= fatorLotacao;
            }
            total += gastoAnimal;
        }
    }
    return total;
}

//getters
string Area::getNomeArea()        { return nome; }
string Area::getEstadoPasto()     { return estadoPasto; }
double Area::getArea()            { return area; }
int Area::getCapacidadeMaxima()   { return capacidadeMaxima; }
int Area::getQtdAnimais()         { return qtdAtualDeAnimais; }
Animal** Area::getRebanho()       { return rebanho; }

//setters
void Area::setNomeArea(string n) {
    nome = n;
}

void Area::setEstadoPasto(string estado) {
    estadoPasto = estado;
    atualizarCapacidadeMaxima();
    if (qtdAtualDeAnimais > capacidadeMaxima) {
        std::cout << "Atencao: com o novo estado do pasto, a lotacao subiu para " << calcularLotacaoArea() << "%" << std::endl;
    }
}

void Area::setArea(double a) {
    if (a <= 0) {
        throw std::runtime_error("A area deve ser maior que zero.");
    }
    area = a;
    atualizarCapacidadeMaxima();
    if (qtdAtualDeAnimais > capacidadeMaxima) {
        std::cout << "Atencao: com a nova area, a lotacao subiu para " << calcularLotacaoArea() << "%" << std::endl;
    }
}