#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include "farm.hpp"
#include "user.hpp"

void limparTela() {
    std::cout << "\033[2J\033[H" << std::flush;
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
        "Fazer Login?[1]\n" << "Criar Nova Conta\n[2]" <<
        "Sair do Sistema[3]\n";
        int opcao;
        std :: cin >> opcao;
        if(opcao==3) return nullptr;
        std::string login="", senha="", nome="", cargo="";
        if(opcao==1){
            std :: cout <<"\n---LOGIN---\n";
            std :: cout << "Login: ";
            std :: cin >> login;
            std :: cout << "Senha: ";
            std :: cin >> senha;
            if(User::autenticar(login, senha)){
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
            if(login !="" && nome!="" && senha!="" && cargo!=""){
                User* usuarioRegister = new User(login, senha, nome, cargo);
                usuarioRegister->salvarEmArquivo();
                return usuarioRegister;
            }
        }
    }
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
        delete usuarioAtual;
    }

    return 0;
}