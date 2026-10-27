#include "user.hpp"

#include <curl/curl.h>
#include <fstream>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <iostream>
// https://github.com/nlohmann/json
//precisa instalar para importar a bbt -> vamos pegar cotacao 
//do dolar daqui
//eu estou usando um metodo mais rustico para pegar a cotacao delegado ao proprio sistema
//talvez precisemos alterar para funcionar em outros sistemas

User::User(std::string log, std::string senha, 
    std::string nom, std::string perfil) {
    login = log;
    senhaHash = senha;
    idUsuario = nom;
    perfilAcesso = perfil;
    cotacaoDolar = -2; //valor para nao quebrar
    precoArrobaBoi = 384,55;
    precoLeite = 2,88;
};

void User::atualizarCotacoes() {

//garante que vai funcionar no windows tambem
#ifdef _WIN32
    system("curl -s https://economia.awesomeapi.com.br/last/USD-BRL > cotacao.txt");
#else
    system("curl -s \"https://economia.awesomeapi.com.br/last/USD-BRL\" > cotacao.txt");
#endif

    std::ifstream arquivo("cotacao.txt");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de cotacao." << '\n';
        return;
    }
    
    std::string linha;
    std::string conteudoCompleto = "";
    while (std::getline(arquivo, linha)) {
        conteudoCompleto += linha;
    }
    arquivo.close();

    std::string chave = "\"bid\":\"";
    size_t posInicial = conteudoCompleto.find(chave);

    if (posInicial != std::string::npos) {
        // Pula os caracteres da chave para chegar no início do número
        posInicial += chave.length(); 
        
        // Encontra onde o número termina (a próxima aspa)
        size_t posFinal = conteudoCompleto.find("\"", posInicial); 
        
        // Recorta o texto do número e converte para double
        std::string valorStr = conteudoCompleto.substr(posInicial, posFinal - posInicial);
        this->cotacaoDolar = std::stod(valorStr);
    }

    if (cotacaoDolar == -2) {
        std::cout << std::endl; std::cout << "---------------------" << std::endl;
        std::cout << "Atencao valor da cotacao apresenta erro e nao foi modificado" << std::endl;
    } else {
        std::cout << "cotacao do dolar atualizada com sucesso" << std::endl;
    }

};

double User::getCotacao() {
    return cotacaoDolar;
};

void User::setPrecoLeite (double p) {
    precoLeite = p;
};

void User::setPrecoArroba(double p) {
    precoArrobaBoi = p;
};

