#include <iostream>
#include <string>
#include "user.hpp"

void exibirDadosUsuario(User* u) {
    std::cout << "\nCotacoes atuais:\n";
    std::cout << "  Dolar:          R$ " << u->getCotacaoDolar() << std::endl;
    std::cout << "  Litro do leite: R$ " << u->getPrecoLeite() << std::endl;
    std::cout << "  Arroba do boi:  R$ " << u->getCotacaoArrobaBoi() << std::endl;
    std::cout << "  Arroba da vaca: R$ " << u->getCotacaoArrobaVaca() << std::endl;

    std::cout << "\nSacas de racao:\n";
    std::cout << "  Engorda: " << u->getKgRacaoEngorda() << " kg - R$ " << u->getSacaRacaoEngorda() << std::endl;
    std::cout << "  Avulsa:  " << u->getKgRacaoAvulsa()  << " kg - R$ " << u->getSacaRacaoAvulsa()  << std::endl;
    std::cout << "  Recria:  " << u->getKgRacaoRecria()  << " kg - R$ " << u->getSacaRacaoRecria()  << std::endl;
}

int main() {
    User* usuarioAtual = nullptr;
    int opcao = -1;

    while (opcao != 0 && usuarioAtual == nullptr) {
        std::cout << "\nAcesso ao sistema:\n";
        std::cout << "  1 - Cadastrar novo usuario\n";
        std::cout << "  2 - Entrar na conta\n";
        std::cout << "  0 - Sair\n";
        std::cout << "\nOpcao: ";
        std::cin >> opcao;

        if (opcao == 1) {
            std::cin.ignore();
            std::string login, senha, nome, perfil;

            std::cout << "\nCadastro de usuario:\n";
            std::cout << "  Login: ";
            std::getline(std::cin, login);
            std::cout << "  Senha: ";
            std::getline(std::cin, senha);
            std::cout << "  Nome do proprietario: ";
            std::getline(std::cin, nome);
            std::cout << "  Perfil de acesso: ";
            std::getline(std::cin, perfil);

            try {
                User* novo = new User(login, senha, nome, perfil);
                novo->salvarEmArquivo();
                std::cout << "\nUsuario cadastrado e salvo com sucesso!\n";
                delete novo;
            } catch (std::runtime_error& e) {
                std::cout << "\nErro ao cadastrar: " << e.what() << std::endl;
            }
        } else if (opcao == 2) {
            std::cin.ignore();
            std::string login, senha;

            std::cout << "\nLogin:\n";
            std::cout << "  Usuario: ";
            std::getline(std::cin, login);
            std::cout << "  Senha: ";
            std::getline(std::cin, senha);

            usuarioAtual = User::carregarUsuario(login, senha);
            if (usuarioAtual == nullptr) {
                std::cout << "\nUsuario ou senha invalidos. Tente novamente.\n";
            } else {
                std::cout << "\nAcesso liberado! Bem-vindo ao sistema.\n";
            }
        } else if (opcao != 0) {
            std::cout << "\nOpcao invalida.\n";
        }
    }

    while (usuarioAtual != nullptr && opcao != 0) {
        std::cout << "\nMenu principal:\n";
        std::cout << "  1 - Consultar cotacoes e racoes\n";
        std::cout << "  2 - Alterar algum valor manualmente\n";
        std::cout << "  3 - Atualizar todas as cotacoes (API + manual)\n";
        std::cout << "  0 - Salvar e encerrar\n";
        std::cout << "\nOpcao: ";
        std::cin >> opcao;

        try {
            if (opcao == 1) {
                exibirDadosUsuario(usuarioAtual);
            } else if (opcao == 2) {
                int subOpcao = -1;
                double valor = 0;

                std::cout << "\nQual valor voce deseja alterar?\n";
                std::cout << "  1  - Cotacao do dolar\n";
                std::cout << "  2  - Preco do litro do leite\n";
                std::cout << "  3  - Preco da arroba do boi\n";
                std::cout << "  4  - Preco da arroba da vaca\n";
                std::cout << "  5  - Preco da saca (Racao Engorda)\n";
                std::cout << "  6  - Preco da saca (Racao Avulsa)\n";
                std::cout << "  7  - Preco da saca (Racao Recria)\n";
                std::cout << "  8  - Peso da saca em kg (Racao Engorda)\n";
                std::cout << "  9  - Peso da saca em kg (Racao Avulsa)\n";
                std::cout << "  10 - Peso da saca em kg (Racao Recria)\n";
                std::cout << "  0  - Voltar ao menu\n";
                std::cout << "\nOpcao: ";
                std::cin >> subOpcao;

                if (subOpcao >= 1 && subOpcao <= 10) {
                    std::cout << "  Digite o novo valor: ";
                    std::cin >> valor;

                    if (subOpcao == 1) usuarioAtual->setCotacaoDolar(valor);
                    else if (subOpcao == 2) usuarioAtual->setPrecoLeite(valor);
                    else if (subOpcao == 3) usuarioAtual->setPrecoArrobaBoi(valor);
                    else if (subOpcao == 4) usuarioAtual->setPrecoArrobaVaca(valor);
                    else if (subOpcao == 5) usuarioAtual->setPrecoSacaRacaoEngorda(valor);
                    else if (subOpcao == 6) usuarioAtual->setPrecoSacaRacaoAvulsa(valor);
                    else if (subOpcao == 7) usuarioAtual->setPrecoSacaRacaoRecria(valor);
                    else if (subOpcao == 8) usuarioAtual->setKgRacaoEngorda(valor);
                    else if (subOpcao == 9) usuarioAtual->setKgRacaoAvulsa(valor);
                    else if (subOpcao == 10) usuarioAtual->setKgRacaoRecria(valor);

                    usuarioAtual->salvarEmArquivo();
                    std::cout << "\nAlteracao salva com sucesso!\n";
                } else if (subOpcao != 0) {
                    std::cout << "\nOpcao invalida.\n";
                }
            } else if (opcao == 3) {
                std::cout << std::endl;
                usuarioAtual->atualizarCotacoes();
            } else if (opcao == 0) {
                usuarioAtual->salvarEmArquivo();
                std::cout << "\nDados salvos. Encerrando o programa...\n";
            } else {
                std::cout << "\nOpcao invalida.\n";
            }
        } catch (std::runtime_error& e) {
            std::cout << "\nErro: " << e.what() << std::endl;
        }
    }

    delete usuarioAtual;
    return 0;
}