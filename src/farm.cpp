#include "farm.hpp"
#include "animal.hpp"
#include "touro.hpp"
#include "boicorte.hpp"
#include "vaca.hpp"
#include "bezerro.hpp"
#include <algorithm>
#include <fstream>
#include <ostream>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <filesystem>
#include <iostream>
#include <stdexcept>

static const int GERACOES_AVALIADAS = 3;
static const double SCORE_REFERENCIA = 50.0;
static const double PONTOS_POR_ANCESTRAL = 2.0;

static bool compararAncestralPorId(const SnapshotAncestral& a, const SnapshotAncestral& b) {
    return a.get_id() < b.get_id();
}

static void ordenarAncestrais(SnapshotAncestral* arr, int n) {
    if (arr == nullptr || n <= 1) return;
    std::sort(arr, arr + n, compararAncestralPorId);
}

static int buscarAncestralBinario(const SnapshotAncestral* arr, int n, const std::string& id) {
    int esq = 0, dir = n - 1;
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        const std::string& atual = arr[meio].get_id();
        if (atual == id) return meio;
        if (atual < id) esq = meio + 1;
        else dir = meio - 1;
    }
    return -1;
}

static bool compararAnimalPorId(const Animal* a, const Animal* b) {
    return a->getId() < b->getId();
}

static void ordenarAnimais(Animal** arr, int n) {
    if (arr == nullptr || n <= 1) return;
    std::sort(arr, arr + n, compararAnimalPorId);
}

static int buscarIndiceAnimalBinario(Animal** arr, int n, const std::string& id) {
    int esq = 0, dir = n - 1;
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        const std::string& atual = arr[meio]->getId();
        if (atual == id) return meio;
        if (atual < id) esq = meio + 1;
        else dir = meio - 1;
    }
    return -1;
}

static int contarAncestraisRec(const SnapshotAncestral* arr, int n, const std::string& id, int niveis) {
    if (id.empty() || niveis <= 0) return 0;
    int idx = buscarAncestralBinario(arr, n, id);
    if (idx == -1 || !arr[idx].get_Registrado()) return 0;
    return 1 + contarAncestraisRec(arr, n, arr[idx].getIdPai(), niveis - 1)
             + contarAncestraisRec(arr, n, arr[idx].getIdMae(), niveis - 1);
}

static double somarEfeitoAncestraisRec(const SnapshotAncestral* arr, int n, const std::string& id,
                                       int niveis, double pesoGeracao, double media) {
    if (id.empty() || niveis <= 0) return 0.0;
    int idx = buscarAncestralBinario(arr, n, id);
    if (idx == -1 || !arr[idx].get_Registrado()) return 0.0;

    double efeito = 0.0;
    if (arr[idx].get_ScoreBase() > 0.0) {
        efeito = ((arr[idx].get_ScoreBase() - media) / SCORE_REFERENCIA) * PONTOS_POR_ANCESTRAL * pesoGeracao;
    }
    return efeito
         + somarEfeitoAncestraisRec(arr, n, arr[idx].getIdPai(), niveis - 1, pesoGeracao * 0.5, media)
         + somarEfeitoAncestraisRec(arr, n, arr[idx].getIdMae(), niveis - 1, pesoGeracao * 0.5, media);
}

static void marcarAncestraisNecessariosRec(const SnapshotAncestral* arr, int n,
                                           const std::string& id, bool* necessario) {
    if (id.empty()) return;
    int idx = buscarAncestralBinario(arr, n, id);
    if (idx == -1 || necessario[idx]) return;
    necessario[idx] = true;
    marcarAncestraisNecessariosRec(arr, n, arr[idx].getIdPai(), necessario);
    marcarAncestraisNecessariosRec(arr, n, arr[idx].getIdMae(), necessario);
}

static std::string rotuloGeracao(int nivel, const std::string& lado) {
    if (nivel == 1) return lado;
    if (nivel == 2) return "Avo (" + lado + ")";
    if (nivel == 3) return "Bisavo (" + lado + ")";
    return "Geracao " + std::to_string(nivel) + " (" + lado + ")";
}

static void exibirAncestraisRec(const SnapshotAncestral* arr, int n, const std::string& id,
                                const std::string& lado, int nivel, int maxGeracoes) {
    const int LIMITE_SEGURANCA = 20;
    if (id.empty() || nivel > LIMITE_SEGURANCA) return;
    if (maxGeracoes > 0 && nivel > maxGeracoes) return;

    std::string indent(nivel * 2, ' ');
    std::string rotulo = rotuloGeracao(nivel, lado);
    int idx = buscarAncestralBinario(arr, n, id);

    if (idx == -1 || !arr[idx].get_Registrado()) {
        std::cout << indent << rotulo << ": " << id << " (sem registro detalhado)\n";
        return;
    }

    const SnapshotAncestral& s = arr[idx];
    std::cout << indent << rotulo << ": " << s.get_id()
              << " | Sexo: " << s.get_Sexo()
              << " | Peso: " << s.get_UltimoPeso() << " kg"
              << " | Idade: " << s.getUltimaIdadeMeses() << "m"
              << " | Score base: " << s.get_ScoreBase()
              << " | Status: " << (s.get_Status().empty() ? "desconhecido" : s.get_Status()) << "\n";

    exibirAncestraisRec(arr, n, s.getIdPai(), "Pai", nivel + 1, maxGeracoes);
    exibirAncestraisRec(arr, n, s.getIdMae(), "Mae", nivel + 1, maxGeracoes);
}

static int obterMaiorProximoIdGlobal() {
    std::ifstream arqFaz("./data/fazendas.txt");
    if (!arqFaz.is_open()) {
        return 1;
    }

    std::string donoLido, nomeLido, locLida, areaAtualLida;
    int baixasLidas, diaLido, proxIdLido;
    double caixaLido, lucroLido, prejuizoLido;
    int maior = 1;

    while (arqFaz >> donoLido >> nomeLido >> locLida >> areaAtualLida
                  >> baixasLidas >> caixaLido >> lucroLido >> prejuizoLido >> diaLido >> proxIdLido) {
        if (proxIdLido > maior) {
            maior = proxIdLido;
        }
    }

    arqFaz.close();
    return maior;
}

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

    ancestrais = nullptr;
    qtdAncestrais = 0;
    capacidadeAncestrais = 0;

    qtdTotalDeAnimais = 0;
    qtdBaixas = 0;
    saldoCaixa = 0.0;
    lucroAcumuladoVendas = 0.0;
    prejuizoAcumuladoMortes = 0.0;
    diaAtualSimulacao = 0;
    proximoIdAnimal = obterMaiorProximoIdGlobal();

    carregarAncestrais();
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
    delete[] ancestrais;
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

void Farm::carregarAncestrais() {
    delete[] ancestrais;
    ancestrais = nullptr;
    qtdAncestrais = 0;
    capacidadeAncestrais = 0;

    std::ifstream arq("./data/ancestrais.txt");
    if (!arq.is_open()) {
        return;
    }

    SnapshotAncestral aux;
    while (aux.ler(arq)) {
        if (aux.get_id().empty()) continue;
        if (qtdAncestrais == capacidadeAncestrais) {
            int novaCap = (capacidadeAncestrais == 0) ? 16 : capacidadeAncestrais * 2;
            SnapshotAncestral* novo = new SnapshotAncestral[novaCap];
            for (int i = 0; i < qtdAncestrais; i++) {
                novo[i] = ancestrais[i];
            }
            delete[] ancestrais;
            ancestrais = novo;
            capacidadeAncestrais = novaCap;
        }
        ancestrais[qtdAncestrais++] = aux;
    }
    arq.close();

    if (qtdAncestrais > 1) {
        ordenarAncestrais(ancestrais, qtdAncestrais);
        int unico = 0;
        for (int i = 0; i < qtdAncestrais; i++) {
            if (unico > 0 && ancestrais[i].get_id() == ancestrais[unico - 1].get_id()) {
                ancestrais[unico - 1] = ancestrais[i];
            } else {
                ancestrais[unico++] = ancestrais[i];
            }
        }
        qtdAncestrais = unico;
    }
}

void Farm::salvarAncestrais() const {
    std::filesystem::create_directories("./data");
    std::ofstream temp("./data/ancestrais_temp.txt");
    if (!temp.is_open()) {
        throw std::runtime_error("Falha ao criar arquivo temporario de ancestrais.");
    }

    try {
        for (int i = 0; i < qtdAncestrais; i++) {
            if (ancestrais[i].get_Registrado() && !ancestrais[i].get_id().empty()) {
                ancestrais[i].escrever(temp);
                temp << "\n";
            }
        }
    } catch (...) {
        temp.close();
        std::remove("./data/ancestrais_temp.txt");
        throw;
    }

    temp.close();
    std::remove("./data/ancestrais.txt");
    std::rename("./data/ancestrais_temp.txt", "./data/ancestrais.txt");
}

void Farm::registrarOuAtualizarAncestral(const SnapshotAncestral& snap) {
    if (!snap.get_Registrado() || snap.get_id().empty()) return;

    int idx = buscarAncestralBinario(ancestrais, qtdAncestrais, snap.get_id());
    if (idx != -1) {
        ancestrais[idx].atualizarDados(snap.get_UltimoPeso(), snap.get_Status(),
                                       snap.get_ScoreBase(), snap.getIdPai(), snap.getIdMae(),
                                       snap.getUltimaIdadeMeses());
        return;
    }

    if (qtdAncestrais == capacidadeAncestrais) {
        int novaCap = (capacidadeAncestrais == 0) ? 16 : capacidadeAncestrais * 2;
        SnapshotAncestral* novo = new SnapshotAncestral[novaCap];
        for (int i = 0; i < qtdAncestrais; i++) {
            novo[i] = ancestrais[i];
        }
        delete[] ancestrais;
        ancestrais = novo;
        capacidadeAncestrais = novaCap;
    }

    int pos = qtdAncestrais;
    while (pos > 0 && ancestrais[pos - 1].get_id() > snap.get_id()) {
        ancestrais[pos] = ancestrais[pos - 1];
        pos--;
    }
    ancestrais[pos] = snap;
    qtdAncestrais++;
}

void Farm::registrarAncestralDeAnimal(Animal* a, const std::string& status) {
    if (a == nullptr || a->getId().empty()) return;
    registrarOuAtualizarAncestral(a->criarSnapshot(status));
}

void Farm::marcarAncestralRemovido(const std::string& id, const std::string& status) {
    if (id.empty()) return;
    Animal* animal = encontrarAnimalOpcional(id);
    if (animal != nullptr) {
        bool temVinculo = !animal->getPai().empty() || !animal->getMae().empty() || animal->getQtdFilhos() > 0;

        if (!temVinculo) {
            for (int i = 0; i < qtdAreas && !temVinculo; i++) {
                if (area[i] == nullptr) continue;
                Animal** rebanho = area[i]->getRebanho();
                int qtd = area[i]->getQtdAnimais();
                for (int j = 0; j < qtd; j++) {
                    if (rebanho[j] == nullptr) continue;
                    if (rebanho[j]->getPai() == id || rebanho[j]->getMae() == id) {
                        temVinculo = true;
                        break;
                    }
                    Vaca* v = dynamic_cast<Vaca*>(rebanho[j]);
                    if (v != nullptr && v->getPrenha() && v->getIdTouroPai() == id) {
                        temVinculo = true;
                        break;
                    }
                }
            }
        }

        if (!temVinculo) {
            for (int i = 0; i < qtdAncestrais; i++) {
                if (ancestrais[i].getIdPai() == id || ancestrais[i].getIdMae() == id) {
                    temVinculo = true;
                    break;
                }
            }
        }

        if (temVinculo) {
            animal->calcularScoreBase();
            registrarAncestralDeAnimal(animal, status);
        }
        return;
    }

    int idx = buscarAncestralBinario(ancestrais, qtdAncestrais, id);
    if (idx != -1) {
        ancestrais[idx].setStatus(status);
    }
}

void Farm::limpezaDeAncestrais() {
    if (ancestrais == nullptr || qtdAncestrais == 0) return;

    int totalVivos = getQuantidadeTotalAnimais();
    if (totalVivos == 0) {
        qtdAncestrais = 0;
        return;
    }

    ordenarAncestrais(ancestrais, qtdAncestrais);

    Animal** vivos = new Animal*[totalVivos];
    bool* necessario = new bool[qtdAncestrais];

    try {
        int idxVivos = 0;
        for (int i = 0; i < qtdAreas; ++i) {
            if (area[i] == nullptr) continue;
            Animal** rebanho = area[i]->getRebanho();
            int qtd = area[i]->getQtdAnimais();
            for (int j = 0; j < qtd; ++j) {
                if (rebanho[j] != nullptr) {
                    vivos[idxVivos++] = rebanho[j];
                }
            }
        }
        ordenarAnimais(vivos, idxVivos);

        for (int i = 0; i < qtdAncestrais; ++i) {
            necessario[i] = false;
        }

        // 1. Marca ascendentes de animais vivos, touros de vacas prenhas e filhos registrados em idFilhos
        for (int i = 0; i < idxVivos; ++i) {
            Animal* a = vivos[i];
            bool temPaiOuMae = !a->getPai().empty() || !a->getMae().empty();

            if (temPaiOuMae) {
                marcarAncestraisNecessariosRec(ancestrais, qtdAncestrais, a->getPai(), necessario);
                marcarAncestraisNecessariosRec(ancestrais, qtdAncestrais, a->getMae(), necessario);
            }

            Vaca* v = dynamic_cast<Vaca*>(a);
            if (v != nullptr && v->getPrenha() && !v->getIdTouroPai().empty()) {
                marcarAncestraisNecessariosRec(ancestrais, qtdAncestrais, v->getIdTouroPai(), necessario);
            }

            std::string* filhos = a->getFilhos();
            for (int k = 0; filhos != nullptr && k < a->getQtdFilhos(); ++k) {
                int idxFilho = buscarAncestralBinario(ancestrais, qtdAncestrais, filhos[k]);
                if (idxFilho != -1) {
                    necessario[idxFilho] = true;
                }
            }

            if (temPaiOuMae || a->getQtdFilhos() > 0) {
                int idxProprio = buscarAncestralBinario(ancestrais, qtdAncestrais, a->getId());
                if (idxProprio != -1) {
                    necessario[idxProprio] = true;
                }
            }
        }

        // 2. Marca filhos (vivos, vendidos, mortos ou removidos) que ainda possuem pai ou mae vivos na fazenda
        for (int i = 0; i < qtdAncestrais; ++i) {
            const std::string& idPai = ancestrais[i].getIdPai();
            const std::string& idMae = ancestrais[i].getIdMae();
            bool paiVivo = !idPai.empty() && buscarIndiceAnimalBinario(vivos, idxVivos, idPai) != -1;
            bool maeViva = !idMae.empty() && buscarIndiceAnimalBinario(vivos, idxVivos, idMae) != -1;

            if (paiVivo || maeViva) {
                necessario[i] = true;
                if (paiVivo) {
                    int idxPai = buscarAncestralBinario(ancestrais, qtdAncestrais, idPai);
                    if (idxPai != -1) necessario[idxPai] = true;
                }
                if (maeViva) {
                    int idxMae = buscarAncestralBinario(ancestrais, qtdAncestrais, idMae);
                    if (idxMae != -1) necessario[idxMae] = true;
                }
            }
        }

        // 3. Compacta o array descartando quem nao possui mais parentes vivos na fazenda
        int novoQtd = 0;
        for (int i = 0; i < qtdAncestrais; ++i) {
            if (necessario[i]) {
                if (novoQtd != i) {
                    ancestrais[novoQtd] = ancestrais[i];
                }
                novoQtd++;
            }
        }
        qtdAncestrais = novoQtd;
    } catch (...) {
        delete[] necessario;
        delete[] vivos;
        throw;
    }

    delete[] necessario;
    delete[] vivos;
}

const SnapshotAncestral* Farm::buscarAncestral(const std::string& id) const {
    if (id.empty()) return nullptr;
    int idx = buscarAncestralBinario(ancestrais, qtdAncestrais, id);
    return (idx != -1) ? &ancestrais[idx] : nullptr;
}

void Farm::exibirAncestrais(const std::string& idAnimal, int maxGeracoes) const {
    Animal* vivo = encontrarAnimalOpcional(idAnimal);
    const SnapshotAncestral* hist = buscarAncestral(idAnimal);

    if (vivo == nullptr && hist == nullptr) {
        std::cout << "Animal '" << idAnimal << "' nao encontrado na fazenda nem no historico genealogico.\n";
        return;
    }

    std::string idPai = (vivo != nullptr) ? vivo->getPai() : hist->getIdPai();
    std::string idMae = (vivo != nullptr) ? vivo->getMae() : hist->getIdMae();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n--- Ancestrais de " << idAnimal << " ---\n";
    if (idPai.empty() && idMae.empty()) {
        std::cout << "  Nenhum ancestral registrado.\n";
        return;
    }

    exibirAncestraisRec(ancestrais, qtdAncestrais, idPai, "Pai", 1, maxGeracoes);
    exibirAncestraisRec(ancestrais, qtdAncestrais, idMae, "Mae", 1, maxGeracoes);
}

void Farm::exibirFilhos(const std::string& idAnimal) const {
    Animal* vivo = encontrarAnimalOpcional(idAnimal);
    const SnapshotAncestral* hist = buscarAncestral(idAnimal);

    if (vivo == nullptr && hist == nullptr) {
        std::cout << "Animal '" << idAnimal << "' nao encontrado na fazenda nem no historico genealogico.\n";
        return;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n--- Filhos de " << idAnimal << " ---\n";

    int encontrados = 0;
    for (int i = 0; i < qtdAncestrais; ++i) {
        if (!ancestrais[i].get_Registrado()) continue;
        if (ancestrais[i].getIdPai() == idAnimal || ancestrais[i].getIdMae() == idAnimal) {
            const SnapshotAncestral& f = ancestrais[i];
            Animal* filhoVivo = encontrarAnimalOpcional(f.get_id());
            double pesoExibir = (filhoVivo != nullptr) ? filhoVivo->getPeso() : f.get_UltimoPeso();
            int idadeExibir = (filhoVivo != nullptr) ? filhoVivo->getIdadeMeses() : f.getUltimaIdadeMeses();
            double scoreExibir = (filhoVivo != nullptr) ? filhoVivo->getScoreBase() : f.get_ScoreBase();
            std::string statusExibir = (filhoVivo != nullptr) ? "ativo" : (f.get_Status().empty() ? "removido" : f.get_Status());

            std::cout << "  - " << f.get_id()
                      << " | Sexo: " << f.get_Sexo()
                      << " | Peso: " << pesoExibir << " kg"
                      << " | Idade: " << idadeExibir << "m"
                      << " | Score base: " << scoreExibir
                      << " | Status: " << statusExibir << "\n";
            encontrados++;
        }
    }

    if (encontrados == 0) {
        std::cout << "  Nenhum filho registrado.\n";
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

double Farm::calcularMediaScoreBase() const {
    double soma = 0.0;
    int n = 0;
    for (int i = 0; i < qtdAreas; ++i) {
        if (area[i] == nullptr) continue;
        Animal** rebanho = area[i]->getRebanho();
        for (int j = 0; j < area[i]->getQtdAnimais(); ++j) {
            if (rebanho[j] != nullptr && rebanho[j]->getScoreBase() > 0.0) {
                soma += rebanho[j]->getScoreBase();
                n++;
            }
        }
    }
    return (n > 0) ? soma / n : 50.0;
}

int Farm::coletarAnimaisVivosOrdenados(Animal** destino) {
    int idx = 0;
    for (int i = 0; i < qtdAreas; ++i) {
        if (area[i] == nullptr) continue;
        Animal** rebanho = area[i]->getRebanho();
        int qtd = area[i]->getQtdAnimais();
        for (int j = 0; j < qtd; ++j) {
            Animal* atual = rebanho[j];
            if (atual == nullptr) continue;
            atual->calcularScoreBase();
            destino[idx++] = atual;
        }
    }
    ordenarAnimais(destino, idx);
    return idx;
}

void Farm::atualizarAncestraisComVinculo(Animal** vivos, int qtdVivos) {
    bool* temVinculo = new bool[qtdVivos];
    for (int i = 0; i < qtdVivos; ++i) temVinculo[i] = false;

    try {
        // 1. Marca animais vivos que possuem pai, mae, filhos ou cobertura ativa
        for (int i = 0; i < qtdVivos; ++i) {
            Animal* a = vivos[i];
            if (!a->getPai().empty() || !a->getMae().empty() || a->getQtdFilhos() > 0) {
                temVinculo[i] = true;
            }
            if (!a->getPai().empty()) {
                int idxPai = buscarIndiceAnimalBinario(vivos, qtdVivos, a->getPai());
                if (idxPai != -1) temVinculo[idxPai] = true;
            }
            if (!a->getMae().empty()) {
                int idxMae = buscarIndiceAnimalBinario(vivos, qtdVivos, a->getMae());
                if (idxMae != -1) temVinculo[idxMae] = true;
            }
            Vaca* v = dynamic_cast<Vaca*>(a);
            if (v != nullptr && v->getPrenha() && !v->getIdTouroPai().empty()) {
                int idxTouro = buscarIndiceAnimalBinario(vivos, qtdVivos, v->getIdTouroPai());
                if (idxTouro != -1) temVinculo[idxTouro] = true;
            }
        }

        // Pais vivos de filhos ja vendidos/mortos/removidos tambem possuem vinculo
        for (int i = 0; i < qtdAncestrais; ++i) {
            if (!ancestrais[i].getIdPai().empty()) {
                int idxPai = buscarIndiceAnimalBinario(vivos, qtdVivos, ancestrais[i].getIdPai());
                if (idxPai != -1) temVinculo[idxPai] = true;
            }
            if (!ancestrais[i].getIdMae().empty()) {
                int idxMae = buscarIndiceAnimalBinario(vivos, qtdVivos, ancestrais[i].getIdMae());
                if (idxMae != -1) temVinculo[idxMae] = true;
            }
        }

        // 2. Insere ou atualiza na tabela de ancestrais apenas quem tem vinculo
        int qtdAntes = qtdAncestrais;
        for (int i = 0; i < qtdVivos; ++i) {
            if (!temVinculo[i]) continue;
            Animal* atual = vivos[i];

            int idxAnc = buscarAncestralBinario(ancestrais, qtdAntes, atual->getId());
            if (idxAnc != -1) {
                ancestrais[idxAnc].atualizarDados(atual->getPeso(), "ativo", atual->getScoreBase(),
                                                  atual->getPai(), atual->getMae(), atual->getIdadeMeses());
            } else {
                if (qtdAncestrais == capacidadeAncestrais) {
                    int novaCap = (capacidadeAncestrais == 0) ? 16 : capacidadeAncestrais * 2;
                    SnapshotAncestral* novo = new SnapshotAncestral[novaCap];
                    for (int c = 0; c < qtdAncestrais; c++) {
                        novo[c] = ancestrais[c];
                    }
                    delete[] ancestrais;
                    ancestrais = novo;
                    capacidadeAncestrais = novaCap;
                }
                ancestrais[qtdAncestrais++] = atual->criarSnapshot("ativo");
            }
        }

        if (qtdAncestrais > qtdAntes) {
            ordenarAncestrais(ancestrais, qtdAncestrais);
        }

        // 3. Remove da tabela registros sem nenhum parente vivo na fazenda
        limpezaDeAncestrais();
    } catch (...) {
        delete[] temVinculo;
        throw;
    }

    delete[] temVinculo;
}

void Farm::calcularAjusteDeDescendentes(Animal** vivos, int qtdVivos, double mediaRebanho, double* ajusteDesc) const {
    const double PONTOS_POR_DESCENDENTE = 1.5;
    const int IDADE_MINIMA_DESCENDENTE = 6;

    for (int i = 0; i < qtdVivos; ++i) ajusteDesc[i] = 0.0;

    for (int k = 0; k < qtdAncestrais; ++k) {
        const SnapshotAncestral& filho = ancestrais[k];
        if (!filho.get_Registrado()) continue;
        if (filho.getUltimaIdadeMeses() < IDADE_MINIMA_DESCENDENTE || filho.get_ScoreBase() <= 0.0) continue;

        double contrib = ((filho.get_ScoreBase() - mediaRebanho) / SCORE_REFERENCIA) * PONTOS_POR_DESCENDENTE;

        const std::string& idPai = filho.getIdPai();
        const std::string& idMae = filho.getIdMae();

        if (!idPai.empty()) {
            int idxPai = buscarIndiceAnimalBinario(vivos, qtdVivos, idPai);
            if (idxPai != -1) ajusteDesc[idxPai] += contrib;
        }
        if (!idMae.empty() && idMae != idPai) {
            int idxMae = buscarIndiceAnimalBinario(vivos, qtdVivos, idMae);
            if (idxMae != -1) ajusteDesc[idxMae] += contrib;
        }
    }
}

void Farm::aplicarScoresFinais(Animal** vivos, int qtdVivos, double mediaRebanho, const double* ajusteDesc) {
    const double TETO_AJUSTE_DESCENDENTES = 6.0;

    for (int i = 0; i < qtdVivos; ++i) {
        Animal* animal = vivos[i];

        int qtdAnc = contarAncestraisRec(ancestrais, qtdAncestrais, animal->getPai(), GERACOES_AVALIADAS)
                   + contarAncestraisRec(ancestrais, qtdAncestrais, animal->getMae(), GERACOES_AVALIADAS);

        double ajusteAnc = somarEfeitoAncestraisRec(ancestrais, qtdAncestrais, animal->getPai(), GERACOES_AVALIADAS, 1.0, mediaRebanho)
                         + somarEfeitoAncestraisRec(ancestrais, qtdAncestrais, animal->getMae(), GERACOES_AVALIADAS, 1.0, mediaRebanho);

        double descLim = ajusteDesc[i];
        if (descLim > TETO_AJUSTE_DESCENDENTES) descLim = TETO_AJUSTE_DESCENDENTES;
        if (descLim < -TETO_AJUSTE_DESCENDENTES) descLim = -TETO_AJUSTE_DESCENDENTES;

        animal->setAjusteDescendentes(descLim);
        animal->recalcularScoreGenetico(qtdAnc, ajusteAnc);
    }
}

void Farm::sincronizarFilhosNosPais(Animal** vivos, int qtdVivos) {
    for (int k = 0; k < qtdAncestrais; ++k) {
        const SnapshotAncestral& filho = ancestrais[k];
        if (!filho.get_Registrado() || filho.get_id().empty()) continue;

        const std::string& idPai = filho.getIdPai();
        const std::string& idMae = filho.getIdMae();

        if (!idPai.empty()) {
            int idxPai = buscarIndiceAnimalBinario(vivos, qtdVivos, idPai);
            if (idxPai != -1) {
                vivos[idxPai]->adicionarFilho(filho.get_id());
            }
        }
        if (!idMae.empty() && idMae != idPai) {
            int idxMae = buscarIndiceAnimalBinario(vivos, qtdVivos, idMae);
            if (idxMae != -1) {
                vivos[idxMae]->adicionarFilho(filho.get_id());
            }
        }
    }
}

// Recalcula o score genetico em O(N log N) considerando ancestrais e todos os filhos (vivos ou vendidos/removidos)
void Farm::recalcularScoresGeneticos() {
    int totalAnimais = getQuantidadeTotalAnimais();
    if (totalAnimais == 0) {
        limpezaDeAncestrais();
        return;
    }

    Animal** todos = new Animal*[totalAnimais];
    double* ajusteDesc = nullptr;

    try {
        int qtdVivos = coletarAnimaisVivosOrdenados(todos);
        ordenarAncestrais(ancestrais, qtdAncestrais);

        atualizarAncestraisComVinculo(todos, qtdVivos);
        sincronizarFilhosNosPais(todos, qtdVivos);

        double mediaRebanho = calcularMediaScoreBase();
        ajusteDesc = new double[qtdVivos];
        calcularAjusteDeDescendentes(todos, qtdVivos, mediaRebanho, ajusteDesc);
        aplicarScoresFinais(todos, qtdVivos, mediaRebanho, ajusteDesc);
        recalcularValoresVenda();
    } catch (...) {
        delete[] ajusteDesc;
        delete[] todos;
        throw;
    }

    delete[] ajusteDesc;
    delete[] todos;
}

void Farm::recalcularValoresVenda() {
    if (proprietario == nullptr) return;

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
                rebanho[j]->calcularValorVendaSugerido(arrobaVaca, precoLeite);
            } else if (tipo == "bezerro") {
                double arrobaRef = (rebanho[j]->getSexo() == 'F') ? arrobaVaca : arrobaBoi;
                rebanho[j]->calcularValorVendaSugerido(arrobaRef, precoLeite);
            } else {
                rebanho[j]->calcularValorVendaSugerido(arrobaBoi, precoLeite);
            }
        }
    }
}

static bool compararPorScoreDecrescente(const Animal* a, const Animal* b) {
    if (a->getScoreGenetico() != b->getScoreGenetico()) {
        return a->getScoreGenetico() > b->getScoreGenetico();
    }
    if (a->getScoreBase() != b->getScoreBase()) {
        return a->getScoreBase() > b->getScoreBase();
    }
    return a->getId() < b->getId();
}

static bool compararPorValorVendaDecrescente(const Animal* a, const Animal* b) {
    if (a->getValorVendaSugerido() != b->getValorVendaSugerido()) {
        return a->getValorVendaSugerido() > b->getValorVendaSugerido();
    }
    if (a->getScoreGenetico() != b->getScoreGenetico()) {
        return a->getScoreGenetico() > b->getScoreGenetico();
    }
    return a->getId() < b->getId();
}

static void exibirRankingAnimais(Farm* fazenda, Area** areas, int qtdAreas, int quantidade, bool porValorVenda) {
    if (quantidade <= 0) {
        throw std::invalid_argument("A quantidade de animais do ranking deve ser maior que zero!");
    }

    int totalAnimais = fazenda->getQuantidadeTotalAnimais();
    if (totalAnimais <= 0) {
        std::cout << "\nNenhum animal cadastrado na fazenda para gerar ranking.\n";
        return;
    }

    fazenda->recalcularScoresGeneticos();

    Animal** lista = new Animal*[totalAnimais];
    int qtdColetada = 0;

    try {
        for (int i = 0; i < qtdAreas; i++) {
            if (areas[i] == nullptr) continue;
            Animal** rebanho = areas[i]->getRebanho();
            int qtd = areas[i]->getQtdAnimais();
            for (int j = 0; j < qtd; j++) {
                if (rebanho[j] != nullptr) {
                    lista[qtdColetada++] = rebanho[j];
                }
            }
        }

        if (porValorVenda) {
            std::sort(lista, lista + qtdColetada, compararPorValorVendaDecrescente);
        } else {
            std::sort(lista, lista + qtdColetada, compararPorScoreDecrescente);
        }

        int limite = (quantidade < qtdColetada) ? quantidade : qtdColetada;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n=== TOP " << limite << " ANIMAIS POR "
                  << (porValorVenda ? "VALOR DE VENDA SUGERIDO" : "SCORE GENETICO")
                  << " (Total no rebanho: " << qtdColetada << ") ===\n";

        for (int i = 0; i < limite; i++) {
            Animal* a = lista[i];
            Area* areaAtualAnimal = fazenda->buscarAreaDoAnimal(a->getId());
            std::string nomeArea = (areaAtualAnimal != nullptr) ? areaAtualAnimal->getNomeArea() : "Sem area";

            std::cout << "#" << (i + 1)
                      << " | ID: " << a->getId()
                      << " | Tipo: " << a->getTipo()
                      << " | Area: " << nomeArea
                      << " | Peso: " << a->getPeso() << " kg"
                      << " | Idade: " << a->getIdadeMeses() << "m"
                      << " | Filhos: " << a->getQtdFilhos();

            if (porValorVenda) {
                std::cout << " | Valor venda: R$ " << a->getValorVendaSugerido()
                          << " | Score: " << a->getScoreGenetico() << "\n";
            } else {
                std::cout << " | Score: " << a->getScoreGenetico()
                          << " (base: " << a->getScoreBase() << ")"
                          << " | Valor venda: R$ " << a->getValorVendaSugerido() << "\n";
            }
        }
    } catch (...) {
        delete[] lista;
        throw;
    }

    delete[] lista;
}

void Farm::listarTopAnimaisPorScore(int quantidade) {
    exibirRankingAnimais(this, area, qtdAreas, quantidade, false);
}

void Farm::listarTopAnimaisPorValorVenda(int quantidade) {
    exibirRankingAnimais(this, area, qtdAreas, quantidade, true);
}

static void salvarAnimaisPorTipoEmLote(const std::string& tipo, const std::string& caminho,
                                       const std::string& caminhoTemp, Animal** vivosOrdenados, int qtdVivos) {
    bool temAnimalDoTipo = false;
    for (int i = 0; i < qtdVivos; i++) {
        if (vivosOrdenados[i]->getTipo() == tipo) {
            temAnimalDoTipo = true;
            break;
        }
    }
    if (!temAnimalDoTipo) return;

    std::ifstream leitura(caminho);
    std::ofstream temp(caminhoTemp);
    if (!temp.is_open()) {
        if (leitura.is_open()) leitura.close();
        throw std::runtime_error("Falha ao criar arquivo temporario: " + caminhoTemp);
    }

    try {
        if (leitura.is_open()) {
            std::string linha;
            while (std::getline(leitura, linha)) {
                std::istringstream iss(linha);
                std::string idLido;
                if (!(iss >> idLido)) continue;

                int idx = buscarIndiceAnimalBinario(vivosOrdenados, qtdVivos, idLido);
                if (idx == -1) {
                    idx = buscarIndiceAnimalBinario(vivosOrdenados, qtdVivos, formatarTextoFarmCarregar(idLido));
                }
                if (idx == -1 || vivosOrdenados[idx]->getTipo() != tipo) {
                    temp << linha << "\n";
                }
            }
            leitura.close();
        }

        for (int i = 0; i < qtdVivos; i++) {
            if (vivosOrdenados[i]->getTipo() == tipo) {
                vivosOrdenados[i]->escreverLinhaArquivo(temp);
            }
        }
    } catch (...) {
        if (leitura.is_open()) leitura.close();
        temp.close();
        std::remove(caminhoTemp.c_str());
        throw;
    }

    temp.close();
    std::remove(caminho.c_str());
    std::rename(caminhoTemp.c_str(), caminho.c_str());
}

void Farm::salvarFazenda() {
    if (proprietario == nullptr) throw std::runtime_error("Proprietario invalido para salvar fazenda!");
    if (nome.empty()) throw std::invalid_argument("Nome da fazenda invalido!");

    recalcularScoresGeneticos();

    std::filesystem::create_directories("./data");
    std::string idDono = proprietario->getId();

    //Salva tabela compartilhada de ancestrais (ja filtrada e limpa)
    salvarAncestrais();

    //Salva dados da fazenda
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

    //Salva areas da fazenda
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

    //Salva todos os animais em 1 unica passada por especie
    int totalAnimais = getQuantidadeTotalAnimais();
    if (totalAnimais > 0) {
        Animal** vivos = new Animal*[totalAnimais];
        try {
            int qtdVivos = 0;
            for (int i = 0; i < qtdAreas; i++) {
                if (area[i] == nullptr) continue;
                Animal** rebanho = area[i]->getRebanho();
                int qtd = area[i]->getQtdAnimais();
                for (int j = 0; j < qtd; j++) {
                    if (rebanho[j] != nullptr) {
                        vivos[qtdVivos++] = rebanho[j];
                    }
                }
            }
            ordenarAnimais(vivos, qtdVivos);

            salvarAnimaisPorTipoEmLote("vaca", "./data/vacas.txt", "./data/vacas_temp.txt", vivos, qtdVivos);
            salvarAnimaisPorTipoEmLote("touro", "./data/touros.txt", "./data/touros_temp.txt", vivos, qtdVivos);
            salvarAnimaisPorTipoEmLote("boicorte", "./data/boicortes.txt", "./data/boicortes_temp.txt", vivos, qtdVivos);
            salvarAnimaisPorTipoEmLote("bezerro", "./data/bezerros.txt", "./data/bezerros_temp.txt", vivos, qtdVivos);
        } catch (...) {
            delete[] vivos;
            throw;
        }
        delete[] vivos;
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
                int maiorGlobal = obterMaiorProximoIdGlobal();
                int proxBase = (proxIdLido > 0) ? proxIdLido : 1;
                fazendaCarregada->proximoIdAnimal = (proxBase > maiorGlobal) ? proxBase : maiorGlobal;
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

int Farm::promoverAnimalGlobal() {
    if (area == nullptr || qtdAreas == 0) {
        throw std::runtime_error("Nenhuma area alocada na fazenda!");
    }

    int totalPromovidos = 0;
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr) {
            totalPromovidos += area[i]->promoverAnimal();
        }
    }
    return totalPromovidos;
}

Animal* Farm::buscarAnimalGlobal(std::string identificador) {
    Animal* encontrado = encontrarAnimalOpcional(identificador);
    if (encontrado != nullptr) {
        return encontrado;
    }
    throw std::out_of_range("Animal nao encontrado em nenhuma area!");
}

Area* Farm::buscarAreaDoAnimal(const std::string& id) const {
    if (area == nullptr || id.empty()) return nullptr;
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i] != nullptr && area[i]->buscarAnimal(id) != nullptr) {
            return area[i];
        }
    }
    return nullptr;
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
    if (area[indexDestino]->buscarAnimal(idAnimal) != nullptr) {
        throw std::runtime_error("Ja existe um animal com este ID na area de destino.");
    }

    Animal* temporario = area[indexOrigem]->extrairAnimal(idAnimal);
    if (temporario == nullptr) {
        throw std::runtime_error("Erro ao extrair animal da area de origem.");
    }

    try {
        area[indexDestino]->adicionarAnimal(temporario);
    } catch (...) {
        area[indexOrigem]->adicionarAnimal(temporario, true);
        throw;
    }

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
    registrarAncestralDeAnimal(touro, "ativo");
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

    if (id.empty()) {
        id = gerarProximoId("bezerro");
    }

    Bezerro* filhote = new Bezerro(id, sexo, peso);
    bool inseridoNaArea = false;
    try {
        std::string idPai = mae->getIdTouroPai();
        filhote->setIdMae(idFemea);
        filhote->setIdPai(idPai);

        mae->calcularScoreBase();
        registrarAncestralDeAnimal(mae, "ativo");

        Animal* machoVivo = idPai.empty() ? nullptr : encontrarAnimalOpcional(idPai);
        if (machoVivo != nullptr) {
            machoVivo->calcularScoreBase();
            registrarAncestralDeAnimal(machoVivo, "ativo");
        }

        double mediaRebanho = calcularMediaScoreBase();
        int qtdAnc = contarAncestraisRec(ancestrais, qtdAncestrais, idPai, GERACOES_AVALIADAS)
                   + contarAncestraisRec(ancestrais, qtdAncestrais, idFemea, GERACOES_AVALIADAS);
        double ajusteAnc = somarEfeitoAncestraisRec(ancestrais, qtdAncestrais, idPai, GERACOES_AVALIADAS, 1.0, mediaRebanho)
                         + somarEfeitoAncestraisRec(ancestrais, qtdAncestrais, idFemea, GERACOES_AVALIADAS, 1.0, mediaRebanho);

        filhote->setPesoNascimento(peso);
        filhote->calcularScoreBase();
        filhote->recalcularScoreGenetico(qtdAnc, ajusteAnc);

        destino->adicionarAnimal(filhote);
        inseridoNaArea = true;

        mae->parir();
        mae->adicionarFilho(id);

        if (machoVivo != nullptr) {
            machoVivo->adicionarFilho(id);
        }

        registrarAncestralDeAnimal(filhote, "ativo");
    } catch (...) {
        if (!inseridoNaArea) {
            delete filhote;
        }
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

int Farm::listarFazendasDoUsuario(User* proprietario) {
    if (proprietario == nullptr) throw std::invalid_argument("Proprietario invalido!");

    std::ifstream arqFaz("./data/fazendas.txt");
    if (!arqFaz.is_open()) {
        std::cout << "Nenhuma fazenda cadastrada para este usuario.\n";
        return 0;
    }

    std::string idDono = proprietario->getId();
    std::string donoLido, nomeLido, locLida, areaAtualLida;
    int baixasLidas, diaLido, proxIdLido;
    double caixaLido, lucroLido, prejuizoLido;
    int contador = 0;

    while (arqFaz >> donoLido >> nomeLido >> locLida >> areaAtualLida
                  >> baixasLidas >> caixaLido >> lucroLido >> prejuizoLido >> diaLido >> proxIdLido) {
        std::string donoFmt = formatarTextoFarmCarregar(donoLido);
        if (donoFmt == idDono) {
            contador++;
            std::string nomeFmt = formatarTextoFarmCarregar(nomeLido);
            std::string locFmt = formatarTextoFarmCarregar(locLida);
            std::cout << "[" << contador << "] " << nomeFmt
                      << " | Localizacao: " << locFmt
                      << " | Dia: " << diaLido << "\n";
        }
    }

    arqFaz.close();

    if (contador == 0) {
        std::cout << "Nenhuma fazenda cadastrada para este usuario.\n";
    }

    return contador;
}

bool Farm::existeFazendaDoUsuario(User* proprietario, const std::string& nomeFazenda) {
    if (proprietario == nullptr) throw std::invalid_argument("Proprietario invalido!");

    std::ifstream arqFaz("./data/fazendas.txt");
    if (!arqFaz.is_open()) {
        return false;
    }

    std::string idDono = proprietario->getId();
    std::string donoLido, nomeLido, locLida, areaAtualLida;
    int baixasLidas, diaLido, proxIdLido;
    double caixaLido, lucroLido, prejuizoLido;

    while (arqFaz >> donoLido >> nomeLido >> locLida >> areaAtualLida
                  >> baixasLidas >> caixaLido >> lucroLido >> prejuizoLido >> diaLido >> proxIdLido) {
        std::string donoFmt = formatarTextoFarmCarregar(donoLido);
        std::string nomeFmt = formatarTextoFarmCarregar(nomeLido);
        if (donoFmt == idDono && nomeFmt == nomeFazenda) {
            arqFaz.close();
            return true;
        }
    }

    arqFaz.close();
    return false;
}