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
    senhaHash = gerarHash(senha);
    idUsuario = nom;
    perfilAcesso = perfil;
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

void User::salvarEmArquivo() {
    std::ofstream arquivo("./data/usuarios.txt", std::ios::app);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Falha ao abrir ou criar o arquivo");
    }
    arquivo << login << " " << senhaHash << " " << idUsuario << " " << perfilAcesso << "\n";
    arquivo.close();
}

std::string User::gerarHash(const std::string& senha) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(senha));
}

bool User::autenticar(std::string login, std::string senha) {
    std::ifstream arquivo("./data/usuarios.txt");
    bool achouUsuario = false;
    std::string hash_digitado;
    if(!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel encontrar um arquivo com dados usuario");
    }
    std::string login_arquivo, hash_arquivo, id_arquivo, perfil_arquivo;
    while (arquivo >> login_arquivo >> hash_arquivo >> id_arquivo >> perfil_arquivo) {
        if (login_arquivo == login) {
            hash_digitado = gerarHash(senha);
            achouUsuario = true;
            break;
        }
    }
    arquivo.close();
    if (!achouUsuario) throw std::runtime_error("Usuario ou senha invalidos"); else return hash_arquivo == hash_digitado;
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
        std::cout << std::endl; std::cout << "---------------------" << std::endl;
        std::cout << "Atencao valor da cotacao apresenta erro e nao foi modificado" << std::endl;
    } else {
        std::cout << "cotacao do dolar atualizada com sucesso" << std::endl;
        std::cout << "a nova cotacao retornou:  " << cotacaoDolar << std::endl;
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
        std::cout << std::endl << "----------------------" << std::endl;
        std::cout << "Atencao: Um ou mais valores de cotacao apresentam erro ou nao foram encontrados." << std::endl;
    } else {
        std::cout << "Cotacoes de boi e vaca atualizadas com sucesso!" << std::endl;
          std::cout << "o teste da cotacao do arroba do boi retornou: " << precoArrobaBoi << " e o da vaca: "<< precoArrobaVaca << std::endl;
    }
}

void User::atualizarCotacoes() {
    atualizarCotacaoArroba();
    atualizarCotacaoDolar();
    std::cout << "Qual o preco que voce vende o litro do leite: ";
    std::cin >> precoLeite;
    std::cout << "Litro do leite atualizado com sucesso para: " << precoLeite << std::endl;
    std::cout << "Voce comprou sacos de racao de quantos kg? Preencha na ordem -> Engorda - Avulsa - Recriacao ";
    std::cin >> TiposRacoes[0]->peso >> TiposRacoes[1]->peso >> TiposRacoes[2]->peso;
    std::cout << "Qual o preco de cada saca de racao? Preencha na ordem -> Engorda - Avulsa - Recriacao: ";
    std::cin >> TiposRacoes[0]->preco >> TiposRacoes[1]->preco >> TiposRacoes[2]->preco;
}

double User::getCotacaoDolar() {
    return cotacaoDolar;
};

double User::getCotacaoArrobaBoi() {
    return precoArrobaBoi;
}

double User::getPrecoLeite() {
    return precoLeite;
}

double User::getKgRacaoAvulsa() {
    return TiposRacoes[1]->peso;
}

double User::getKgRacaoEngorda() {
    return TiposRacoes[0]->peso;
}

double User::getKgRacaoRecria() {
    return TiposRacoes[2]->peso;
}

double User::getSacaRacaoAvulsa() {
    return TiposRacoes[1]->preco;
}

double User::getSacaRacaoEngorda() {
    return TiposRacoes[0]->preco;
}

double User::getSacaRacaoRecria() {
    return TiposRacoes[2]->preco;
}

double User::getCotacaoArrobaVaca() {
    return precoArrobaVaca;
}

void User::setPrecoLeite (double p) {
    precoLeite = p;
};

void User::setPrecoArrobaBoi(double p) {
    precoArrobaBoi = p;
};

void User::setPrecoArrobaVaca(double p) {
    precoArrobaVaca = p;
};

void User::setCotacaoDolar(double p) {
    cotacaoDolar = p;
};

void User::setPrecoSacaRacaoAvulsa(double p) {
    TiposRacoes[1]->preco = p;
};

void User::setPrecoSacaRacaoEngorda(double p) {
    TiposRacoes[0]->preco = p;
};

void User::setPrecoSacaRacaoRecria(double p) {
    TiposRacoes[2]->preco = p;
};

void User::setKgRacaoAvulsa(double p) {
    TiposRacoes[1]->peso = p;
};

void User::setKgRacaoEngorda(double p) {
    TiposRacoes[0]->peso = p;
};

void User::setKgRacaoRecria(double p) {
    TiposRacoes[2]->peso = p;
};

SacaRacao** User::getRelatorioRacao() {
    return TiposRacoes;
}