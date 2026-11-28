#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>
#include <cctype>
#include <limits>
#include <string>
#include <iomanip>
#include "farm.hpp"
#include "user.hpp"
#include "vaca.hpp"
#include "touro.hpp"
#include "bezerro.hpp"
#include "boicorte.hpp"

void limparTela() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void limparBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pausarTela() {
    std::cout << "\nPressione ENTER para continuar...";
    std::string tmp;
    std::getline(std::cin, tmp);
}

void salvarFazendaComFeedback(Farm* fazendaAtual) {
    if (fazendaAtual == nullptr) return;
    std::cout << "\nSalvando dados da fazenda, aguarde..." << std::flush;
    fazendaAtual->salvarFazenda();
    std::cout << " Concluido!\n";
}

bool existemUsuariosCadastrados() {
    namespace fs = std::filesystem;
    const std::string pastasPossiveis[] = {"usuarios", "users", "data/usuarios", "dados/usuarios"};
    for (const std::string& pasta : pastasPossiveis) {
        if (fs::exists(pasta) && fs::is_directory(pasta)) {
            for (const auto& entry : fs::directory_iterator(pasta)) {
                if (entry.is_regular_file()) {
                    return true;
                }
            }
        }
    }
    const std::string arquivosPossiveis[] = {"./data/usuarios.txt", "./data/users.txt", "usuarios.txt", "users.txt"};
    for (const std::string& caminho : arquivosPossiveis) {
        if (fs::exists(caminho)) {
            std::ifstream arq(caminho);
            if (arq.peek() != std::ifstream::traits_type::eof()) {
                return true;
            }
        }
    }
    return false;
}

User* moduloLogin() {
    while (true) {
        limparTela();
        bool temUsuarios = existemUsuariosCadastrados();

        if (!temUsuarios) {
            std::cout << "--- BEM-VINDO AO SISTEMA DE GESTAO ---\n"
            << "Boas-vindas, verificamos que nunca criou um usuario, deseja cadastrar o primeiro usuario?\n\n"
            << "[1] Cadastrar Primeiro Usuario\n"
            << "[0] Sair do Sistema\n"
            << "Opcao: ";

            int opcao;
            std::cin >> opcao;
            if (std::cin.fail()) {
                limparBuffer();
                continue;
            }
            limparBuffer();

            if (opcao == 0) return nullptr;

            if (opcao == 1) {
                std::string login = "", senha = "", nome = "", cargo = "";
                std::cout << "\n--- CADASTRAR PRIMEIRO USUARIO ---\n";
                std::cout << "Login: ";
                std::getline(std::cin >> std::ws, login);
                std::cout << "Senha: ";
                std::getline(std::cin >> std::ws, senha);
                std::cout << "Nome do proprietario: ";
                std::getline(std::cin >> std::ws, nome);
                std::cout << "Cargo: ";
                std::getline(std::cin >> std::ws, cargo);

                if (!login.empty() && !nome.empty() && !senha.empty() && !cargo.empty()) {
                    User* usuarioRegister = nullptr;
                    try {
                        usuarioRegister = new User(login, senha, nome, cargo);
                        usuarioRegister->salvarEmArquivo();
                        std::cout << "Primeiro usuario criado com sucesso! Agora voce ja pode fazer login.\n";
                        delete usuarioRegister;
                        pausarTela();
                    } catch (const std::exception& e) {
                        delete usuarioRegister;
                        std::cerr << "Erro ao criar conta: " << e.what() << "\n";
                        pausarTela();
                    }
                } else {
                    std::cout << "Erro: Todos os campos sao obrigatorios!\n";
                    pausarTela();
                }
            } else {
                std::cout << "Opcao invalida.\n";
                pausarTela();
            }
            continue;
        }

        std::cout << "---SISTEMA DE LOGIN---\n" << "[1] Fazer Login\n"
        << "[2] Criar Nova Conta\n" << "[3] Sair do Sistema\n"
        << "Opcao: ";
        int opcao;
        std::cin >> opcao;
        if (std::cin.fail()) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 3) return nullptr;

        std::string login = "", senha = "", nome = "", cargo = "";

        if (opcao == 1) {
            std::cout << "\n---LOGIN---\n";
            std::cout << "Login: ";
            std::getline(std::cin >> std::ws, login);
            std::cout << "Senha: ";
            std::getline(std::cin >> std::ws, senha);

            User* u = User::carregarUsuario(login, senha);
            if (u != nullptr) {
                std::cout << "Login realizado com sucesso! Bem-vindo(a), " << u->getNomeProprietario() << "!\n";
                pausarTela();
                return u;
            } else {
                std::cout << "Erro ao autenticar: login ou senha invalidos.\n";
                pausarTela();
            }
        }
        else if (opcao == 2) {
            std::cout << "\n---REGISTER---\n";
            std::cout << "Login: ";
            std::getline(std::cin >> std::ws, login);
            std::cout << "Senha: ";
            std::getline(std::cin >> std::ws, senha);
            std::cout << "Nome do proprietario: ";
            std::getline(std::cin >> std::ws, nome);
            std::cout << "Cargo: ";
            std::getline(std::cin >> std::ws, cargo);

            if (!login.empty() && !nome.empty() && !senha.empty() && !cargo.empty()) {
                User* usuarioRegister = nullptr;
                try {
                    usuarioRegister = new User(login, senha, nome, cargo);
                    usuarioRegister->salvarEmArquivo();
                    std::cout << "Conta criada com sucesso! Por favor, faca login para entrar.\n";
                    delete usuarioRegister;
                    pausarTela();
                } catch (const std::exception& e) {
                    delete usuarioRegister;
                    std::cerr << "Erro ao criar conta: " << e.what() << "\n";
                    pausarTela();
                }
            } else {
                std::cout << "Erro: Todos os campos sao obrigatorios!\n";
                pausarTela();
            }
        }
        else {
            std::cout << "Opcao invalida.\n";
            pausarTela();
        }
    }
}

Farm* criarNovaFazendaFluxo(User* usuarioAtual) {
    std::cout << "\n--- CRIAR NOVA FAZENDA ---\n";
    std::string nomeFazenda, localizacao;
    std::cout << "Nome da Fazenda: ";
    std::getline(std::cin >> std::ws, nomeFazenda);
    std::cout << "Localizacao: ";
    std::getline(std::cin >> std::ws, localizacao);

    if (nomeFazenda.empty() || localizacao.empty()) {
        std::cout << "Erro: Nome e localizacao sao obrigatorios!\n";
        pausarTela();
        return nullptr;
    }

    if (Farm::existeFazendaDoUsuario(usuarioAtual, nomeFazenda)) {
        std::cout << "Erro: Voce ja possui uma fazenda com esse nome!\n";
        pausarTela();
        return nullptr;
    }

    Farm* novaFazenda = nullptr;
    try {
        std::cout << "Criando e salvando fazenda, aguarde..." << std::flush;
        novaFazenda = new Farm(nomeFazenda, localizacao, usuarioAtual);
        novaFazenda->salvarFazenda();
        std::cout << " Concluido!\nFazenda criada e salva com sucesso!\n";
        pausarTela();
        return novaFazenda;
    } catch (const std::exception& e) {
        delete novaFazenda;
        std::cerr << "\nErro ao criar fazenda: " << e.what() << "\n";
        pausarTela();
        return nullptr;
    }
}

Farm* menuMinhasFazendas(User* usuarioAtual) {
    while (true) {
        limparTela();
        std::cout << "=== MINHAS FAZENDAS | PROPRIETARIO: " << usuarioAtual->getNomeProprietario() << " ===\n"
        << "[1] Listar minhas fazendas\n"<< "[2] Selecionar/Abrir uma fazenda\n"
        << "[3] Criar nova fazenda\n"<< "[4] Fazer logout\n"<< "Opcao: ";
        int opcao;
        std::cin >> opcao;
        if (std::cin.fail()) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 4) {
            return nullptr;
        }
        else if (opcao == 1) {
            std::cout << "\n--- FAZENDAS CADASTRADAS ---\n";
            Farm::listarFazendasDoUsuario(usuarioAtual);
            pausarTela();
        }
        else if (opcao == 2) {
            std::cout << "\n--- ABRIR FAZENDA ---\n";
            int total = Farm::listarFazendasDoUsuario(usuarioAtual);
            if (total == 0) {
                char resp;
                std::cout << "Nenhuma fazenda cadastrada ainda. Deseja criar uma agora? [S/N]: ";
                std::cin >> resp;
                limparBuffer();
                if (resp == 'S' || resp == 's') {
                    Farm* criada = criarNovaFazendaFluxo(usuarioAtual);
                    if (criada != nullptr) return criada;
                }
                continue;
            }
            std::string nomeEscolhido;
            std::cout << "\nNome da fazenda que deseja abrir: ";
            std::getline(std::cin >> std::ws, nomeEscolhido);

            std::cout << "Carregando fazenda '" << nomeEscolhido << "', aguarde..." << std::flush;
            Farm* fazenda = Farm::carregarFazenda(usuarioAtual, nomeEscolhido);
            if (fazenda != nullptr) {
                std::cout << " Concluido!\nFazenda '" << nomeEscolhido << "' carregada com sucesso!\n";
                pausarTela();
                return fazenda;
            } else {
                std::cout << "\nErro: Fazenda nao encontrada para este usuario.\n";
                pausarTela();
            }
        }
        else if (opcao == 3) {
            Farm* criada = criarNovaFazendaFluxo(usuarioAtual);
            if (criada != nullptr) return criada;
        }
        else {
            std::cout << "Opcao invalida.\n";
            pausarTela();
        }
    }
}

void importarArquivoEmLote(Farm* fazendaAtual, const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    std::cout << "Importando arquivo '" << caminhoArquivo << "', aguarde...\n" << std::flush;

    std::string linha;
    int areasImportadas = 0;
    int animaisImportados = 0;
    int linhaAtual = 0;

    while (std::getline(arquivo, linha)) {
        linhaAtual++;
        if (!linha.empty() && linha[linha.size() - 1] == '\r') {
            linha.erase(linha.size() - 1);
        }
        if (linha.empty() || linha[0] == '#') {
            continue;
        }

        std::istringstream iss(linha);
        std::string comando;
        if (!(iss >> comando)) {
            continue;
        }

        if (comando == "ANCESTRAL") {
            SnapshotAncestral snap;
            if (!snap.ler(iss)) {
                throw std::runtime_error("Dados invalidos para ancestral na linha " + std::to_string(linhaAtual));
            }
            fazendaAtual->registrarOuAtualizarAncestral(snap);
        }
        else if (comando == "AREA") {
            std::string nomeArea, estadoPasto;
            double hectares;
            if (!(iss >> nomeArea >> estadoPasto >> hectares)) {
                throw std::runtime_error("Erro de formato na linha " + std::to_string(linhaAtual));
            }
            for (size_t i = 0; i < estadoPasto.size(); i++) {
                if (estadoPasto[i] == '_') {
                    estadoPasto[i] = ' ';
                }
            }

            if (fazendaAtual->buscarArea(nomeArea) == nullptr) {
                Area* novaArea = new Area(nomeArea, estadoPasto, hectares);
                try {
                    fazendaAtual->adicionarArea(novaArea, true);
                    areasImportadas++;
                } catch (...) {
                    delete novaArea;
                    throw;
                }
            }
        }
        else if (comando == "ANIMAL") {
            std::string nomeArea, tipo, idToken;
            if (!(iss >> nomeArea >> tipo >> idToken)) {
                throw std::runtime_error("Erro de formato na linha " + std::to_string(linhaAtual));
            }

            std::string id = (idToken == "AUTO") ? fazendaAtual->gerarProximoId(tipo) : idToken;
            Animal* novoAnimal = nullptr;

            try {
                if (tipo == "vaca") {
                    double peso, litros;
                    int idade, lact;
                    if (!(iss >> peso >> idade >> lact >> litros)) {
                        throw std::runtime_error("Dados invalidos para vaca na linha " + std::to_string(linhaAtual));
                    }
                    Vaca* v = new Vaca(id, peso, idade, 0, 0);
                    if (lact != 0) {
                        v->setEmLactacao(true);
                        v->setProducaoLeiteDia(litros);
                    } else {
                        v->setEmLactacao(false);
                    }
                    novoAnimal = v;
                }
                else if (tipo == "touro") {
                    double peso, dose;
                    int idade;
                    if (!(iss >> peso >> idade >> dose)) {
                        throw std::runtime_error("Dados invalidos para touro na linha " + std::to_string(linhaAtual));
                    }
                    novoAnimal = new Touro(id, peso, idade, 0, 0, nullptr, dose);
                }
                else if (tipo == "boicorte") {
                    double peso, rendimento;
                    int idade, eng;
                    if (!(iss >> peso >> idade >> eng >> rendimento)) {
                        throw std::runtime_error("Dados invalidos para boicorte na linha " + std::to_string(linhaAtual));
                    }
                    Boicorte* b = new Boicorte(id, peso, idade, 0, 0);
                    b->setFaseEngorda(eng != 0);
                    b->setrendimentoCarcaca(rendimento);
                    novoAnimal = b;
                }
                else if (tipo == "bezerro") {
                    char sexo;
                    double peso;
                    int idade, desm;
                    if (!(iss >> sexo >> peso >> idade >> desm)) {
                        throw std::runtime_error("Dados invalidos para bezerro na linha " + std::to_string(linhaAtual));
                    }
                    char sexoFmt = static_cast<char>(std::toupper(static_cast<unsigned char>(sexo)));
                    bool desmamado = (desm != 0);
                    double consumoLeite = desmamado ? 0.0 : 4.0;
                    int idadeDesmame = desmamado ? (idade >= 7 ? idade : 7) : 0;
                    novoAnimal = new Bezerro(id, sexoFmt, peso, idade, 0, 0, nullptr, consumoLeite, 0.7, idadeDesmame, desmamado);
                }
                else {
                    throw std::runtime_error("Tipo de animal desconhecido na linha " + std::to_string(linhaAtual));
                }

                std::string idPai, idMae;
                if (iss >> idPai >> idMae) {
                    if (idPai != "_") {
                        novoAnimal->setIdPai(idPai);
                    }
                    if (idMae != "_") {
                        novoAnimal->setIdMae(idMae);
                    }
                    SnapshotAncestral snapPai;
                    SnapshotAncestral snapMae;
                    if (snapPai.ler(iss)) {
                        fazendaAtual->registrarOuAtualizarAncestral(snapPai);
                    }
                    if (snapMae.ler(iss)) {
                        fazendaAtual->registrarOuAtualizarAncestral(snapMae);
                    }
                }

                fazendaAtual->adicionarAnimalNaArea(nomeArea, novoAnimal, true);
                animaisImportados++;
            } catch (...) {
                delete novoAnimal;
                throw;
            }
        }
    }

    arquivo.close();
    std::cout << "Recalculando scores geneticos..." << std::flush;
    fazendaAtual->recalcularScoresGeneticos();
    std::cout << " Concluido!\n";
    std::cout << "Importacao concluida! Areas criadas: " << areasImportadas
              << " | Animais importados: " << animaisImportados << "\n";
}

void menuRelatorios(Farm* fazendaAtual) {
    while (true) {
        limparTela();
        std::cout << "=== RELATORIOS ===\n"<< "[1] Emitir Relatorio Operacional\n"
        << "[2] Emitir Relatorio Break-Even & Vendas\n"
        << "[3] Ranking Top N Animais por Score Genetico\n"
        << "[4] Ranking Top N Animais por Valor de Venda\n"
        << "[0] Voltar\n"<< "Opcao: ";
        int opcao;
        std::cin >> opcao;
        if (std::cin.fail()) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 0) break;

        if (opcao == 1) {
            fazendaAtual->emitirRelatorioOperacional();
            pausarTela();
        }
        else if (opcao == 2) {
            fazendaAtual->emitirRelatorioBreakEvenEVendas();
            pausarTela();
        }
        else if (opcao == 3 || opcao == 4) {
            int topN;
            std::cout << "Quantos animais deseja exibir no ranking? ";
            if (!(std::cin >> topN) || topN <= 0) {
                limparBuffer();
                throw std::invalid_argument("Quantidade invalida. Informe um numero inteiro maior que zero.");
            }
            limparBuffer();

            if (opcao == 3) {
                fazendaAtual->listarTopAnimaisPorScore(topN);
            } else {
                fazendaAtual->listarTopAnimaisPorValorVenda(topN);
            }
            pausarTela();
        }
        else {
            std::cout << "Opcao invalida.\n";
            pausarTela();
        }
    }
}

void menuGerenciarAreas(Farm* fazendaAtual, User* usuarioAtual, bool& alterou) {
    while (true) {
        limparTela();
        std::cout << "=== GERENCIAR AREAS | ATUAL: "<< (fazendaAtual->getAreaAtual().empty() ? "Nenhuma" : fazendaAtual->getAreaAtual()) << " ===\n"
        << "[1] Listar Areas / Pastos\n"<< "[2] Criar Nova Area / Pasto\n"<< "[3] Selecionar Area Atual e Listar Rebanho\n"
        << "[4] Editar Estado do Pasto / Hectares\n"<< "[5] Resumo de Gasto e Sacos de Racao da Area\n"
        << "[6] Remover Area Vazia\n"<< "[0] Voltar\n"
        << "Opcao: ";
        int opcao;
        std::cin >> opcao;
        if (std::cin.fail()) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 0) break;

        if (opcao == 1) {
            fazendaAtual->listarAreas();
            pausarTela();
        }
        else if (opcao == 2) {
            std::string nomeArea, estado;
            double hectares;
            std::cout << "Nome da area: ";
            std::getline(std::cin >> std::ws, nomeArea);
            std::cout << "Estado do pasto (seco/ruim/intermediario/bom/muito bom): ";
            std::getline(std::cin >> std::ws, estado);
            std::cout << "Tamanho (hectares): ";
            if (!(std::cin >> hectares)) {
                limparBuffer();
                throw std::invalid_argument("Entrada invalida. Hectares devem ser um numero.");
            }
            limparBuffer();

            Area* novaArea = new Area(nomeArea, estado, hectares);
            try {
                fazendaAtual->adicionarArea(novaArea);
                alterou = true;
            } catch (...) {
                delete novaArea;
                throw;
            }
            pausarTela();
        }
        else if (opcao == 3) {
            std::string nomeArea;
            std::cout << "Nome da area: ";
            std::getline(std::cin >> std::ws, nomeArea);
            fazendaAtual->selecionarArea(nomeArea);

            Area* areaSelecionada = fazendaAtual->buscarArea(nomeArea);
            if (areaSelecionada != nullptr) {
                areaSelecionada->listarAnimais();
            }
            pausarTela();
        }
        else if (opcao == 4) {
            std::string nomeArea;
            std::cout << "Nome da area a editar: ";
            std::getline(std::cin >> std::ws, nomeArea);
            Area* areaAlvo = fazendaAtual->buscarArea(nomeArea);
            if (areaAlvo == nullptr) {
                throw std::runtime_error("Area nao encontrada na fazenda!");
            }

            std::cout << "[1] Editar estado do pasto\n"<< "[2] Editar tamanho (hectares)\n"
            << "Opcao: ";
            int subOp;
            if (!(std::cin >> subOp)) {
                limparBuffer();
                throw std::invalid_argument("Entrada invalida.");
            }
            limparBuffer();

            if (subOp == 1) {
                std::string novoEstado;
                std::cout << "Novo estado (seco/ruim/intermediario/bom/muito bom): ";
                std::getline(std::cin >> std::ws, novoEstado);
                areaAlvo->setEstadoPasto(novoEstado);
                alterou = true;
                std::cout << "Estado do pasto atualizado com sucesso!\n";
            }
            else if (subOp == 2) {
                double novosHectares;
                std::cout << "Novo tamanho (hectares): ";
                if (!(std::cin >> novosHectares)) {
                    limparBuffer();
                    throw std::invalid_argument("Entrada invalida.");
                }
                limparBuffer();
                areaAlvo->setArea(novosHectares);
                alterou = true;
                std::cout << "Tamanho da area atualizado com sucesso!\n";
            }
            else {
                std::cout << "Opcao invalida.\n";
            }
            pausarTela();
        }
        else if (opcao == 5) {
            std::string nomeArea;
            std::cout << "Nome da area: ";
            std::getline(std::cin >> std::ws, nomeArea);
            Area* areaAlvo = fazendaAtual->buscarArea(nomeArea);
            if (areaAlvo == nullptr) {
                throw std::runtime_error("Area nao encontrada na fazenda!");
            }

            double gasto = areaAlvo->getGastoDaArea(usuarioAtual->getPrecoLeite(), usuarioAtual->getRelatorioRacao());
            ConsumoRacao sacos = areaAlvo->getSacosGastosNaArea(usuarioAtual->getRelatorioRacao());

            std::cout << std::fixed << std::setprecision(2);
            std::cout << "\n--- Resumo da Area: " << areaAlvo->getNomeArea() << " ---\n"
            << "Estado do pasto: " << areaAlvo->getEstadoPasto() << "\n"
            << "Lotacao: " << areaAlvo->getQtdAnimais() << "/" << areaAlvo->getCapacidadeMaxima()
            << " (" << areaAlvo->calcularLotacaoArea() << "%)\n"
            << "Gasto diario estimado: R$ " << gasto << "\n"
            << "Sacos/dia -> Engorda: " << sacos.engorda
            << " | Avulsa: " << sacos.avulsa
            << " | Recria: " << sacos.recria << "\n";
            pausarTela();
        }
        else if (opcao == 6) {
            std::string nomeArea;
            std::cout << "Nome da area vazia a remover: ";
            std::getline(std::cin >> std::ws, nomeArea);
            Area* areaAlvo = fazendaAtual->buscarArea(nomeArea);
            if (areaAlvo == nullptr) {
                throw std::runtime_error("Area nao encontrada na fazenda!");
            }
            char confirma;
            std::cout << "Tem certeza que deseja remover a area '" << nomeArea << "'? [S/N]: ";
            std::cin >> confirma;
            limparBuffer();
            if (confirma == 'S' || confirma == 's') {
                fazendaAtual->removerArea(areaAlvo);
                alterou = true;
            } else {
                std::cout << "Remocao cancelada.\n";
            }
            pausarTela();
        }
        else {
            std::cout << "Opcao invalida.\n";
            pausarTela();
        }
    }
}

void menuGerenciarAnimais(Farm* fazendaAtual, User* usuarioAtual, bool& alterou) {
    while (true) {
        limparTela();
        std::cout << "=== GERENCIAR ANIMAIS | AREA ATUAL: "
        << (fazendaAtual->getAreaAtual().empty() ? "Nenhuma" : fazendaAtual->getAreaAtual()) << " ===\n"
        << "[1] Adicionar Animal na Area Atual\n"
        << "[2] Buscar Animal por ID (Ficha e Gasto Individual)\n"
        << "[3] Atualizar Fase (Lactacao / Engorda / Desmame)\n"
        << "[4] Remover Animal (Venda / Morte / Saida)\n"
        << "[5] Consultar Genealogia (Ancestrais e Filhos)\n"
        << "[0] Voltar\n"
        << "Opcao: ";

        int opcao;
        std::cin >> opcao;
        if (std::cin.fail()) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 0) break;

        if (opcao == 1) {
            std::string areaDestino = fazendaAtual->getAreaAtual();
            if (areaDestino.empty()) {
                std::cout << "Nenhuma area selecionada! Crie ou selecione uma area antes.\n";
                pausarTela();
                continue;
            }
            int tipoOp;
            std::cout << "Tipo ([1] Vaca | [2] Touro | [3] Boi de Corte | [4] Bezerro): ";
            if (!(std::cin >> tipoOp)) {
                limparBuffer();
                throw std::invalid_argument("Entrada invalida.");
            }
            limparBuffer();

            double peso;
            int idade, vacinas;

            std::cout << "Peso (kg): ";
            if (!(std::cin >> peso) || peso <= 0.0) {
                limparBuffer();
                throw std::invalid_argument("Peso invalido. Deve ser maior que zero.");
            }
            limparBuffer();

            std::cout << "Idade (meses): ";
            if (!(std::cin >> idade) || idade < 0) {
                limparBuffer();
                throw std::invalid_argument("Idade invalida. Nao pode ser negativa.");
            }
            limparBuffer();

            std::cout << "Quantidade de vacinas aplicadas: ";
            if (!(std::cin >> vacinas) || vacinas < 0) {
                limparBuffer();
                throw std::invalid_argument("Quantidade de vacinas invalida. Nao pode ser negativa.");
            }
            limparBuffer();

            Animal* novoAnimal = nullptr;

            if (tipoOp == 1) {
                char lact;
                double litros = 0.0;
                std::cout << "Esta em lactacao [S/N]: ";
                std::cin >> lact;
                limparBuffer();

                if (lact == 'S' || lact == 's') {
                    std::cout << "Producao diaria de leite (L): ";
                    if (!(std::cin >> litros) || litros < 0.0) {
                        limparBuffer();
                        throw std::invalid_argument("Litros invalidos. Nao pode ser negativo.");
                    }
                    limparBuffer();
                }

                std::string id = fazendaAtual->gerarProximoId("vaca");
                Vaca* v = new Vaca(id, peso, idade, 0, vacinas);
                if (lact == 'S' || lact == 's') {
                    v->setEmLactacao(true);
                    v->setProducaoLeiteDia(litros);
                } else {
                    v->setEmLactacao(false);
                }
                novoAnimal = v;
            }
            else if (tipoOp == 2) {
                double valorDose;
                std::cout << "Valor da dose de semen (R$): ";
                if (!(std::cin >> valorDose) || valorDose < 0.0) {
                    limparBuffer();
                    throw std::invalid_argument("Valor invalido. Nao pode ser negativo.");
                }
                limparBuffer();

                std::string id = fazendaAtual->gerarProximoId("touro");
                novoAnimal = new Touro(id, peso, idade, 0, vacinas, nullptr, valorDose);
            }
            else if (tipoOp == 3) {
                char engorda;
                double rendimento;
                std::cout << "Esta em fase de engorda/confinamento? [S/N]: ";
                std::cin >> engorda;
                limparBuffer();

                std::cout << "Rendimento de carcaca (%): ";
                if (!(std::cin >> rendimento)) {
                    limparBuffer();
                    throw std::invalid_argument("Rendimento invalido.");
                }
                limparBuffer();

                std::string id = fazendaAtual->gerarProximoId("boicorte");
                Boicorte* b = new Boicorte(id, peso, idade, 0, vacinas);
                b->setFaseEngorda(engorda == 'S' || engorda == 's');
                b->setrendimentoCarcaca(rendimento);
                novoAnimal = b;
            }
            else if (tipoOp == 4) {
                char sexo;
                std::cout << "Sexo [M/F]: ";
                std::cin >> sexo;
                limparBuffer();

                char sexoFmt = static_cast<char>(std::toupper(static_cast<unsigned char>(sexo)));
                if (sexoFmt != 'M' && sexoFmt != 'F') {
                    throw std::invalid_argument("Sexo invalido. Informe M ou F.");
                }
                bool desmamado = (idade >= 7);
                double consumoLeite = desmamado ? 0.0 : 4.0;
                int idadeDesmame = desmamado ? idade : 0;
                std::string id = fazendaAtual->gerarProximoId("bezerro");
                novoAnimal = new Bezerro(id, sexoFmt, peso, idade, 0, vacinas, nullptr, consumoLeite, 0.7, idadeDesmame, desmamado);
            }
            else {
                std::cout << "Opcao Invalida.\n";
                pausarTela();
                continue;
            }

            try {
                if (novoAnimal != nullptr) {
                    fazendaAtual->adicionarAnimalNaArea(areaDestino, novoAnimal);
                    alterou = true;
                }
            } catch (...) {
                delete novoAnimal;
                throw;
            }
            pausarTela();
        }
        else if (opcao == 2) {
            std::string id;
            std::cout << "ID do Animal: ";
            std::cin >> id;
            limparBuffer();

            Animal* animal = fazendaAtual->buscarAnimalGlobal(id);
            double arrobaRef = (animal->getTipo() == "vaca" || (animal->getTipo() == "bezerro" && animal->getSexo() == 'F'))
                ? usuarioAtual->getCotacaoArrobaVaca()
                : usuarioAtual->getCotacaoArrobaBoi();
            animal->calcularValorVendaSugerido(arrobaRef, usuarioAtual->getPrecoLeite());
            animal->imprimirFichaParcial();

            double gastoDia = animal->calcularGastoDiario(usuarioAtual->getPrecoLeite(), usuarioAtual->getRelatorioRacao());
            double sacosDia = animal->calcularSacosDiarios(usuarioAtual->getRelatorioRacao());
            Area* areaDoAnimal = fazendaAtual->buscarAreaDoAnimal(id);

            if (areaDoAnimal != nullptr) {
                gastoDia = areaDoAnimal->getGastoIndividual(id, usuarioAtual->getPrecoLeite(), usuarioAtual->getRelatorioRacao());
                sacosDia = areaDoAnimal->getSacoGastoIndividual(id, usuarioAtual->getRelatorioRacao());
            }

            std::cout << std::fixed << std::setprecision(2);
            if (areaDoAnimal != nullptr) {
                std::cout << "Area atual: " << areaDoAnimal->getNomeArea()
                          << " (Lotacao: " << areaDoAnimal->calcularLotacaoArea() << "%)\n";
            }
            std::cout << "Pai: " << (animal->getPai().empty() ? "Nao registrado" : animal->getPai()) << "\n"
            << "Mae: " << (animal->getMae().empty() ? "Nao registrada" : animal->getMae()) << "\n"
            << "Filhos registrados (" << animal->getQtdFilhos() << "): ";
            if (animal->getQtdFilhos() == 0 || animal->getFilhos() == nullptr) {
                std::cout << "Nenhum";
            } else {
                std::string* listaFilhos = animal->getFilhos();
                for (int i = 0; i < animal->getQtdFilhos(); i++) {
                    if (i > 0) std::cout << ", ";
                    std::cout << listaFilhos[i];
                }
            }
            std::cout << "\n"
            << "Racao: " << (animal->getTipoRacao().empty() ? "Leite" : animal->getTipoRacao())
            << " (" << sacosDia << " sacos/dia)\n"
            << "Gasto diario individual: R$ " << gastoDia << "\n"
            << "Valor de venda sugerido: R$ " << animal->getValorVendaSugerido() << "\n";
            pausarTela();
        }
        else if (opcao == 3) {
            std::string id;
            std::cout << "ID do Animal: ";
            std::cin >> id;
            limparBuffer();

            Animal* animal = fazendaAtual->buscarAnimalGlobal(id);
            std::string tipo = animal->getTipo();

            if (tipo == "vaca") {
                Vaca* v = dynamic_cast<Vaca*>(animal);
                char lact;
                std::cout << "Colocar vaca em lactacao? [S/N]: ";
                std::cin >> lact;
                limparBuffer();

                if (lact == 'S' || lact == 's') {
                    double litros;
                    std::cout << "Producao diaria de leite (L): ";
                    if (!(std::cin >> litros)) {
                        limparBuffer();
                        throw std::invalid_argument("Litros invalidos.");
                    }
                    limparBuffer();
                    v->setEmLactacao(true);
                    v->setProducaoLeiteDia(litros);
                } else {
                    v->setEmLactacao(false);
                }
                alterou = true;
                std::cout << "Fase de lactacao atualizada com sucesso!\n";
            }
            else if (tipo == "boicorte") {
                Boicorte* b = dynamic_cast<Boicorte*>(animal);
                char eng;
                std::cout << "Colocar boi em fase de engorda? [S/N]: ";
                std::cin >> eng;
                limparBuffer();
                b->setFaseEngorda(eng == 'S' || eng == 's');
                alterou = true;
                std::cout << "Fase de engorda atualizada com sucesso!\n";
            }
            else if (tipo == "bezerro") {
                Bezerro* bz = dynamic_cast<Bezerro*>(animal);
                if (bz->realizarDesmame()) {
                    alterou = true;
                    std::cout << "Desmame realizado com sucesso!\n";
                }
            }
            else {
                std::cout << "Este tipo de animal nao possui mudanca de fase.\n";
            }
            pausarTela();
        }
        else if (opcao == 4) {
            std::string id;
            std::cout << "ID do Animal a remover: ";
            std::cin >> id;
            limparBuffer();

            fazendaAtual->buscarAnimalGlobal(id);

            Area* areaAlvo = nullptr;
            if (!fazendaAtual->getAreaAtual().empty()) {
                Area* atual = fazendaAtual->buscarArea(fazendaAtual->getAreaAtual());
                if (atual != nullptr && atual->buscarAnimal(id) != nullptr) {
                    areaAlvo = atual;
                }
            }

            if (areaAlvo == nullptr) {
                std::string nomeArea;
                std::cout << "Nome da area onde o animal se encontra: ";
                std::getline(std::cin >> std::ws, nomeArea);
                areaAlvo = fazendaAtual->buscarArea(nomeArea);
            }

            if (areaAlvo == nullptr) {
                throw std::runtime_error("Area nao encontrada!");
            }

            std::cout << "Motivo da saida ([1] Vendido | [2] Morto | [3] Removido/Outro): ";
            int motivo;
            if (!(std::cin >> motivo)) {
                limparBuffer();
                throw std::invalid_argument("Entrada invalida.");
            }
            limparBuffer();

            std::string statusSaida = "removido";
            if (motivo == 1) statusSaida = "vendido";
            else if (motivo == 2) statusSaida = "morto";

            char confirma;
            std::cout << "Tem certeza que deseja remover o animal '" << id << "' (" << statusSaida << ")? [S/N]: ";
            std::cin >> confirma;
            limparBuffer();
            if (confirma == 'S' || confirma == 's') {
                fazendaAtual->marcarAncestralRemovido(id, statusSaida);
                areaAlvo->removerAnimal(id);
                fazendaAtual->limpezaDeAncestrais();
                alterou = true;
            } else {
                std::cout << "Remocao cancelada.\n";
            }
            pausarTela();
        }
        else if (opcao == 5) {
            std::string id;
            std::cout << "ID do Animal (vivo ou no historico): ";
            std::cin >> id;
            limparBuffer();

            fazendaAtual->exibirAncestrais(id);
            fazendaAtual->exibirFilhos(id);
            pausarTela();
        }
        else {
            std::cout << "Opcao invalida.\n";
            pausarTela();
        }
    }
}

void menuMovimentacaoReproducao(Farm* fazendaAtual, bool& alterou) {
    while (true) {
        limparTela();
        std::cout << "=== MOVIMENTACAO E REPRODUCAO ===\n"
        << "[1] Transferir Animal Entre Areas\n"
        << "[2] Transferir Lote de Animais Entre Areas\n"
        << "[3] Realizar Cobertura (Touro x Vaca)\n"
        << "[4] Registrar Nascimento\n"
        << "[5] Promover Animais\n"
        << "[6] Recalcular Scores Geneticos\n"
        << "[0] Voltar\n"
        << "Opcao: ";

        int opcao;
        std::cin >> opcao;
        if (std::cin.fail()) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 0) break;

        if (opcao == 1) {
            std::string id, nomeArea;
            std::cout << "Id do Animal: ";
            std::cin >> id;
            limparBuffer();
            std::cout << "Area de destino: ";
            std::getline(std::cin >> std::ws, nomeArea);
            fazendaAtual->transferirAnimalEntreAreas(id, nomeArea);
            alterou = true;
            pausarTela();
        }
        else if (opcao == 2) {
            int qtd;
            std::cout << "Quantidade de animais no lote: ";
            if (!(std::cin >> qtd) || qtd <= 0) {
                limparBuffer();
                throw std::invalid_argument("Quantidade invalida.");
            }
            limparBuffer();

            std::string* ids = new std::string[qtd];
            try {
                for (int i = 0; i < qtd; i++) {
                    std::cout << "ID do animal [" << (i + 1) << "/" << qtd << "]: ";
                    std::cin >> ids[i];
                    limparBuffer();
                }
                std::string areaDestino;
                std::cout << "Area de destino: ";
                std::getline(std::cin >> std::ws, areaDestino);

                fazendaAtual->transferirLoteDeAnimaisEntreAreas(ids, qtd, areaDestino);
                alterou = true;
                delete[] ids;
            } catch (...) {
                delete[] ids;
                throw;
            }
            pausarTela();
        }
        else if (opcao == 3) {
            std::string idVaca, idTouro;
            std::cout << "Id da Vaca: ";
            std::cin >> idVaca;
            limparBuffer();
            std::cout << "Id do Touro: ";
            std::cin >> idTouro;
            limparBuffer();
            fazendaAtual->realizarCobertura(idTouro, idVaca);
            alterou = true;
            std::cout << "Cobertura registrada com sucesso!\n";
            pausarTela();
        }
        else if (opcao == 4) {
            std::string idMae, areaNascimento;
            double pesoNascimento;
            char sexo;
            std::cout << "Id Vaca(Mae): ";
            std::cin >> idMae;
            limparBuffer();

            std::cout << "Area do Nascimento: ";
            std::getline(std::cin >> std::ws, areaNascimento);

            std::cout << "Sexo do bezerro [M/F]: ";
            std::cin >> sexo;
            limparBuffer();

            std::cout << "Peso ao Nascer (kg): ";
            if (!(std::cin >> pesoNascimento)) {
                limparBuffer();
                throw std::invalid_argument("Entrada invalida. O peso deve ser um numero.");
            }
            limparBuffer();

            char sexoFmt = static_cast<char>(std::toupper(static_cast<unsigned char>(sexo)));
            std::string idGerado = fazendaAtual->registrarNascimento(idMae, areaNascimento, sexoFmt, pesoNascimento);
            alterou = true;
            std::cout << "Nascimento registrado com sucesso! ID do novo bezerro: " << idGerado << "\n";
            pausarTela();
        }
        else if (opcao == 5) {
            std::cout << "Recalculando scores e promovendo animais, aguarde...\n" << std::flush;
            fazendaAtual->recalcularScoresGeneticos();
            int promovidos = fazendaAtual->promoverAnimalGlobal();
            if (promovidos > 0) {
                alterou = true;
            } else {
                std::cout << "Nenhum animal foi promovido.\n";
            }
            pausarTela();
        }
        else if (opcao == 6) {
            std::cout << "Recalculando scores geneticos, aguarde..." << std::flush;
            fazendaAtual->recalcularScoresGeneticos();
            alterou = true;
            std::cout << " Concluido!\n";
            pausarTela();
        }
        else {
            std::cout << "Opcao invalida.\n";
            pausarTela();
        }
    }
}

void menuCotacoesRacoes(User* usuarioAtual) {
    while (true) {
        limparTela();
        std::cout << "=== COTACOES E RACOES ===\n"
        << "[1] Visualizar Valores Atuais\n"
        << "[2] Atualizar Todas as Cotacoes (API + Manual)\n"
        << "[3] Editar um Unico Valor Manualmente\n"
        << "[0] Voltar\n"
         << "Opcao: ";

        int opcao;
        std::cin >> opcao;
        if (std::cin.fail()) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 0) break;

        if (opcao == 1) {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "\n--- VALORES ATUAIS ---\n"
            << "Cotacao Dolar:   R$ " << usuarioAtual->getCotacaoDolar() << "\n"
            << "Preco Leite (L): R$ " << usuarioAtual->getPrecoLeite() << "\n"
            << "Arroba Boi:      R$ " << usuarioAtual->getCotacaoArrobaBoi() << "\n"
            << "Arroba Vaca:     R$ " << usuarioAtual->getCotacaoArrobaVaca() << "\n"
            << "Racao Engorda:   " << usuarioAtual->getKgRacaoEngorda() << " kg | R$ " << usuarioAtual->getSacaRacaoEngorda() << "\n"
            << "Racao Avulsa:    " << usuarioAtual->getKgRacaoAvulsa() << " kg | R$ " << usuarioAtual->getSacaRacaoAvulsa() << "\n"
            << "Racao Recria:    " << usuarioAtual->getKgRacaoRecria() << " kg | R$ " << usuarioAtual->getSacaRacaoRecria() << "\n";
            pausarTela();
        }
        else if (opcao == 2) {
            usuarioAtual->atualizarCotacoes();
            limparBuffer();
            pausarTela();
        }
        else if (opcao == 3) {
            std::cout << "\nQual valor deseja editar?\n"
            << "[1] Cotacao Dolar (R$)\n"
            << "[2] Preco Litro do Leite (R$)\n"
            << "[3] Arroba do Boi (R$)\n"
            << "[4] Arroba da Vaca (R$)\n"
            << "[5] Preco Saca Racao Engorda (R$)\n"
            << "[6] Peso Saca Racao Engorda (kg)\n"
            << "[7] Preco Saca Racao Avulsa (R$)\n"
            << "[8] Peso Saca Racao Avulsa (kg)\n"
            << "[9] Preco Saca Racao Recria (R$)\n"
            << "[10] Peso Saca Racao Recria (kg)\n"
            << "Opcao: ";
            int campo;
            if (!(std::cin >> campo)) {
                limparBuffer();
                throw std::invalid_argument("Entrada invalida.");
            }
            limparBuffer();

            if (campo < 1 || campo > 10) {
                std::cout << "Opcao invalida.\n";
                pausarTela();
                continue;
            }

            double novoValor;
            std::cout << "Novo valor: ";
            if (!(std::cin >> novoValor) || novoValor <= 0.0) {
                limparBuffer();
                throw std::invalid_argument("Valor invalido. Deve ser maior que zero.");
            }
            limparBuffer();

            if (campo == 1) usuarioAtual->setCotacaoDolar(novoValor);
            else if (campo == 2) usuarioAtual->setPrecoLeite(novoValor);
            else if (campo == 3) usuarioAtual->setPrecoArrobaBoi(novoValor);
            else if (campo == 4) usuarioAtual->setPrecoArrobaVaca(novoValor);
            else if (campo == 5) usuarioAtual->setPrecoSacaRacaoEngorda(novoValor);
            else if (campo == 6) usuarioAtual->setKgRacaoEngorda(novoValor);
            else if (campo == 7) usuarioAtual->setPrecoSacaRacaoAvulsa(novoValor);
            else if (campo == 8) usuarioAtual->setKgRacaoAvulsa(novoValor);
            else if (campo == 9) usuarioAtual->setPrecoSacaRacaoRecria(novoValor);
            else if (campo == 10) usuarioAtual->setKgRacaoRecria(novoValor);

            usuarioAtual->salvarEmArquivo();
            std::cout << "Valor atualizado e salvo com sucesso!\n";
            pausarTela();
        }
        else {
            std::cout << "Opcao invalida.\n";
            pausarTela();
        }
    }
}

int main(int argc, char* argv[]) {
    bool sistemaOk = true;
    bool importouArquivoInicial = false;

    while (sistemaOk) {
        User* usuarioAtual = moduloLogin();
        if (usuarioAtual == nullptr) {
            sistemaOk = false;
            break;
        }

        bool usuarioLogado = true;
        while (usuarioLogado) {
            Farm* fazendaAtual = nullptr;
            try {
                fazendaAtual = menuMinhasFazendas(usuarioAtual);
            } catch (const std::exception& e) {
                std::cerr << "Erro critico ao selecionar fazenda: " << e.what() << "\n";
                pausarTela();
                continue;
            }

            if (fazendaAtual == nullptr) {
                usuarioLogado = false;
                break;
            }

            if (argc > 1 && !importouArquivoInicial) {
                try {
                    importarArquivoEmLote(fazendaAtual, argv[1]);
                    salvarFazendaComFeedback(fazendaAtual);
                } catch (const std::exception& e) {
                    std::cerr << "Erro ao importar arquivo inicial: " << e.what() << "\n";
                }
                importouArquivoInicial = true;
                pausarTela();
            }

            bool sessaoAtiva = true;
            while (sessaoAtiva) {
                limparTela();
                std::cout << "=== PAINEL DE GESTAO | PROPRIETARIO: " << usuarioAtual->getNomeProprietario() << " ===\n";
                std::cout << "Area selecionada: " << (fazendaAtual->getAreaAtual().empty() ? "Nenhuma" : fazendaAtual->getAreaAtual()) << "\n\n";
                std::cout << "[1] Relatorios\n"
                << "[2] Gerenciar Areas\n"
                << "[3] Gerenciar Animais\n"
                << "[4] Movimentacao e Reproducao\n"
                << "[5] Cotacoes e Racoes\n"
                << "[6] Importar Lote (.txt)\n"
                << "[7] Salvar Tudo Agora\n"
                << "[8] Trocar de Fazenda\n"
                << "[0] Salvar e Voltar ao Login (Logout)\n"
                << "Opcao: ";

                int opcao;
                std::cin >> opcao;
                if (std::cin.fail()) {
                    limparBuffer();
                    continue;
                }
                limparBuffer();

                bool precisaSalvar = false;

                try {
                    switch (opcao) {
                        case 1:
                            menuRelatorios(fazendaAtual);
                            break;
                        case 2:
                            menuGerenciarAreas(fazendaAtual, usuarioAtual, precisaSalvar);
                            break;
                        case 3:
                            menuGerenciarAnimais(fazendaAtual, usuarioAtual, precisaSalvar);
                            break;
                        case 4:
                            menuMovimentacaoReproducao(fazendaAtual, precisaSalvar);
                            break;
                        case 5:
                            menuCotacoesRacoes(usuarioAtual);
                            break;
                        case 6: {
                            std::string caminhoArquivo;
                            std::cout << "Caminho/nome do arquivo (.txt): ";
                            std::getline(std::cin >> std::ws, caminhoArquivo);
                            importarArquivoEmLote(fazendaAtual, caminhoArquivo);
                            salvarFazendaComFeedback(fazendaAtual);
                            pausarTela();
                            break;
                        }
                        case 7:
                            salvarFazendaComFeedback(fazendaAtual);
                            usuarioAtual->salvarEmArquivo();
                            std::cout << "Dados da fazenda, areas, rebanho e cotacoes salvos.\n";
                            pausarTela();
                            break;
                        case 8:
                            salvarFazendaComFeedback(fazendaAtual);
                            usuarioAtual->salvarEmArquivo();
                            sessaoAtiva = false;
                            break;
                        case 0:
                            salvarFazendaComFeedback(fazendaAtual);
                            usuarioAtual->salvarEmArquivo();
                            sessaoAtiva = false;
                            usuarioLogado = false;
                            break;
                        default:
                            std::cout << "Opcao invalida.\n";
                            pausarTela();
                            break;
                    }

                    if (precisaSalvar && fazendaAtual != nullptr) {
                        salvarFazendaComFeedback(fazendaAtual);
                    }

                } catch (const std::exception& e) {
                    if (precisaSalvar && fazendaAtual != nullptr) {
                        salvarFazendaComFeedback(fazendaAtual);
                    }
                    std::cerr << "Erro: " << e.what() << "\n";
                    pausarTela();
                }
            }
            delete fazendaAtual;
        }
        delete usuarioAtual;
    }
    std::cout << "Sistema encerrado.\n";
    return 0;
}