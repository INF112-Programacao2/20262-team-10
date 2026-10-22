#include "farm.hpp"
#include "animal.hpp"

Farm::Farm(std::string s, std::string l, double a) {
    nome = s;
    localizacao = l;
    area = a;
    qtdAtualDeAnimais = 0;
    rebanho = new Animal*[qtdAtualDeAnimais+1];
    //sistema adequado de no max ate 3 cabecas por hec
    capacidadeMaxima = area/3 - qtdAtualDeAnimais;
}


int Farm::getQuantidadeAnimais() {
    return qtdAtualDeAnimais;
}

void Farm::adicionarAnimal(Animal* a) {
    if (qtdAtualDeAnimais < capacidadeMaxima) {
        rebanho[qtdAtualDeAnimais] = a;
        qtdAtualDeAnimais++;
        std::cout << "animal de indice: " << "valor do ID aqui " << "foi aidicionado com sucesso!"
        << std::endl;
        std::cout << "A quantidade atual de animais e de: " << qtdAtualDeAnimais << std::endl;
    } else {
        std::cout << "Na sua fazenda nao cabem mais animais para o atual padrao mantido" << std::endl;
    }
};