#include "farm.hpp"
#include "animal.hpp"
#include "touro.hpp"
#include "boicorte.hpp"
#include "vaca.hpp"
#include <fstream>
#include <ostream>
#include <cstdio>
#include <iomanip>
#include <filesystem>
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
    proximoIdAnimal = 1;
}

ContagemRebanho Farm::getContagemGlobalPorTipo() const {
    ContagemRebanho total;
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr) {
            ContagemRebanho parcial = area[i]->getContagemPorTipo();
            total.vacas += parcial.vacas;
            total.touros += parcial.touros;
            total.boisCorte += parcial.boisCorte;
            total.bezerros += parcial.bezerros;
        }
    }
    return total;
}

void Farm::listarAreas() const {
    if (qtdAreas == 0) {
        std::cout << "Nenhuma area cadastrada na fazenda.\n";
        return;
    }
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] == nullptr) continue;
        ContagemRebanho c = area[i]->getContagemPorTipo();
        std::cout << "  - " << area[i]->getNomeArea()
                  << " | Pasto: " << area[i]->getEstadoPasto()
                  << " | " << area[i]->getArea() << " ha"
                  << " | Lotacao: " << area[i]->getQtdAnimais()
                  << "/" << area[i]->getCapacidadeMaxima()
                  << " (" << area[i]->calcularLotacaoArea() << "%)"
                  << (area[i]->getNomeArea() == areaAtual ? " [ATUAL]" : "")
                  << "\n    Rebanho -> Vacas: " << c.vacas
                  << " | Touros: " << c.touros
                  << " | Bois de corte: " << c.boisCorte
                  << " | Bezerros: " << c.bezerros << "\n";
    }
}

void Farm::emitirRelatorioOperacional() {
    double gastoDia = calcularGastoTotalDiarioFazenda();
    double receitaDia = calcularReceitaTotalDiariaFazenda();
    double lucroDia = calcularLucroGeralDiario();
    ConsumoRacao sacosDia = calcularConsumoRacaoDiarioFazenda();
    ContagemRebanho rebanho = getContagemGlobalPorTipo();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n--- Relatorio da Fazenda: " << nome << " (" << localizacao << ") ---" << std::endl;
    std::cout << "Dia atual: " << diaAtualSimulacao << std::endl;
    std::cout << "Total de animais: " << rebanho.total()
              << " (Vacas: " << rebanho.vacas
              << " | Touros: " << rebanho.touros
              << " | Bois de corte: " << rebanho.boisCorte
              << " | Bezerros: " << rebanho.bezerros << ")" << std::endl;

    std::cout << "\nFinanceiro diario:" << std::endl;
    std::cout << "  Receita: R$ " << receitaDia << std::endl;
    std::cout << "  Gasto:   R$ " << gastoDia << std::endl;
    std::cout << "  Lucro:   R$ " << lucroDia << std::endl;

    std::cout << "\nConsumo de racao (diario / estimativa 30 dias):" << std::endl;
    std::cout << "  Engorda: " << sacosDia.engorda << " sacos/dia (" << sacosDia.engorda * 30.0 << " sacos/mes)" << std::endl;
    std::cout << "  Avulsa:  " << sacosDia.avulsa << " sacos/dia (" << sacosDia.avulsa * 30.0 << " sacos/mes)" << std::endl;
    std::cout << "  Recria:  " << sacosDia.recria << " sacos/dia (" << sacosDia.recria * 30.0 << " sacos/mes)" << std::endl;
}

void Farm::emitirRelatorioBreakEvenEVendas() {
    double lucroDia = calcularLucroGeralDiario();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n--- Caixa e Break-Even ---" << std::endl;
    std::cout << "  Saldo em caixa: R$ " << saldoCaixa << std::endl;
    std::cout << "  Total em vendas: R$ " << lucroAcumuladoVendas << std::endl;
    std::cout << "  Prejuizo acumulado: R$ " << prejuizoAcumuladoMortes << std::endl;
    std::cout << "  Lucro por dia: R$ " << lucroDia << std::endl;

    if (saldoCaixa >= 0.0) {
        std::cout << "Situacao: Caixa positivo, ponto de equilibrio ja atingido." << std::endl;
    } else if (lucroDia > 0.0) {
        double razao = (-saldoCaixa) / lucroDia;
        int dias = (int)razao;
        if (razao > dias) {
            dias++;
        }
        std::cout << "Situacao: Faltam " << dias << " dias no ritmo atual para empatar o caixa." << std::endl;
    } else {
        std::cout << "Situacao: Operacao diaria no prejuizo, nao tem como empatar o caixa assim." << std::endl;
    }
}

Farm::~Farm() {
    if (area != nullptr) {
        for (int i = 0; i < qtdAreas; i++) {
            delete area[i];
        }
        delete[] area;
    }
}

static std::string formatarTextoFarmSalvar(std::string texto) {
    std::string formatado = texto.empty() ? "_" : texto;
    for (size_t i = 0; i < formatado.length(); i++) {
        if (formatado[i] == ' ') formatado[i] = '_';
    }
    return formatado;
}

static std::string formatarTextoFarmCarregar(std::string texto) {
    if (texto == "_") return "";
    for (size_t i = 0; i < texto.length(); i++) {
        if (texto[i] == '_') texto[i] = ' ';
    }
    return texto;
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

void Farm::adicionarArea(Area* a, bool silencioso) {
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

    if (!silencioso) {
        std::cout << "Nova area --" << a->getNomeArea() << "-- foi adicionada com sucesso!" << std::endl;
    }

}

void Farm::salvarFazenda() {
    if (proprietario == nullptr) throw std::runtime_error("Proprietario invalido para salvar fazenda!");
    if (nome.empty()) throw std::invalid_argument("Nome da fazenda invalido!");

    std::filesystem::create_directories("./data");
    std::string idDono = proprietario->getId();

    // 1. Salva dados da fazenda
    std::ifstream leituraFaz("./data/fazendas.txt");
    std::ofstream tempFaz("./data/fazendas_temp.txt");
    if (!tempFaz.is_open()) {
        throw std::runtime_error("Falha ao criar arquivo temporario de fazendas.");
    }

    bool fazendaAtualizada = false;

    try {
        if (leituraFaz.is_open()) {
            std::string donoLido, nomeLido, locLida, areaAtualLida;
            int baixasLidas, diaLido, proxIdLido;
            double caixaLido, lucroLido, prejuizoLido;

            while (leituraFaz >> donoLido >> nomeLido >> locLida >> areaAtualLida
                              >> baixasLidas >> caixaLido >> lucroLido >> prejuizoLido >> diaLido >> proxIdLido) {
                std::string donoFmt = formatarTextoFarmCarregar(donoLido);
                std::string nomeFmt = formatarTextoFarmCarregar(nomeLido);

                if (donoFmt == idDono && nomeFmt == nome) {
                    tempFaz << formatarTextoFarmSalvar(idDono) << " "
                            << formatarTextoFarmSalvar(nome) << " "
                            << formatarTextoFarmSalvar(localizacao) << " "
                            << formatarTextoFarmSalvar(areaAtual) << " "
                            << qtdBaixas << " "
                            << saldoCaixa << " "
                            << lucroAcumuladoVendas << " "
                            << prejuizoAcumuladoMortes << " "
                            << diaAtualSimulacao << " "
                            << proximoIdAnimal << "\n";
                    fazendaAtualizada = true;
                } else {
                    tempFaz << donoLido << " " << nomeLido << " " << locLida << " " << areaAtualLida << " "
                            << baixasLidas << " " << caixaLido << " " << lucroLido << " "
                            << prejuizoLido << " " << diaLido << " " << proxIdLido << "\n";
                }
            }
            leituraFaz.close();
        }

        if (!fazendaAtualizada) {
            tempFaz << formatarTextoFarmSalvar(idDono) << " "
                    << formatarTextoFarmSalvar(nome) << " "
                    << formatarTextoFarmSalvar(localizacao) << " "
                    << formatarTextoFarmSalvar(areaAtual) << " "
                    << qtdBaixas << " "
                    << saldoCaixa << " "
                    << lucroAcumuladoVendas << " "
                    << prejuizoAcumuladoMortes << " "
                    << diaAtualSimulacao << " "
                    << proximoIdAnimal << "\n";
        }
    } catch (...) {
        if (leituraFaz.is_open()) leituraFaz.close();
        tempFaz.close();
        std::remove("./data/fazendas_temp.txt");
        throw;
    }

    tempFaz.close();
    std::remove("./data/fazendas.txt");
    std::rename("./data/fazendas_temp.txt", "./data/fazendas.txt");

    // 2. Salva areas da fazenda
    std::ifstream leituraAreas("./data/areas.txt");
    std::ofstream tempAreas("./data/areas_temp.txt");
    if (!tempAreas.is_open()) {
        throw std::runtime_error("Falha ao criar arquivo temporario de areas.");
    }

    try {
        if (leituraAreas.is_open()) {
            std::string donoLido, fazendaLida, nomeAreaLida, estadoLido;
            double hectaresLidos;
            int qtdAnimaisLida;

            while (leituraAreas >> donoLido >> fazendaLida >> nomeAreaLida >> estadoLido >> hectaresLidos >> qtdAnimaisLida) {
                std::string donoFmt = formatarTextoFarmCarregar(donoLido);
                std::string fazendaFmt = formatarTextoFarmCarregar(fazendaLida);

                if (donoFmt == idDono && fazendaFmt == nome) {
                    // Pula os pares tipo/id antigos desta area para reescrever atualizados
                    for (int i = 0; i < qtdAnimaisLida; i++) {
                        std::string tipoSkip, idSkip;
                        leituraAreas >> tipoSkip >> idSkip;
                    }
                } else {
                    tempAreas << donoLido << " " << fazendaLida << " " << nomeAreaLida << " "
                              << estadoLido << " " << hectaresLidos << " " << qtdAnimaisLida;
                    for (int i = 0; i < qtdAnimaisLida; i++) {
                        std::string tipoCopia, idCopia;
                        leituraAreas >> tipoCopia >> idCopia;
                        tempAreas << " " << tipoCopia << " " << idCopia;
                    }
                    tempAreas << "\n";
                }
            }
            leituraAreas.close();
        }

        for (int i = 0; i < qtdAreas; i++) {
            if (area[i] != nullptr) {
                area[i]->escreverNoArquivo(tempAreas, idDono, nome);
            }
        }
    } catch (...) {
        if (leituraAreas.is_open()) leituraAreas.close();
        tempAreas.close();
        std::remove("./data/areas_temp.txt");
        throw;
    }

    tempAreas.close();
    std::remove("./data/areas.txt");
    std::rename("./data/areas_temp.txt", "./data/areas.txt");

    // 3. Salva todos os animais nas respectivas tabelas
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] == nullptr) continue;
        Animal** rebanho = area[i]->getRebanho();
        int qtd = area[i]->getQtdAnimais();
        for (int j = 0; j < qtd; j++) {
            if (rebanho[j] != nullptr) {
                rebanho[j]->salvarEmArquivo();
            }
        }
    }
}

Farm* Farm::carregarFazenda(User* proprietario, std::string nomeFazenda) {
    if (proprietario == nullptr) throw std::invalid_argument("Proprietario invalido!");

    std::ifstream arqFaz("./data/fazendas.txt");
    if (!arqFaz.is_open()) {
        return nullptr;
    }

    std::string idDono = proprietario->getId();
    Farm* fazendaCarregada = nullptr;
    std::string areaSelecionadaSalva = "";

    try {
        std::string donoLido, nomeLido, locLida, areaAtualLida;
        int baixasLidas, diaLido, proxIdLido;
        double caixaLido, lucroLido, prejuizoLido;

        while (arqFaz >> donoLido >> nomeLido >> locLida >> areaAtualLida
                      >> baixasLidas >> caixaLido >> lucroLido >> prejuizoLido >> diaLido >> proxIdLido) {
            std::string donoFmt = formatarTextoFarmCarregar(donoLido);
            std::string nomeFmt = formatarTextoFarmCarregar(nomeLido);

            if (donoFmt == idDono && (nomeFazenda.empty() || nomeFmt == nomeFazenda)) {
                fazendaCarregada = new Farm(nomeFmt, formatarTextoFarmCarregar(locLida), proprietario);
                fazendaCarregada->qtdBaixas = baixasLidas;
                fazendaCarregada->saldoCaixa = caixaLido;
                fazendaCarregada->lucroAcumuladoVendas = lucroLido;
                fazendaCarregada->prejuizoAcumuladoMortes = prejuizoLido;
                fazendaCarregada->diaAtualSimulacao = diaLido;
                fazendaCarregada->proximoIdAnimal = (proxIdLido > 0) ? proxIdLido : 1;
                areaSelecionadaSalva = formatarTextoFarmCarregar(areaAtualLida);
                break;
            }
        }
        arqFaz.close();

        if (fazendaCarregada == nullptr) {
            return nullptr;
        }

        std::ifstream arqAreas("./data/areas.txt");
        if (arqAreas.is_open()) {
            std::string donoArea, fazendaArea, nomeAreaLida, estadoLido;
            double hectaresLidos;
            int qtdAnimaisLida;

            while (arqAreas >> donoArea >> fazendaArea >> nomeAreaLida >> estadoLido >> hectaresLidos >> qtdAnimaisLida) {
                std::string donoFmt = formatarTextoFarmCarregar(donoArea);
                std::string fazendaFmt = formatarTextoFarmCarregar(fazendaArea);

                if (donoFmt == idDono && fazendaFmt == fazendaCarregada->nome) {
                    Area* novaArea = new Area(
                        formatarTextoFarmCarregar(nomeAreaLida),
                        formatarTextoFarmCarregar(estadoLido),
                        hectaresLidos
                    );

                    for (int i = 0; i < qtdAnimaisLida; i++) {
                        std::string tipoLido, idAnimalLido;
                        if (!(arqAreas >> tipoLido >> idAnimalLido)) break;

                        std::string tipoFmt = formatarTextoFarmCarregar(tipoLido);
                        std::string idFmt = formatarTextoFarmCarregar(idAnimalLido);
                        Animal* animalCarregado = nullptr;

                        if (tipoFmt == "vaca") animalCarregado = Vaca::carregarVaca(idFmt);
                        else if (tipoFmt == "touro") animalCarregado = Touro::carregarTouro(idFmt);
                        else if (tipoFmt == "boicorte") animalCarregado = Boicorte::carregarBoicorte(idFmt);
                        else if (tipoFmt == "bezerro") animalCarregado = Bezerro::carregarBezerro(idFmt);

                        if (animalCarregado != nullptr) {
                            novaArea->adicionarAnimal(animalCarregado, true);
                        }
                    }

                    fazendaCarregada->adicionarArea(novaArea, true);
                } else {
                    for (int i = 0; i < qtdAnimaisLida; i++) {
                        std::string tipoSkip, idSkip;
                        arqAreas >> tipoSkip >> idSkip;
                    }
                }
            }
            arqAreas.close();
        }

        if (!areaSelecionadaSalva.empty()) {
            fazendaCarregada->areaAtual = areaSelecionadaSalva;
        }

        return fazendaCarregada;
    } catch (...) {
        delete fazendaCarregada;
        throw;
    }
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

Area* Farm::buscarArea(const std::string& nomeArea) const {
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr && area[i]->getNomeArea() == nomeArea) {
            return area[i];
        }
    }
    return nullptr;
}

std::string Farm::getAreaAtual() const {
    return areaAtual;
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

std::string Farm::registrarNascimento(const std::string& idFemea, const std::string& nomeArea, char sexo, double peso, std::string id) {
    if (sexo != 'M' && sexo != 'F') throw std::invalid_argument("Sexo invalido!");
    if (peso <= 0) throw std::invalid_argument("Peso invalido!");

    if (!id.empty() && encontrarAnimalOpcional(id) != nullptr) {
        throw std::invalid_argument("ID do filhote ja existente na fazenda!");
    }

    Animal* femea = buscarAnimalGlobal(idFemea);
    Vaca* mae = dynamic_cast<Vaca*>(femea);
    if (mae == nullptr) throw std::invalid_argument("Mae do bezerro nao encontrada!");
    if (!mae->getPrenha()) throw std::runtime_error("Esta vaca nao esta prenha!");

    Area* destino = buscarArea(nomeArea);
    if (destino == nullptr) {
        throw std::invalid_argument("Area de destino nao encontrada!");
    }

    // So gera e consome o proximo ID depois que todas as validacoes passaram
    if (id.empty()) {
        id = gerarProximoId("bezerro");
    }

    Bezerro* filhote = new Bezerro(id, sexo, peso);
    try {
        std::string idPai = mae->parir();
        filhote->setIdMae(idFemea);
        filhote->setIdPai(idPai);

        // snapshot incluindo toda a arvore ancestral
        filhote->setSnapshotMae(mae->criarSnapshot());

        // o pai pode estar em qualquer area da fazenda
        if (!idPai.empty()) { Animal* paiSnapshot = encontrarAnimalOpcional(idPai);

            if (paiSnapshot != nullptr) {
                filhote->setSnapshotPai(paiSnapshot->criarSnapshot());
            }
        }

        destino->adicionarAnimal(filhote);
        mae->adicionarFilho(id);

        if (!idPai.empty()) {
            Animal* macho = encontrarAnimalOpcional(idPai);
            Touro* pai = dynamic_cast<Touro*>(macho);
            if (pai != nullptr) {
                pai->adicionarFilho(id);
            }
        }
    } catch (...) {
        delete filhote;
        throw;
    }

    return id;
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

std::string Farm::gerarProximoId(const std::string& tipo) {
    std::string prefixo = "AN";
    if (tipo == "vaca") prefixo = "VC";
    else if (tipo == "touro") prefixo = "TR";
    else if (tipo == "boicorte") prefixo = "BC";
    else if (tipo == "bezerro") prefixo = "BZ";

    std::string novoId;
    do {
        novoId = prefixo + std::to_string(proximoIdAnimal++);
    } while (encontrarAnimalOpcional(novoId) != nullptr);

    return novoId;
}

void Farm::adicionarAnimalNaArea(const std::string& nomeArea, Animal* a, bool silencioso) {
    if (a == nullptr) {
        throw std::invalid_argument("Nao e possivel adicionar um animal nulo!");
    }
    if (a->getId().empty() || encontrarAnimalOpcional(a->getId()) != nullptr) {
        throw std::runtime_error("ID invalido ou ja existente em alguma area da fazenda!");
    }

    Area* destino = buscarArea(nomeArea);
    if (destino == nullptr) {
        throw std::invalid_argument("Area de destino nao encontrada!");
    }

    destino->adicionarAnimal(a, silencioso);
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

void Farm::calcularMeritoTouro(std::string idTouro) {
    if(idTouro.empty()) throw std::invalid_argument("ID do touro invalido!");
    Animal* touro = buscarAnimalGlobal(idTouro);
    if(touro==nullptr) throw std::runtime_error("Touro nao encontrado!");
    if(touro->getTipo()!="touro") throw std::runtime_error("Este animal nao eh um touro");
    Touro* _touro = dynamic_cast<Touro*>(touro);
    if(_touro == nullptr) throw std::runtime_error("Falha ao converter para touro!");

    double mediaPesoFilhos=0;
    int qtdEncontrados = 0;
    std::string* FilhosEncontrados = touro->getFilhos();
    if(FilhosEncontrados==nullptr || touro->getQtdFilhos()==0){
        _touro->setMerito(0.0);
        return;
    }
    for(int i=0; i< touro->getQtdFilhos(); i++){
        try{
           Animal* filhote = buscarAnimalGlobal(FilhosEncontrados[i]);
                if(filhote != nullptr){
                    mediaPesoFilhos += filhote->getPeso();
                    qtdEncontrados++;
           }
        }catch(const std::out_of_range&){
            continue;
        }
    }

    if(qtdEncontrados == 0) {
        _touro->setMerito(0.0);
        return;
    }
    mediaPesoFilhos/=qtdEncontrados;
    //media para bezerros: 180.0, media para adulto 450.0
    // valor de referencia : herdabilidade 0.3 , confiabilidade 5.0
    _touro->setMerito((mediaPesoFilhos - 180.0)*0.3*(qtdEncontrados/(qtdEncontrados+5.0)));
}