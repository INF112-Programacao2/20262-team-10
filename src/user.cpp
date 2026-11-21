#include "user.hpp"

#include <curl/curl.h>
#include <fstream>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <iostream>
#include <cstdio>

// https://github.com/nlohmann/json
//precisa instalar para importar a bbt -> vamos pegar cotacao 
//do dolar daqui
//eu estou usando um metodo mais rustico para pegar a cotacao delegado ao proprio sistema
//talvez precisemos alterar para funcionar em outros sistemas

std::string User::gerarProximoId() {
    std::ifstream arquivo("./data/usuarios.txt");
    if (!arquivo.is_open()) {
        return "1";
    }

    int maiorId = 0;
    int idLido;
    std::string loginLido, hashLido, nomeLido, perfilLido;
    double dolarLido, leiteLido, boiLido, vacaLido;
    double pesoEngorda, precoEngorda, pesoAvulsa, precoAvulsa, pesoRecria, precoRecria;

    while (arquivo >> idLido >> loginLido >> hashLido >> nomeLido >> perfilLido
                   >> dolarLido >> leiteLido >> boiLido >> vacaLido
                   >> pesoEngorda >> precoEngorda >> pesoAvulsa >> precoAvulsa >> pesoRecria >> precoRecria) {
        if (idLido > maiorId) {
            maiorId = idLido;
        }
    }

    arquivo.close();
    return std::to_string(maiorId + 1);
}

std::string User::gerarHash(const std::string& senha) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(senha));
}

User::User(std::string log, std::string senha, 
    std::string nom, std::string perfil) {
    login = log;
    senhaHash = gerarHash(senha);
    nomeProprietario = nom;
    precoArrobaVaca = 320.55;
    perfilAcesso = perfil;
    idUsuario = gerarProximoId();
    cotacaoDolar = -2; //valor para nao quebrar
    precoArrobaBoi = 384.55;
    precoLeite = 2.88;
    //todos os valores sao sacas de 40kg

    TiposRacoes = new SacaRacao*[3];
    for (int i = 0; i < 3; i++) {
        TiposRacoes[i] = new SacaRacao();
    }

    TiposRacoes[0]->name = "Racao Engorda";
    TiposRacoes[0]->peso = 40.0;
    TiposRacoes[0]->preco = 140;

    TiposRacoes[1]->name = "Racao Avulsa";
    TiposRacoes[1]->peso = 40.0;
    TiposRacoes[1]->preco = 60;

    TiposRacoes[2]->name = "Racao Recria";
    TiposRacoes[2]->peso = 40;
    TiposRacoes[2]->preco = 80;

};

User::~User() {
    for (int i = 0; i < 3; i++) {
        delete TiposRacoes[i];
    }
    delete[] TiposRacoes;
}

void User::salvarEmArquivo() {
    std::string loginFormatado = login.empty() ? "_" : login;
    std::string nomeFormatado = nomeProprietario.empty() ? "_" : nomeProprietario;
    std::string perfilFormatado = perfilAcesso.empty() ? "_" : perfilAcesso;

    for (size_t i = 0; i < loginFormatado.length(); i++) {
        if (loginFormatado[i] == ' ') loginFormatado[i] = '_';
    }
    for (size_t i = 0; i < nomeFormatado.length(); i++) {
        if (nomeFormatado[i] == ' ') nomeFormatado[i] = '_';
    }
    for (size_t i = 0; i < perfilFormatado.length(); i++) {
        if (perfilFormatado[i] == ' ') perfilFormatado[i] = '_';
    }

    std::ifstream leitura("./data/usuarios.txt");
    std::ofstream temp("./data/usuarios_temp.txt");
    if (!temp.is_open()) {
        throw std::runtime_error("Falha ao criar arquivo temporario de usuarios.");
    }

    bool usuarioAtualizado = false;

    if (leitura.is_open()) {
        std::string idLido, loginLido, hashLido, nomeLido, perfilLido;
        double dolarLido, leiteLido, boiLido, vacaLido;
        double pesoEngorda, precoEngorda, pesoAvulsa, precoAvulsa, pesoRecria, precoRecria;

        while (leitura >> idLido >> loginLido >> hashLido >> nomeLido >> perfilLido
                       >> dolarLido >> leiteLido >> boiLido >> vacaLido
                       >> pesoEngorda >> precoEngorda >> pesoAvulsa >> precoAvulsa >> pesoRecria >> precoRecria) {
            if (loginLido == loginFormatado && idLido != idUsuario) {
                leitura.close();
                temp.close();
                std::remove("./data/usuarios_temp.txt");
                throw std::runtime_error("Este login ja esta em uso.");
            }

            if (idLido == idUsuario) {
                temp << idUsuario << " " << loginFormatado << " " << senhaHash << " " << nomeFormatado << " " << perfilFormatado << " "
                     << cotacaoDolar << " " << precoLeite << " " << precoArrobaBoi << " " << precoArrobaVaca << " "
                     << TiposRacoes[0]->peso << " " << TiposRacoes[0]->preco << " "
                     << TiposRacoes[1]->peso << " " << TiposRacoes[1]->preco << " "
                     << TiposRacoes[2]->peso << " " << TiposRacoes[2]->preco << "\n";
                usuarioAtualizado = true;
            } else {
                temp << idLido << " " << loginLido << " " << hashLido << " " << nomeLido << " " << perfilLido << " "
                     << dolarLido << " " << leiteLido << " " << boiLido << " " << vacaLido << " "
                     << pesoEngorda << " " << precoEngorda << " "
                     << pesoAvulsa << " " << precoAvulsa << " "
                     << pesoRecria << " " << precoRecria << "\n";
            }
        }
        leitura.close();
    }

    if (!usuarioAtualizado) {
        temp << idUsuario << " " << loginFormatado << " " << senhaHash << " " << nomeFormatado << " " << perfilFormatado << " "
             << cotacaoDolar << " " << precoLeite << " " << precoArrobaBoi << " " << precoArrobaVaca << " "
             << TiposRacoes[0]->peso << " " << TiposRacoes[0]->preco << " "
             << TiposRacoes[1]->peso << " " << TiposRacoes[1]->preco << " "
             << TiposRacoes[2]->peso << " " << TiposRacoes[2]->preco << "\n";
    }

    temp.close();
    std::remove("./data/usuarios.txt");
    std::rename("./data/usuarios_temp.txt", "./data/usuarios.txt");
}

bool User::autenticar(std::string login, std::string senha) {
    std::ifstream arquivo("./data/usuarios.txt");
    if (!arquivo.is_open()) {
        return false;
    }

    std::string loginFormatado = login.empty() ? "_" : login;
    for (size_t i = 0; i < loginFormatado.length(); i++) {
        if (loginFormatado[i] == ' ') loginFormatado[i] = '_';
    }

    std::string hashDigitado = gerarHash(senha);
    std::string idLido, loginLido, hashLido, nomeLido, perfilLido;
    double dolarLido, leiteLido, boiLido, vacaLido;
    double pesoEngorda, precoEngorda, pesoAvulsa, precoAvulsa, pesoRecria, precoRecria;

    while (arquivo >> idLido >> loginLido >> hashLido >> nomeLido >> perfilLido
                   >> dolarLido >> leiteLido >> boiLido >> vacaLido
                   >> pesoEngorda >> precoEngorda >> pesoAvulsa >> precoAvulsa >> pesoRecria >> precoRecria) {
        if (loginLido == loginFormatado) {
            arquivo.close();
            return hashLido == hashDigitado;
        }
    }

    arquivo.close();
    return false;
}

User::User() {
    TiposRacoes = new SacaRacao*[3];
    for (int i = 0; i < 3; i++) {
        TiposRacoes[i] = new SacaRacao();
    }
    TiposRacoes[0]->name = "Racao Engorda";
    TiposRacoes[1]->name = "Racao Avulsa";
    TiposRacoes[2]->name = "Racao Recria";
}

User* User::carregarUsuario(std::string login, std::string senha) {
    std::ifstream arquivo("./data/usuarios.txt");
    if (!arquivo.is_open()) {
        return nullptr;
    }

    std::string loginFormatado = login.empty() ? "_" : login;
    for (size_t i = 0; i < loginFormatado.length(); i++) {
        if (loginFormatado[i] == ' ') loginFormatado[i] = '_';
    }

    std::string hashDigitado = gerarHash(senha);
    std::string idLido, loginLido, hashLido, nomeLido, perfilLido;
    double dolarLido, leiteLido, boiLido, vacaLido;
    double pesoEngorda, precoEngorda, pesoAvulsa, precoAvulsa, pesoRecria, precoRecria;

    while (arquivo >> idLido >> loginLido >> hashLido >> nomeLido >> perfilLido
                   >> dolarLido >> leiteLido >> boiLido >> vacaLido
                   >> pesoEngorda >> precoEngorda >> pesoAvulsa >> precoAvulsa >> pesoRecria >> precoRecria) {
        if (loginLido == loginFormatado) {
            arquivo.close();
            if (hashLido != hashDigitado) {
                return nullptr;
            }

            if (loginLido == "_") loginLido = "";
            if (nomeLido == "_") nomeLido = "";
            if (perfilLido == "_") perfilLido = "";

            for (size_t i = 0; i < loginLido.length(); i++) {
                if (loginLido[i] == '_') loginLido[i] = ' ';
            }
            for (size_t i = 0; i < nomeLido.length(); i++) {
                if (nomeLido[i] == '_') nomeLido[i] = ' ';
            }
            for (size_t i = 0; i < perfilLido.length(); i++) {
                if (perfilLido[i] == '_') perfilLido[i] = ' ';
            }

            User* usuarioCarregado = new User();
            usuarioCarregado->idUsuario = idLido;
            usuarioCarregado->login = loginLido;
            usuarioCarregado->senhaHash = hashLido;
            usuarioCarregado->nomeProprietario = nomeLido;
            usuarioCarregado->perfilAcesso = perfilLido;
            usuarioCarregado->cotacaoDolar = dolarLido;
            usuarioCarregado->precoLeite = leiteLido;
            usuarioCarregado->precoArrobaBoi = boiLido;
            usuarioCarregado->precoArrobaVaca = vacaLido;

            usuarioCarregado->TiposRacoes[0]->peso = pesoEngorda;
            usuarioCarregado->TiposRacoes[0]->preco = precoEngorda;
            usuarioCarregado->TiposRacoes[1]->peso = pesoAvulsa;
            usuarioCarregado->TiposRacoes[1]->preco = precoAvulsa;
            usuarioCarregado->TiposRacoes[2]->peso = pesoRecria;
            usuarioCarregado->TiposRacoes[2]->preco = precoRecria;

            return usuarioCarregado;
        }
    }

    arquivo.close();
    return nullptr;
}

void User::atualizarCotacaoDolar() {
//garante que vai funcionar no windows tambem
#ifdef _WIN32
    system("curl -s https://economia.awesomeapi.com.br/last/USD-BRL > ./data/cotacao.txt");
#else
    system("curl -s \"https://economia.awesomeapi.com.br/last/USD-BRL\" > ./data/cotacao.txt");
#endif

    std::ifstream arquivo("./data/cotacao.txt");
    if (!arquivo.is_open()) {
        throw std::runtime_error("Falha ao abrir ou criar o arquivo");
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
        std::cout << "\nNao foi possivel atualizar a cotacao do dolar. O valor anterior foi mantido.\n";
    } else {
        std::cout << "\nCotacao do dolar atualizada:\n";
        std::cout << "  Dolar atual: R$ " << cotacaoDolar << std::endl;
    }
};

void User::atualizarCotacaoArroba() {
#ifdef _WIN32
    system("curl -s https://agrodocai.com.br/api/v1/cotacao > ./data/cotacao_boi.txt");
#else
    system("curl -s \"https://agrodocai.com.br/api/v1/cotacao\" > ./data/cotacao_boi.txt");
#endif

    std::ifstream arquivo("./data/cotacao_boi.txt"); 
    if (!arquivo.is_open()) {
        throw std::runtime_error("Falha ao abrir ou criar o arquivo");
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
        std::cout << "\nNao foi possivel atualizar as cotacoes da arroba.\n";
    } else {
        std::cout << "\nCotacoes da arroba atualizadas:\n";
        std::cout << "  Arroba do boi:  R$ " << precoArrobaBoi << std::endl;
        std::cout << "  Arroba da vaca: R$ " << precoArrobaVaca << std::endl;
    }
}

void User::atualizarCotacoes() {
    atualizarCotacaoArroba();
    atualizarCotacaoDolar();

    std::cout << "\nAtualizacao manual:\n";
    std::cout << "  Preco do litro do leite (R$): ";
    std::cin >> precoLeite;

    std::cout << "\nPeso das sacas de racao (kg):\n";
    std::cout << "  Engorda: ";
    std::cin >> TiposRacoes[0]->peso;
    std::cout << "  Avulsa:  ";
    std::cin >> TiposRacoes[1]->peso;
    std::cout << "  Recria:  ";
    std::cin >> TiposRacoes[2]->peso;

    std::cout << "\nPreco das sacas de racao (R$):\n";
    std::cout << "  Engorda: ";
    std::cin >> TiposRacoes[0]->preco;
    std::cout << "  Avulsa:  ";
    std::cin >> TiposRacoes[1]->preco;
    std::cout << "  Recria:  ";
    std::cin >> TiposRacoes[2]->preco;

    salvarEmArquivo();
    std::cout << "\nCotacoes atualizadas e salvas com sucesso!\n";
}

//getters
std::string User::getNomeProprietario()const{return nomeProprietario;}
double User::getPrecoLeite()         { return precoLeite; }
double User::getCotacaoDolar()       { return cotacaoDolar; }
double User::getCotacaoArrobaBoi()   { return precoArrobaBoi; }
double User::getCotacaoArrobaVaca()  { return precoArrobaVaca; }

// Preço das sacas de ração
double User::getSacaRacaoEngorda()   { return TiposRacoes[0]->preco; }
double User::getSacaRacaoAvulsa()    { return TiposRacoes[1]->preco; }
double User::getSacaRacaoRecria()    { return TiposRacoes[2]->preco; }

// Peso (kg) das sacas de ração
double User::getKgRacaoEngorda()     { return TiposRacoes[0]->peso; }
double User::getKgRacaoAvulsa()      { return TiposRacoes[1]->peso; }
double User::getKgRacaoRecria()      { return TiposRacoes[2]->peso; }

SacaRacao** User::getRelatorioRacao() { return TiposRacoes; }

std::string User::getId() { return idUsuario; }

//setters
// Mercado e produtos
void User::setPrecoLeite(double p)              { precoLeite = p; }
void User::setCotacaoDolar(double p)            { cotacaoDolar = p; }
void User::setPrecoArrobaBoi(double p)          { precoArrobaBoi = p; }
void User::setPrecoArrobaVaca(double p)         { precoArrobaVaca = p; }

// Preço das sacas de ração
void User::setPrecoSacaRacaoEngorda(double p)   { TiposRacoes[0]->preco = p; }
void User::setPrecoSacaRacaoAvulsa(double p)    { TiposRacoes[1]->preco = p; }
void User::setPrecoSacaRacaoRecria(double p)    { TiposRacoes[2]->preco = p; }

// Peso (kg) das sacas de ração
void User::setKgRacaoEngorda(double p)          { TiposRacoes[0]->peso = p; }
void User::setKgRacaoAvulsa(double p)           { TiposRacoes[1]->peso = p; }
void User::setKgRacaoRecria(double p)           { TiposRacoes[2]->peso = p; }