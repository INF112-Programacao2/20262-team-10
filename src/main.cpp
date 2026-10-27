#include <iostream>
#include <string>

#include "user.hpp"
#include "farm.hpp"

int main () {

User usuarioTeste("paulo_admin", "senha123", "Paulo", "Proprietario");

    std::cout << "--- Tabela de Precos Locais ---" << std::endl;
    std::cout << "Preco do Leite: R$ " << usuarioTeste.getPrecoLeite() << std::endl;
    std::cout << "Preco da Arroba: R$ " << usuarioTeste.getPrecoArroba() << std::endl;

    return 0;
};