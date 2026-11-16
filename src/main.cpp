#include <iostream>
#include <string>

#include "user.hpp"
#include "farm.hpp"
#include "animal.hpp"
#include "bezerro.hpp"

int main () {

    User* usuarioTeste = new User("paulo_admin", "senha123", "Paulo", "Proprietario");

    std::cout << "--- Tabela de Precos Locais ---" << std::endl;
    usuarioTeste->atualizarCotacaoDolar();
    usuarioTeste->atualizarCotacaoBoi();
    std::cout << "o teste da cotacao retornou:  " << usuarioTeste->getCotacaoDolar() << std::endl;
    // std::cout << "Preco do Leite: R$ " << usuarioTeste.getPrecoLeite() << std::endl;
    //std::cout << "Preco da Arroba: R$ " << usuarioTeste.getPrecoArroba() << std::endl;
    Farm fazendinha("fazenda joao rock", "vicosa" , usuarioTeste);
    Area area1("area1", "bom", 12.50);
    Bezerro* b1 = new Bezerro("141", 'M', 100, 13, 0, 0, 1, 1.0, 1, false);
    area1.adicionarAnimal(b1);
    //area1.promoverAnimal(); nao usar ainda
    area1.removerAnimal(b1->getId());
    std::cout << "o teste da cotacao do arroba do boi retornou: " << usuarioTeste->getCotacaoArrobaBoi() << " e o da vaca: "<< usuarioTeste->getCotacaoArrobaVaca() << std::endl;
    std :: cout << area1.getQtdAnimais() << std :: endl;

    delete usuarioTeste;
    return 0;
};