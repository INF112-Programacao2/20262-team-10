#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include "farm.hpp"
#include "user.hpp"

void limparTela() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void pausarTela() {
    std::cout << "\nPressione ENTER para continuar...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void prepararDiretorios() { //verifica se realmente existe a pasta data na raiz 
    if (!std::filesystem::exists("./data")) {
        std::filesystem::create_directories("./data");
    }
}

User* moduloLogin() {
    while(1==1){
        limparTela();
        std:: cout << "===SISTEMA DE LOGIN===\n" << 
        "[1] Fazer Login\n" << "[2] Criar Nova Conta\n" <<
        "[3] Sair do Sistema\n" << "Opcao: ";
        int opcao;
        std :: cin >> opcao;
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        if(opcao==3) return nullptr;
        std::string login="", senha="", nome="", cargo="";
        if(opcao==1){
            std :: cout <<"\n---LOGIN---\n";
            std :: cout << "Login: ";
            std :: cin >> login;
            std :: cout << "Senha: ";
            std :: cin >> senha;
            if(User::autenticar(login, senha)){
                std :: cout << "Login Realizado com Sucesso.\n";
                return User::carregarUsuario(login, senha);
            }
            else{
                std :: cout << "Erro ao autenticar: login ou senha inválidos.\n";
                std :: cout << "Pressione ENTER para tentar novamente...";
                std :: cin.ignore(10000, '\n');
                std :: cin.get();
            }
        }
        else if(opcao==2){
            std :: cout <<"\n---REGISTER---\n";
            std :: cout << "Login: ";
            std :: cin >> login;
            std :: cout << "Senha: ";
            std :: cin >> senha;
            std :: cout << "Nome do proprietario: ";
            std::getline(std::cin >> std::ws, nome); //std::ws pra limpar o enter
            std :: cout << "Cargo: ";
            std::getline(std::cin >> std::ws, cargo);
            if(!login.empty() && !nome.empty() && !senha.empty() && !cargo.empty()){
                User* usuarioRegister = new User(login, senha, nome, cargo);
                try{
                usuarioRegister->salvarEmArquivo();
                std::cout << "Conta criada com sucesso!\n";
                return usuarioRegister;
                }catch(const std::exception& e){
                    delete usuarioRegister;
                    std::cerr <<"Erro ao criar conta: " << e.what() << "\n";
                }
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
        fazenda->salvarFazenda();
        std::cout << "Fazenda criada e salva com sucesso!\n";
        pausarTela();
    }
    return fazenda;
}

int main() {
    prepararDiretorios();
    
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
            std :: cout << "[3] Listar Areas / Pastos\n" << "[4] Criar Nova Area / Pasto\n" <<"[11] Atualizar Cotacoes (Dolar, Arroba, Leite, Racoes)\n"
            << "[12] Salvar Tudo Agora\n" << "[0] Salvar e Voltar ao Login (Logout)\n"
            <<"Opcao: ";
            int opcao;
            std::cin >> opcao;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }
            try{
                switch(opcao){
                    case 3:
                        fazendaAtual->listarAreas();
                        pausarTela();
                        break;
                    case 4: {
                        std :: string nomeArea, estado;
                        double hectares;
                        std :: cout <<"Nome da area: ";
                        std :: getline(std :: cin>> std::ws, nomeArea);
                        std :: cout << "Estado do pasto (seco/ruim/intermediario/bom/muito bom): ";
                        std :: getline(std::cin >> std::ws, estado);
                        std :: cout << "Tamanho (hectares): ";
                        std :: cin >> hectares;
                        Area* novaArea = new Area(nomeArea, estado, hectares);
                        try{
                            fazendaAtual->adicionarArea(novaArea);
                        }catch(...){
                            delete novaArea;
                            throw;
                        }
                        pausarTela();
                        break;
                    }
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
            }catch(const std::exception& e){
                std :: cerr << "Erro: " << e.what() << "\n";
            }
        }
        delete fazendaAtual;
        delete usuarioAtual;
    }
    std::cout << "Sistema encerrado.\n";
    return 0;
}