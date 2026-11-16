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
    precoArrobaBoi = 384.55;
    precoLeite = 2.88;
};

void User::atualizarCotacaoDolar() {

//garante que vai funcionar no windows tambem
#ifdef _WIN32
    system("curl -s https://economia.awesomeapi.com.br/last/USD-BRL > ./data/cotacao.txt");
#else
    system("curl -s \"https://economia.awesomeapi.com.br/last/USD-BRL\" > ./data/cotacao.txt");
#endif

    std::ifstream arquivo("./data/cotacao.txt");
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

void User::atualizarCotacaoBoi() {
#ifdef _WIN32
    system("curl -s https://agrodocai.com.br/api/v1/cotacao > ./data/cotacao_boi.txt");
#else
    system("curl -s \"https://agrodocai.com.br/api/v1/cotacao\" > ./data/cotacao_boi.txt");
#endif

    std::ifstream arquivo("./data/cotacao_boi.txt"); 
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de cotacao do boi.\n";
        return;
    }

    std::string linha;
    std::string conteudoCompleto = "";
    while (std::getline(arquivo, linha)) {
        conteudoCompleto += linha;
    }
    arquivo.close();

    // A chave na API é boi_gordo_cepea_sp
    std::string chave = "\"boi_gordo_cepea_sp\":";
    size_t posInicial = conteudoCompleto.find(chave);

    if (posInicial != std::string::npos) {
        // Pula os caracteres da chave para chegar no início do número
        posInicial += chave.length();
        
        //pula espacos em branco
        while (posInicial < conteudoCompleto.length() && std::isspace(conteudoCompleto[posInicial])) {
            posInicial++;
        }

        size_t posFinal = conteudoCompleto.find(",", posInicial);
        
        //npos significa final da string
        if (posFinal != std::string::npos) {
            std::string valorStr = conteudoCompleto.substr(posInicial, posFinal - posInicial);
            
            this->precoArrobaBoi = std::stod(valorStr); 
        }
    }

    std::string chaveVaca = "\"vaca_gorda\":";
    size_t posVaca = conteudoCompleto.find(chaveVaca);

    if (posVaca != std::string::npos) {
        posVaca += chaveVaca.length();
        while (posVaca < conteudoCompleto.length() && std::isspace(conteudoCompleto[posVaca])) posVaca++;
        
        size_t posFinalVaca = conteudoCompleto.find(",", posVaca);
        if (posFinalVaca == std::string::npos) { // Caso seja o último item do JSON
            posFinalVaca = conteudoCompleto.find("}", posVaca);
        }
        
        if (posFinalVaca != std::string::npos) {
            std::string valorStr = conteudoCompleto.substr(posVaca, posFinalVaca - posVaca);
            this->precoArrobaVaca = std::stod(valorStr); 
        }
    }

    // Validações
    if (this->precoArrobaBoi <= 0 || this->precoArrobaVaca <= 0) { 
        std::cout << std::endl << "----------------------" << std::endl;
        std::cout << "Atencao: Um ou mais valores de cotacao apresentam erro ou nao foram encontrados." << std::endl;
    } else {
        std::cout << "Cotacoes de boi e vaca atualizadas com sucesso!" << std::endl;
    }
}


double User::getCotacaoDolar() {
    return cotacaoDolar;
};

double User::getCotacaoArrobaBoi() {
    return precoArrobaBoi;
}

double User::getCotacaoArrobaVaca() {
    return precoArrobaVaca;
}

void User::setPrecoLeite (double p) {
    precoLeite = p;
};

void User::setPrecoArroba(double p) {
    precoArrobaBoi = p;
};

