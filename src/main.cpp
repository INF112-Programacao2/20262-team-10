#include <iostream>
#include <string>

#include "user.hpp"
#include "farm.hpp"
#include "animal.hpp"
#include "bezerro.hpp"
#include "parserplanilha.hpp"

int main () {

    User* usuarioTeste = new User("paulo_admin", "senha123", "Paulo", "Proprietario");
    Farm fazendinha("fazenda joao rock", "vicosa" , usuarioTeste);
    Area area1("area1", "bom", 12.50);
    Bezerro* b1 = new Bezerro("141", 'M', 100, 13, 0, 0, 1, 1.0, 1, false);
    area1.adicionarAnimal(b1);
    //area1.promoverAnimal(); nao usar ainda
    area1.removerAnimal(b1->getId());
    std :: cout << area1.getQtdAnimais() << std :: endl;
    ParserPlanilha::importarCSV("/docs/fazenda.csv", fazendinha);
    usuarioTeste->atualizarCotacoes();
    delete usuarioTeste;
    return 0;
};