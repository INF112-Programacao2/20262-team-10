#include <iostream>
#include "area.hpp"
#include "farm.hpp"
#include "animal.hpp"
#include "touro.hpp"
#include "user.hpp"

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

        if (qtdAtualDeAnimais >= capacidadeMaxima) {
            std::cout << "Atencao, verifique a lotacao atual do pasto, ela e atualmente de: " << calcularLotacaoArea() << "%" << std::endl;
        }
};

void Area::removerAnimal(string idAnimal) {
    Animal* animalParaAbate = extrairAnimal(idAnimal);

    if (animalParaAbate == nullptr) {
        std::cout << "Erro: Animal nao encontrado no rebanho." << std::endl;
        return;
    }

    delete animalParaAbate; 
    std::cout << "Animal removido e deletado com sucesso!" << std::endl;
}

std::string Area::getNomeArea() {
    return nome;
}

int Area::getQtdAnimais() {
    return qtdAtualDeAnimais;
}

void Area::promoverAnimal() { //incluir conexão com a função recomendar destino(Touro/corte)
    if (rebanho == nullptr) throw std::runtime_error("Rebanho da area nao foi alocado!");

    for(int i=0; i< qtdAtualDeAnimais; i++){
        if(rebanho[i] == nullptr)
            continue;
        if(rebanho[i]->podeSerPromovido() && rebanho[i]->getTipo() == "bezerro"){
            std :: cout << "O animal já esta na idade apropriada para promoçãoz\n";
            std :: cout << "Gostaria de realizar a promoção[Y/N]?";
            unsigned char resposta; 
            std :: cin >> resposta;
            if(resposta=='Y' || resposta=='y'){
                if(rebanho[i]->getSexo() == 'M'){
                    Touro* t = new Touro(rebanho[i]->getId(), rebanho[i]->getSexo(),
                    rebanho[i]->getPeso(), rebanho[i]->getIdadeMeses(), rebanho[i]->getQtdFilhos(), rebanho[i]->getQtdvacinas(), 0);
                    delete rebanho[i];
                    rebanho[i] = t;
                    std :: cout << "Bezerro promovido a touro com sucesso!\n";
                }
                if(rebanho[i]->getSexo() == 'F'){
                    Vaca* v = new Vaca(rebanho[i]->getId(), rebanho[i]->getSexo(),
                    rebanho[i]->getPeso(), rebanho[i]->getIdadeMeses(), rebanho[i]->getQtdFilhos(), rebanho[i]->getQtdvacinas());
                    delete rebanho[i];
                    rebanho[i] = v;
                    std :: cout << "Bezerro promovido a Vaca com sucesso!\n";
                }
            }
        }
    }
}

Animal* Area::extrairAnimal(string idAnimal) {
    int indexAnimalRemovido = -1;
    Animal* animalExtraido = nullptr;

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i]->getId() == idAnimal) {
            indexAnimalRemovido = i;
            animalExtraido = rebanho[i];
            break;
        }
    }

    if (indexAnimalRemovido == -1) {
        return nullptr;
    }

    Animal** rebanhoNovaAlocacao = new Animal*[qtdAtualDeAnimais - 1];

    for (int i = 0; i < indexAnimalRemovido; i++) {
        rebanhoNovaAlocacao[i] = rebanho[i];
    }
    for (int i = indexAnimalRemovido + 1; i < qtdAtualDeAnimais; i++) {
        rebanhoNovaAlocacao[i - 1] = rebanho[i];
    }

    delete[] rebanho;
    rebanho = rebanhoNovaAlocacao;
    qtdAtualDeAnimais--;
    
    return animalExtraido; 
}

double Area::calcularLotacaoArea() {
   return qtdAtualDeAnimais*100/capacidadeMaxima;
}

int Area::getCapacidadeMaxima() {
    return capacidadeMaxima;
}

Animal* Area::buscarAnimal(string id) {
    bool achou = false;
    Animal* temporario = nullptr;
    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i]->getId() == id) {
            achou = true;
            temporario = rebanho[i];
            break;
        }
    }
    if (achou) {
        std::cout << "animal encontrado com sucesso" << std::endl;
        return temporario;
    } else {
        return nullptr;
    }
}

double Area::getGastoIndividual(string id) {
    Animal* animal = buscarAnimal(id);
}
