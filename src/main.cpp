#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include "farm.hpp"
#include "user.hpp"
#include "vaca.hpp"
#include "touro.hpp"
#include "bezerro.hpp"
#include "boicorte.hpp"

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
            std::getline(std::cin >> std::ws, login);
            std :: cout << "Senha: ";
            std::getline(std::cin >> std::ws, senha);
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
            std::getline(std::cin >> std::ws, login);
            std :: cout << "Senha: ";
            std::getline(std::cin >> std::ws, senha);
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
                    pausarTela();
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
            std :: cout <<"[1] Emitir Relatorio Operacional\n" << "[2] Emitir Relatorio Break Even & Vendas\n" <<"[3] Listar Areas / Pastos\n" << "[4] Criar Nova Area / Pasto\n" << "[5] Selecionar Area Atual e Listar Rebanho\n" 
            << "[6] Adicionar Animal na Area Atual\n" << "[7] Transferir Animal Entre Areas\n" <<"[8] Realizar Cobertura (Touro x Vaca)\n" <<"[9] Registrar Nascimento\n" <<"[10] Promover Animais\n" <<"[11] Atualizar Cotacoes (Dolar, Arroba, Leite, Racoes)\n" << "[12] Salvar Tudo Agora\n" 
            << "[0] Salvar e Voltar ao Login (Logout)\n"
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
                    case 6:{
                        std::string areaDestino = fazendaAtual->getAreaAtual();
                        if(areaDestino.empty()){
                            std::cout << "Nenhuma area selecionada! Crie ou selecione uma area antes.\n";
                            pausarTela();
                            break;
                        }
                        int tipoOp;
                        std :: cout << "Tipo ([1] Vaca | [2] Touro | [3] Boi de Corte | [4] Bezerro):";
                        std :: cin >> tipoOp;
                        if(std::cin.fail()){
                            std::cin.clear();
                            std::cin.ignore(10000, '\n');
                            continue;
                        }
                        double peso;
                        int idade, vacinas;
                        std::cout << "Peso (kg): ";
                        std::cin >> peso;
                        std::cout << "Idade (meses): ";
                        std::cin >> idade;
                        std::cout << "Quantidade de vacinas aplicadas: ";
                        std::cin >> vacinas;
                        Animal* novoAnimal = nullptr;
                        if(tipoOp==1){
                            std::string id = fazendaAtual->gerarProximoId("vaca");
                            Vaca* v = new Vaca(id, peso, idade, 0 , vacinas);
                            char lact;
                            std :: cout << "Esta em lactacao [S/N]: ";
                            std :: cin >> lact;
                            if(lact=='S' || lact == 's'){
                                double litros;
                                std::cout << "Producao diaria de leite (L): ";
                                std::cin >> litros;
                                v->setEmLactacao(true);
                                v->setProducaoLeiteDia(litros);
                            }
                            else if(lact=='N' || lact=='n'){
                                v->setEmLactacao(false);
                            }
                            novoAnimal = v;
                        }
                        else if(tipoOp==2){
                            std::string id = fazendaAtual->gerarProximoId("touro");
                            double valorDose;
                            std::cout << "Valor da dose de semen (R$): ";
                            std::cin >> valorDose;
                            Touro* t = new Touro(id, peso, idade, 0, vacinas, nullptr, valorDose);
                            novoAnimal = t;
                        }
                        else if(tipoOp==3){
                            std::string id = fazendaAtual->gerarProximoId("boicorte");
                            Boicorte* b = new Boicorte(id, peso, idade, 0, vacinas);
                            char engorda;
                            std::cout << "Esta em fase de engorda/confinamento? [S/N]: ";
                            std::cin >> engorda;
                            b->setFaseEngorda(engorda == 'S' || engorda == 's');
                            novoAnimal = b;
                        }
                        else if(tipoOp==4){

                        }
                        else{
                            std :: cout << "Opcao Invalida.";
                            pausarTela();
                            break;
                        }
                        try{
                            fazendaAtual->adicionarAnimalNaArea(areaDestino, novoAnimal);
                        }catch(...){
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
                        std :: cout << "Id do Touro: ";
                        fazendaAtual->realizarCobertura(idTouro, idVaca);
                        std::cout << "Cobertura registrada com sucesso!\n";
                        pausarTela();
                        break;
                    }
                    case 9:{
                        std :: string idMae, areaNascimento;
                        double pesoNascimento;
                        char sexo;
                        std :: cout << "Id Vaca(Mae): ";
                        std :: cin >> idMae;
                        std :: cout << "Area do Nascimento: ";
                        std :: getline(std :: cin >> std::ws, areaNascimento);
                        std :: cout << "Sexo do bezerro[M/F]: ";
                        std :: cin >> sexo;
                        std :: cout << "Peso ao Nascer(kg): ";
                        std :: cin >> pesoNascimento;
                        std :: string idgerado =fazendaAtual->gerarProximoId("bezerro");
                        fazendaAtual->registrarNascimento(idMae, areaNascimento, sexo, pesoNascimento, idgerado);
                        std :: cout << "Nascimento registrado! ID do bezerro: " << idgerado << "\n";
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