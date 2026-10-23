#include <iostream>
#include "area.hpp"
#include "animal.hpp"

Area::Area(std::string n, std::string estado, double a) {
    nome = n;
    estadoPasto = estado;
    area = a;
    qtdAtualDeAnimais = 0;
    //aqui separa o primeiro array de 1 posicao
    rebanho = new Animal*[qtdAtualDeAnimais+1];
    //sistema adequado de no max ate 3 cabecas por hec
    //tem que fazer a logica if else para cada qualidade do pasto
    capacidadeMaxima = area*3;
}

void Area::adicionarAnimal(Animal* a) {
    if (qtdAtualDeAnimais < capacidadeMaxima) {
        //alocacao dinamica do novo rebanho
        Animal** rebanhoNovaAlocacao = new Animal*[qtdAtualDeAnimais+1];

        //copiando enderecos dos animais para o novo rebanho
        for (int i = 0; i < qtdAtualDeAnimais; i++) {
            rebanhoNovaAlocacao[i] = rebanho[i];
        }

        //coloca o animal na ultima posicao de index
        rebanhoNovaAlocacao[qtdAtualDeAnimais] = a;
        qtdAtualDeAnimais++;

        //deleta os enderecos antigos que faltavam o ultimo que rebanho aponta
        delete[] rebanho;

        //agora rebanho aponta exatamente para os enderecos de todos os animais, incluindo o inserido;
        rebanho = rebanhoNovaAlocacao;

        std::cout << "animal de indice: --" << a->getId()  << "-- foi aidicionado com sucesso!"
        << std::endl;
        std::cout << "A quantidade atual de animais e de: " << qtdAtualDeAnimais << std::endl;
    } else {
        std::cout << "Na sua fazenda nao cabem mais animais para o atual padrao mantido" << std::endl;
    }
};

void Area::removerAnimal(Animal* a) {
    int indexAnimalRemovido = -1;

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i]->getId() == a->getId()) {
            indexAnimalRemovido = i;
            break;
        }
    }

    if (indexAnimalRemovido == -1) {
        std::cout << "Erro: Animal nao encontrado no rebanho." << std::endl;
        return;
    }

    Animal** rebanhoNovaAlocacao = new Animal*[qtdAtualDeAnimais-1];

    for (int i = 0; i < indexAnimalRemovido; i++) {
        rebanhoNovaAlocacao[i] = rebanho[i];
    }

    for (int i = indexAnimalRemovido + 1; i < qtdAtualDeAnimais; i++) {
        rebanhoNovaAlocacao[i - 1] = rebanho[i];
    }

    delete rebanho[indexAnimalRemovido];
    delete[] rebanho;

    rebanho = rebanhoNovaAlocacao;

    qtdAtualDeAnimais--;
    std::cout << "Animal removido com sucesso!" << std::endl;
}

std::string Area::getNomeArea() {
    return nome;
}

int Area::getQtdAnimais() {
    return qtdAtualDeAnimais;
}