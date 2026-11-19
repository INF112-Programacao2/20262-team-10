#include "farm.hpp"
#include "animal.hpp"
#include "touro.hpp"
#include "vaca.hpp"
#include "bezerro.hpp"
#include <iostream>
#include <stdexcept>

Farm::Farm(std::string s, std::string l, User* propr) {
    if (propr == nullptr) {
        throw std::invalid_argument("Proprietario da fazenda invalido!");
    }
    nome = s;
    localizacao = l;
    areaAtual = "";
    proprietario = propr;
    area = nullptr;
    qtdAreas = 0;

    qtdTotalDeAnimais = 0;
    qtdBaixas = 0;
    saldoCaixa = 0.0;
    lucroAcumuladoVendas = 0.0;
    prejuizoAcumuladoMortes = 0.0;
    diaAtualSimulacao = 0;
}

Farm::~Farm() {
    if (area != nullptr) {
        for (int i = 0; i < qtdAreas; i++) {
            delete area[i];
        }
        delete[] area;
    }
}

Animal* Farm::encontrarAnimalOpcional(const std::string& identificador) const {
    if (area == nullptr) return nullptr;
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr) {
            Animal* encontrado = area[i]->buscarAnimal(identificador);
            if (encontrado != nullptr) {
                return encontrado;
            }
        }
    }
    return nullptr;
}

void Farm::adicionarArea(Area* a) {
    if (a == nullptr) {
        throw std::invalid_argument("Nao e possivel adicionar uma area nula!");
    }

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr && area[i]->getNomeArea() == a->getNomeArea()) {
            throw std::runtime_error("Ja existe uma area com este nome na fazenda!");
        }
    }

    Area** areaNovaAlocacao = new Area*[qtdAreas + 1];

    for (int i = 0; i < qtdAreas; i++) {
        areaNovaAlocacao[i] = area[i];
    }
    areaNovaAlocacao[qtdAreas] = a;
    qtdAreas++;

    delete[] area;
    area = areaNovaAlocacao;

    if (areaAtual.empty()) {
        areaAtual = a->getNomeArea();
    }

    std::cout << "Nova area --" << a->getNomeArea() << "-- foi adicionada com sucesso!" << std::endl;
}

// double Farm::venderAnimal(string id) {
//     precoVenda = buscarAnimalGlobal(id).;
// }

void Farm::removerArea(Area* a) {
    if (a == nullptr) {
        throw std::invalid_argument("Area invalida para remocao!");
    }

    int indexAreaRemovido = -1;

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr && area[i]->getNomeArea() == a->getNomeArea()) {
            if (area[i]->getQtdAnimais() > 0) {
                throw std::runtime_error("Voce nao pode remover essa area, existem animais nela!");
            }
            indexAreaRemovido = i;
            break;
        }
    }

    if (indexAreaRemovido == -1) {
        throw std::runtime_error("Area nao encontrada na fazenda!");
    }

    std::string nomeRemovido = area[indexAreaRemovido]->getNomeArea();
    delete area[indexAreaRemovido];

    if (qtdAreas - 1 == 0) {
        delete[] area;
        area = nullptr;
        qtdAreas = 0;
    } else {
        Area** areaNovaAlocacao = new Area*[qtdAreas - 1];

        for (int i = 0; i < indexAreaRemovido; i++) {
            areaNovaAlocacao[i] = area[i];
        }
        for (int i = indexAreaRemovido + 1; i < qtdAreas; i++) {
            areaNovaAlocacao[i - 1] = area[i];
        }

        delete[] area;
        area = areaNovaAlocacao;
        qtdAreas--;
    }

    if (areaAtual == nomeRemovido) {
        areaAtual = (qtdAreas > 0 && area[0] != nullptr) ? area[0]->getNomeArea() : "";
    }

    std::cout << "Area removida com sucesso!" << std::endl;
}


int Farm::getQuantidadeTotalAnimais() {
    qtdTotalDeAnimais = 0;
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr) {
            qtdTotalDeAnimais += area[i]->getQtdAnimais();
        }
    }
    return qtdTotalDeAnimais;
}

void Farm::selecionarArea(std::string n) {
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr && area[i]->getNomeArea() == n) {
            std::cout << "Area: " << area[i]->getNomeArea() << " selecionada." << std::endl;
            areaAtual = n;
            return;
        }
    }
    throw std::runtime_error("Area nao encontrada na fazenda!");
}

void Farm::promoverAnimalGlobal() {
    if (area == nullptr || qtdAreas == 0) {
        throw std::runtime_error("Nenhuma area alocada na fazenda!");
    }

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr) {
            area[i]->promoverAnimal();
        }
    }
}

Animal* Farm::buscarAnimalGlobal(std::string identificador) {
    Animal* encontrado = encontrarAnimalOpcional(identificador);
    if (encontrado != nullptr) {
        return encontrado;
    }
    throw std::out_of_range("Animal nao encontrado em nenhuma area!");
}

bool Farm::transferirAnimalEntreAreas(std::string idAnimal, std::string nomeArea) {
    int indexOrigem = -1;
    int indexDestino = -1;

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] == nullptr) continue;
        if (area[i]->getNomeArea() == nomeArea) {
            indexDestino = i;
        }
        if (area[i]->buscarAnimal(idAnimal) != nullptr) {
            indexOrigem = i;
        }
    }

    if (indexOrigem == -1) {
        throw std::invalid_argument("Erro: Animal de id " + idAnimal + " nao encontrado em nenhuma area para transferencia.");
    }
    if (indexDestino == -1) {
        throw std::invalid_argument("Erro: Area de destino nao encontrada na fazenda.");
    }
    if (indexOrigem == indexDestino) {
        return true;
    }

    Animal* temporario = area[indexOrigem]->extrairAnimal(idAnimal);
    area[indexDestino]->adicionarAnimal(temporario);

    std::cout << "Animal transferido com sucesso para a area " << nomeArea << std::endl;
    return true;
}

bool Farm::transferirLoteDeAnimaisEntreAreas(std::string* ids, int qtdAnimais, std::string areaDestino) {
    if (ids == nullptr || qtdAnimais <= 0) {
        throw std::invalid_argument("Lista de animais invalida para transferencia em lote.");
    }

    bool destinoExiste = false;
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr && area[i]->getNomeArea() == areaDestino) {
            destinoExiste = true;
            break;
        }
    }
    if (!destinoExiste) {
        throw std::invalid_argument("Erro: Area de destino nao encontrada na fazenda.");
    }

    // Valida todos os IDs antes para evitar transferencia pela metade
    for (int i = 0; i < qtdAnimais; i++) {
        buscarAnimalGlobal(ids[i]);
    }

    for (int i = 0; i < qtdAnimais; i++) {
        transferirAnimalEntreAreas(ids[i], areaDestino);
    }
    return true;
}

void Farm::realizarCobertura(const std::string& idMacho, const std::string& idFemea) {
    Animal* macho = buscarAnimalGlobal(idMacho);
    Animal* femea = buscarAnimalGlobal(idFemea);
    Touro* touro = dynamic_cast<Touro*>(macho);
    Vaca* vaca = dynamic_cast<Vaca*>(femea);

    if (touro == nullptr) throw std::invalid_argument("O animal informado como macho nao e um touro!");
    if (vaca == nullptr) throw std::invalid_argument("O animal informado como femea nao e uma vaca!");

    touro->cobrirFemea(vaca);
}

void Farm::registrarNascimento(const std::string& idFemea, const std::string& nomeArea, std::string id, char sexo, double peso) {
    if (sexo != 'M' && sexo != 'F') throw std::invalid_argument("Sexo invalido!");
    if (peso <= 0) throw std::invalid_argument("Peso invalido!");
    if (id.empty() || encontrarAnimalOpcional(id) != nullptr) {
        throw std::invalid_argument("ID do filhote invalido ou ja existente na fazenda!");
    }

    Animal* femea = buscarAnimalGlobal(idFemea);
    Vaca* mae = dynamic_cast<Vaca*>(femea);
    if (mae == nullptr) throw std::invalid_argument("Mae do bezerro nao encontrada!");
    if (!mae->getPrenha()) throw std::runtime_error("Esta vaca nao esta prenha!");

    Area* destino = nullptr;
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr && area[i]->getNomeArea() == nomeArea) {
            destino = area[i];
            break;
        }
    }
    if (destino == nullptr) {
        throw std::invalid_argument("Area de destino nao encontrada!");
    }

    std::string idPai = mae->parir();
    Bezerro* filhote = new Bezerro(id, sexo, peso);
    filhote->setIdMae(idFemea);
    filhote->setIdPai(idPai);

    destino->adicionarAnimal(filhote);
    mae->adicionarFilho(id);

    if (!idPai.empty()) {
        Animal* macho = encontrarAnimalOpcional(idPai);
        Touro* pai = dynamic_cast<Touro*>(macho);
        if (pai != nullptr) {
            pai->adicionarFilho(id);
        }
    }
}

double Farm::calcularGastoTotalDiarioFazenda() {
    if (proprietario == nullptr) {
        throw std::runtime_error("Proprietario nao definido na fazenda!");
    }

    double total = 0.0;
    double precoLeite = proprietario->getPrecoLeite();
    SacaRacao** relatorio = proprietario->getRelatorioRacao();

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr) {
            total += area[i]->getGastoDaArea(precoLeite, relatorio);
        }
    }
    return total;
}

double Farm::calcularReceitaTotalDiariaFazenda() {
    if (proprietario == nullptr) {
        throw std::runtime_error("Proprietario nao definido na fazenda!");
    }

    double total = 0.0;
    double precoLeite = proprietario->getPrecoLeite();
    double arrobaBoi = proprietario->getCotacaoArrobaBoi();
    double arrobaVaca = proprietario->getCotacaoArrobaVaca();

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] == nullptr) continue;

        Animal** rebanho = area[i]->getRebanho();
        int qtd = area[i]->getQtdAnimais();

        for (int j = 0; j < qtd; j++) {
            if (rebanho[j] == nullptr) continue;

            std::string tipo = rebanho[j]->getTipo();
            if (tipo == "vaca") {
                total += rebanho[j]->calcularReceitaDiaria(precoLeite);
            } else if (tipo == "boicorte") {
                total += rebanho[j]->calcularReceitaDiaria(arrobaBoi);
            } else if (tipo == "bezerro") {
                double arrobaRef = (rebanho[j]->getSexo() == 'F') ? arrobaVaca : arrobaBoi;
                total += rebanho[j]->calcularReceitaDiaria(arrobaRef);
            }
        }
    }
    return total;
}

double Farm::calcularLucroGeralDiario() {
    return calcularReceitaTotalDiariaFazenda() - calcularGastoTotalDiarioFazenda();
}

ConsumoRacao Farm::calcularConsumoRacaoDiarioFazenda() {
    if (proprietario == nullptr) {
        throw std::runtime_error("Proprietario nao definido na fazenda!");
    }

    ConsumoRacao total;
    SacaRacao** relatorio = proprietario->getRelatorioRacao();

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr) {
            ConsumoRacao parcial = area[i]->getSacosGastosNaArea(relatorio);
            total.engorda += parcial.engorda;
            total.avulsa += parcial.avulsa;
            total.recria += parcial.recria;
        }
    }
    return total;
}