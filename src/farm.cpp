#include "farm.hpp"
#include "animal.hpp"

Farm::Farm(std::string s, std::string l, double a) {
    nome = s;
    localizacao = l;
    area = a;
    qtdAtualDeAnimais = 0;
    //aqui separa o primeiro array de 1 posicao
    rebanho = new Animal*[qtdAtualDeAnimais+1];
    //sistema adequado de no max ate 3 cabecas por hec
    capacidadeMaxima = area*3;
}


int Farm::getQuantidadeAnimais() {
    return qtdAtualDeAnimais;
}

void Farm::adicionarAnimal(Animal* a) {
    if (qtdAtualDeAnimais < capacidadeMaxima) {
        //alocacao dinamica do novo rebanho
        Animal** rebanhoNovaAlocacao = new Animal*[qtdAtualDeAnimais+1];

        //copiando enderecos dos animais para o novo rebanho
        for (int i = 0; i < qtdAtualDeAnimais; i++) {
            rebanhoNovaAlocacao[i] = rebanho[i];
        }

        //coloca o animal na ultima posicao de index
        rebanhoNovaAlocacao[qtdAtualDeAnimais] = a;

        delete[] rebanho;

        rebanho = rebanhoNovaAlocacao;

        std::cout << "animal de indice: --" << a->getId()  << "-- foi aidicionado com sucesso!"
        << std::endl;
        std::cout << "A quantidade atual de animais e de: " << qtdAtualDeAnimais << std::endl;
    } else {
        std::cout << "Na sua fazenda nao cabem mais animais para o atual padrao mantido" << std::endl;
    }
};