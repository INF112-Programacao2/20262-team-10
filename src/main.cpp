#include <iostream>
#include <string>
#include "user.hpp"
#include "vaca.hpp"
#include "touro.hpp"
#include "bezerro.hpp"
#include "boicorte.hpp"

void exibirDadosUsuario(User* u) {
    std::cout << "\nCotacao Dolar: R$ " << u->getCotacaoDolar() << std::endl;
    std::cout << "Preco Leite (L): R$ " << u->getPrecoLeite() << std::endl;
    std::cout << "Arroba Boi: R$ " << u->getCotacaoArrobaBoi() << std::endl;
    std::cout << "Arroba Vaca: R$ " << u->getCotacaoArrobaVaca() << std::endl;
    std::cout << "Racao Engorda: " << u->getKgRacaoEngorda() << " kg | R$ " << u->getSacaRacaoEngorda() << std::endl;
    std::cout << "Racao Avulsa: " << u->getKgRacaoAvulsa() << " kg | R$ " << u->getSacaRacaoAvulsa() << std::endl;
    std::cout << "Racao Recria: " << u->getKgRacaoRecria() << " kg | R$ " << u->getSacaRacaoRecria() << std::endl;
}

int main() {
    User* usuarioAtual = nullptr;
    int opcao = -1;

    while (opcao != 0 && usuarioAtual == nullptr) {
        std::cout << "\n1 - Cadastrar usuario" << std::endl;
        std::cout << "2 - Fazer login" << std::endl;
        std::cout << "0 - Sair" << std::endl;
        std::cout << "Opcao: ";
        std::cin >> opcao;
        std::cin.ignore();

        if (opcao == 1) {
            std::string login, senha, nome, perfil;
            std::cout << "Login: ";
            std::getline(std::cin, login);
            std::cout << "Senha: ";
            std::getline(std::cin, senha);
            std::cout << "Nome do proprietario: ";
            std::getline(std::cin, nome);
            std::cout << "Perfil de acesso: ";
            std::getline(std::cin, perfil);

            try {
                User* novo = new User(login, senha, nome, perfil);
                novo->salvarEmArquivo();
                std::cout << "Usuario cadastrado com sucesso!" << std::endl;
                delete novo;
            } catch (std::runtime_error& e) {
                std::cout << "Erro: " << e.what() << std::endl;
            }
        } else if (opcao == 2) {
            std::string login, senha;
            std::cout << "Login: ";
            std::getline(std::cin, login);
            std::cout << "Senha: ";
            std::getline(std::cin, senha);

            usuarioAtual = User::carregarUsuario(login, senha);
            if (usuarioAtual == nullptr) {
                std::cout << "Login ou senha invalidos." << std::endl;
            } else {
                std::cout << "Login realizado!" << std::endl;
            }
        }
    }

    while (usuarioAtual != nullptr && opcao != 0) {
        std::cout << "\n1 - Ver cotacoes e racoes" << std::endl;
        std::cout << "2 - Alterar preco do leite e racao" << std::endl;
        std::cout << "3 - Atualizar cotacoes" << std::endl;
        std::cout << "4 - Cadastrar e salvar animal (teste)" << std::endl;
        std::cout << "5 - Carregar animal por ID" << std::endl;
        std::cout << "0 - Salvar e sair" << std::endl;
        std::cout << "Opcao: ";
        std::cin >> opcao;

        try {
            if (opcao == 1) {
                exibirDadosUsuario(usuarioAtual);
            } else if (opcao == 2) {
                double novoLeite, novaRacaoEngorda;
                std::cout << "Novo preco do leite: ";
                std::cin >> novoLeite;
                std::cout << "Novo preco da saca de Racao Engorda: ";
                std::cin >> novaRacaoEngorda;

                usuarioAtual->setPrecoLeite(novoLeite);
                usuarioAtual->setPrecoSacaRacaoEngorda(novaRacaoEngorda);
                usuarioAtual->salvarEmArquivo();
                std::cout << "Dados atualizados!" << std::endl;
            } else if (opcao == 3) {
                usuarioAtual->atualizarCotacoes();
            } else if (opcao == 4) {
                int tipo;
                std::string id;
                char sexo;
                double peso;

                std::cout << "Tipo (1-Bezerro, 2-Vaca, 3-Touro, 4-Boicorte): ";
                std::cin >> tipo;
                std::cout << "ID: ";
                std::cin >> id;
                std::cout << "Sexo (M/F): ";
                std::cin >> sexo;
                std::cout << "Peso: ";
                std::cin >> peso;

                if (tipo == 1) {
                    Bezerro* b = new Bezerro(id, sexo, peso);
                    b->salvarEmArquivo();
                    delete b;
                } else if (tipo == 2) {
                    Vaca* v = new Vaca(id, sexo, peso, 24, 0, 0);
                    v->salvarEmArquivo();
                    delete v;
                } else if (tipo == 3) {
                    Touro* t = new Touro(id, sexo, peso, 36, 0, 0, nullptr, 50.0);
                    t->salvarEmArquivo();
                    delete t;
                } else if (tipo == 4) {
                    Boicorte* bc = new Boicorte(id, sexo, peso, 20, 0, 0);
                    bc->salvarEmArquivo();
                    delete bc;
                }
                std::cout << "Animal salvo com sucesso!" << std::endl;
            } else if (opcao == 5) {
                int tipo;
                std::string id;

                std::cout << "Tipo (1-Bezerro, 2-Vaca, 3-Touro, 4-Boicorte): ";
                std::cin >> tipo;
                std::cout << "ID do animal: ";
                std::cin >> id;

                Animal* carregado = nullptr;
                if (tipo == 1) carregado = Bezerro::carregarBezerro(id);
                else if (tipo == 2) carregado = Vaca::carregarVaca(id);
                else if (tipo == 3) carregado = Touro::carregarTouro(id);
                else if (tipo == 4) carregado = Boicorte::carregarBoicorte(id);

                if (carregado == nullptr) {
                    std::cout << "Animal nao encontrado." << std::endl;
                } else {
                    std::cout << "Animal carregado: " << carregado->getTipo()
                              << " | ID: " << carregado->getId()
                              << " | Sexo: " << carregado->getSexo()
                              << " | Peso: " << carregado->getPeso() << " kg" << std::endl;
                    delete carregado;
                }
            } else if (opcao == 0) {
                usuarioAtual->salvarEmArquivo();
                std::cout << "Saindo..." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Erro: " << e.what() << std::endl;
        }
    }

    delete usuarioAtual;
    return 0;
}