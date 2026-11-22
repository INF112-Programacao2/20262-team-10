#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include <cctype>
#include <limits>
#include <string>
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

User* moduloLogin() {
    while(true) {
        limparTela();
        std::cout << "===SISTEMA DE LOGIN===\n" 
                  << "[1] Fazer Login\n" 
                  << "[2] Criar Nova Conta\n" 
                  << "[3] Sair do Sistema\n" 
                  << "Opcao: ";
        
        int opcao;
        std::cin >> opcao;
        if(std::cin.fail()){
            limparBuffer();
            continue;
        }
        limparBuffer();
        
        if(opcao == 3) return nullptr;
        
        std::string login = "", senha = "", nome = "", cargo = "";
        
        if(opcao == 1) {
            std::cout << "\n---LOGIN---\n";
            std::cout << "Login: ";
            std::getline(std::cin >> std::ws, login);
            std::cout << "Senha: ";
            std::getline(std::cin >> std::ws, senha);
            
            User* u = User::carregarUsuario(login, senha);
            if (u != nullptr) {
                std::cout << "Login Realizado com Sucesso.\n";
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
                } catch(const std::exception& e) {
                    delete usuarioRegister;
                    std::cerr << "Erro ao criar conta: " << e.what() << "\n";
                    pausarTela();
                }
            } else {
                std::cout << "Erro: Todos os campos sao obrigatorios!\n";
                pausarTela();
            }
        }
    }
}

Farm* inicializarFazendaDoUsuario(User* usuarioAtual) {
    Farm* fazenda = Farm::carregarFazenda(usuarioAtual);

    if (fazenda == nullptr) {
        limparTela();
        std::cout << "=== CONFIGURACAO INICIAL DA FAZENDA ===\n";
        std::cout << "Nenhuma fazenda encontrada para " << usuarioAtual->getNomeProprietario() << ".\n";

        std::string nomeFazenda, localizacao;
        std::cout << "Nome da Fazenda: ";
        std::getline(std::cin >> std::ws, nomeFazenda);
        std::cout << "Localizacao: ";
        std::getline(std::cin >> std::ws, localizacao);

        fazenda = new Farm(nomeFazenda, localizacao, usuarioAtual);
        try {
            fazenda->salvarFazenda();
            std::cout << "Fazenda criada e salva com sucesso!\n";
            pausarTela();
        } catch (const std::exception& e) {
            delete fazenda;
            throw;
        }
    }
    return fazenda;
}

int main() {
    bool sistemaOk = true;

    while(sistemaOk){
        User * usuarioAtual = nullptr;
        usuarioAtual = moduloLogin();
        if(usuarioAtual == nullptr){
            sistemaOk = false;
            break;
        }
        Farm* fazendaAtual = nullptr;
        try {
            fazendaAtual = inicializarFazendaDoUsuario(usuarioAtual);
        } catch (const std::exception& e) {
            std::cerr << "Erro critico ao carregar fazenda: " << e.what() << "\n";
            delete usuarioAtual;
            pausarTela();
            continue;
        }
        bool sessaoAtiva = true;
        while(sessaoAtiva) {
            limparTela();
            std :: cout << "=== PAINEL DE GESTAO | PROPRIETARIO: " << usuarioAtual->getNomeProprietario() << " ===\n";
            std :: cout << "Area selecionada: " << (fazendaAtual->getAreaAtual().empty() ? "Nenhuma" : fazendaAtual->getAreaAtual()) << "\n\n";
            std :: cout <<"[1] Emitir Relatorio Operacional\n" << "[2] Emitir Relatorio Break Even & Vendas\n" <<"[3] Listar Areas / Pastos\n" << "[4] Criar Nova Area / Pasto\n" << "[5] Selecionar Area Atual e Listar Rebanho\n" 
            << "[6] Adicionar Animal na Area Atual\n" << "[7] Transferir Animal Entre Areas\n" <<"[8] Realizar Cobertura (Touro x Vaca)\n" <<"[9] Registrar Nascimento\n" <<"[10] Promover Animais\n" <<"[11] Atualizar Cotacoes (Dolar, Arroba, Leite, Racoes)\n" << "[12] Salvar Tudo Agora\n" 
            << "[0] Salvar e Voltar ao Login (Logout)\n"
            <<"Opcao: ";
            
            int opcao;
            std::cin >> opcao;
            if (std::cin.fail()) {
                limparBuffer();
                continue;
            }
            limparBuffer();
            
            try{
                switch(opcao){
                    case 1:
                        fazendaAtual->emitirRelatorioOperacional();
                        pausarTela();
                        break;
                    case 2:
                        fazendaAtual->emitirRelatorioBreakEvenEVendas();
                        pausarTela();
                        break;
                    case 3:
                        fazendaAtual->listarAreas();
                        pausarTela();
                        break;
                    case 4: {
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
                        } catch(...) {
                            delete novaArea;
                            throw;
                        }
                        pausarTela();
                        break;
                    }
                    case 5:{
                        std :: string nomeArea;
                        std :: cout << "Nome da area: ";
                        std :: getline(std :: cin>> std::ws, nomeArea);
                        fazendaAtual->selecionarArea(nomeArea);
                        
                        Area* areaSelecionada = fazendaAtual->buscarArea(nomeArea);
                        if(areaSelecionada!=nullptr){
                            areaSelecionada->listarAnimais();
                        }
                        pausarTela();
                        break;
                    }
                    case 6: {
                        std::string areaDestino = fazendaAtual->getAreaAtual();
                        if(areaDestino.empty()){
                            std::cout << "Nenhuma area selecionada! Crie ou selecione uma area antes.\n";
                            pausarTela();
                            break;
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
                        if (!(std::cin >> peso)) { 
                            limparBuffer(); 
                            throw std::invalid_argument("Peso invalido."); 
                        }
                        limparBuffer();
                        
                        std::cout << "Idade (meses): ";
                        if (!(std::cin >> idade)) { 
                            limparBuffer(); 
                            throw std::invalid_argument("Idade invalida."); 
                        }
                        limparBuffer();
                        
                        std::cout << "Quantidade de vacinas aplicadas: ";
                        if (!(std::cin >> vacinas)) { 
                            limparBuffer(); 
                            throw std::invalid_argument("Quantidade de vacinas invalida."); 
                        }
                        limparBuffer();

                        Animal* novoAnimal = nullptr;

                        if (tipoOp == 1) {
                            char lact;
                            double litros = 0.0;
                            std::cout << "Esta em lactacao [S/N]: ";
                            std::cin >> lact;
                            limparBuffer();
                            
                            if(lact == 'S' || lact == 's'){
                                std::cout << "Producao diaria de leite (L): ";
                                if (!(std::cin >> litros)) { 
                                    limparBuffer(); 
                                    throw std::invalid_argument("Litros invalidos."); 
                                }
                                limparBuffer();
                            }
                            
                            std::string id = fazendaAtual->gerarProximoId("vaca");
                            try {
                                Vaca* v = new Vaca(id, peso, idade, 0, vacinas);
                                if(lact == 'S' || lact == 's'){
                                    v->setEmLactacao(true);
                                    v->setProducaoLeiteDia(litros);
                                } else {
                                    v->setEmLactacao(false);
                                }
                                novoAnimal = v;
                            } catch(...) {
                                throw;
                            }
                        }
                        else if (tipoOp == 2) {
                            double valorDose;
                            std::cout << "Valor da dose de semen (R$): ";
                            if (!(std::cin >> valorDose)) { 
                                limparBuffer(); 
                                throw std::invalid_argument("Valor invalido."); 
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
                            try {
                                Boicorte* b = new Boicorte(id, peso, idade, 0, vacinas);
                                b->setFaseEngorda(engorda == 'S' || engorda == 's');
                                b->setrendimentoCarcaca(rendimento);
                                novoAnimal = b;
                            } catch(...) {
                                throw;
                            }
                        }
                        else if (tipoOp == 4) {
                            char sexo;
                            std::cout << "Sexo [M/F]: ";
                            std::cin >> sexo;
                            limparBuffer();
                            
                            std::string id = fazendaAtual->gerarProximoId("bezerro");
                            try {
                                novoAnimal = new Bezerro(id, std::tolower(sexo), peso, idade, 0, vacinas, nullptr, 4.0, 0.7, 0, idade >= 7);
                            } catch(...) {
                                throw;
                            }
                        }
                        else {
                            std::cout << "Opcao Invalida.\n";
                            pausarTela();
                            break;
                        }

                        try {
                            if (novoAnimal != nullptr) {
                                fazendaAtual->adicionarAnimalNaArea(areaDestino, novoAnimal);
                            }
                        } catch(...) {
                            delete novoAnimal;
                            throw;
                        }
                        pausarTela();
                        break;
                    }
                    case 7:{
                        std :: string id, nomeArea;
                        std :: cout << "Id do Animal: ";
                        std :: cin >> id;
                        limparBuffer();
                        std :: cout <<"Area de destino: ";
                        std :: getline(std :: cin>> std::ws, nomeArea);
                        fazendaAtual->transferirAnimalEntreAreas(id, nomeArea);
                        pausarTela();
                        break;   
                    }
                    case 8:{
                        std :: string idVaca, idTouro;
                        std :: cout << "Id da Vaca: ";
                        std :: cin >> idVaca;
                        limparBuffer();
                        std :: cout << "Id do Touro: ";
                        std :: cin >> idTouro;
                        limparBuffer();
                        fazendaAtual->realizarCobertura(idTouro, idVaca);
                        std::cout << "Cobertura registrada com sucesso!\n";
                        pausarTela();
                        break;
                    }
                    case 9: {
                        std::string idMae, areaNascimento;
                        double pesoNascimento;
                        char sexo;
                        std::cout << "Id Vaca(Mae): ";
                        std::cin >> idMae;
                        limparBuffer();
                        
                        std::cout << "Area do Nascimento: ";
                        std::getline(std::cin >> std::ws, areaNascimento);
                        
                        std::cout << "Sexo do bezerro[M/F]: ";
                        std::cin >> sexo;
                        limparBuffer();
                        
                        std::cout << "Peso ao Nascer(kg): ";
                        if (!(std::cin >> pesoNascimento)) {
                            limparBuffer();
                            throw std::invalid_argument("Entrada invalida. O peso deve ser um numero.");
                        }
                        limparBuffer();
                        
                        std::string idGerado = fazendaAtual->registrarNascimento(idMae, areaNascimento, std::tolower(sexo), pesoNascimento);
                        std::cout << "Nascimento registrado com sucesso! ID do novo bezerro: " << idGerado << "\n";
                        pausarTela();
                        break;
                    }
                    case 10:
                        fazendaAtual->promoverAnimalGlobal();
                        pausarTela();
                        break;
                    case 11:
                        usuarioAtual->atualizarCotacoes();
                        pausarTela();
                        break;
                    case 12:
                        fazendaAtual->salvarFazenda();
                        usuarioAtual->salvarEmArquivo();
                        std::cout << "Dados da fazenda, areas, rebanho e cotacoes salvos\n";
                        pausarTela();
                        break;
                    case 0:
                        fazendaAtual->salvarFazenda();
                        usuarioAtual->salvarEmArquivo();
                        sessaoAtiva = false;
                        break;
                    default:
                        std::cout << "Opcao invalida.\n";
                        pausarTela();
                        break;
                }
                
                if(fazendaAtual!=nullptr && opcao!=0)
                    fazendaAtual->salvarFazenda();
                    
            }catch(const std::exception& e){
                std :: cerr << "Erro: " << e.what() << "\n";
                pausarTela();
            }
        }
        delete fazendaAtual;
        delete usuarioAtual;
    }
    std::cout << "Sistema encerrado.\n";
    return 0;
}