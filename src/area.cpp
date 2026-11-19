#include <iostream>
#include <stdexcept>
#include "area.hpp"
#include "farm.hpp"
#include "animal.hpp"
#include "touro.hpp"
#include "bezerro.hpp"
#include "vaca.hpp"
#include "user.hpp"
#include "sacaRacao.hpp"
#include "snapshotancestral.hpp"

Area::Area(std::string n, std::string estado, double a) {
    nome = n;
    estadoPasto = estado;
    area = a;
    qtdAtualDeAnimais = 0;
    rebanho = new Animal*[1];

    double cabecasPorHectare = 2.0; // padrao intermediario
    if (estadoPasto == "seco") cabecasPorHectare = 0.5;
    else if (estadoPasto == "ruim") cabecasPorHectare = 1.0;
    else if (estadoPasto == "intermediario") cabecasPorHectare = 2.0;
    else if (estadoPasto == "bom") cabecasPorHectare = 3.0;
    else if (estadoPasto == "muito bom") cabecasPorHectare = 4.0;

    capacidadeMaxima = static_cast<int>(area * cabecasPorHectare);
    if (capacidadeMaxima < 1) {
        capacidadeMaxima = 1;
    }
}

std::string Area::getEstadoPasto() {
    return estadoPasto;
}

double Area::calcularLotacaoArea() {
    if (capacidadeMaxima <= 0) {
        return 0.0;
    }
    return (qtdAtualDeAnimais * 100.0) / capacidadeMaxima;
}

Area::~Area() {
    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        delete rebanho[i];
    }
    delete[] rebanho;
}

void Area::adicionarAnimal(Animal* a) {
    if (a == nullptr) {
        throw std::runtime_error("Nao e possivel adicionar um animal nulo.");
    }

    if (buscarAnimal(a->getId()) != nullptr) {
        throw std::runtime_error("Ja existe um animal com este ID nesta area.");
    }

    Animal** rebanhoNovaAlocacao = new Animal*[qtdAtualDeAnimais + 1];

    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        rebanhoNovaAlocacao[i] = rebanho[i];
    }

    rebanhoNovaAlocacao[qtdAtualDeAnimais] = a;
    qtdAtualDeAnimais++;

    delete[] rebanho;
    rebanho = rebanhoNovaAlocacao;

    std::cout << "Animal de indice --" << a->getId() << "-- foi adicionado com sucesso!" << std::endl;
    std::cout << "A quantidade atual de animais e de: " << qtdAtualDeAnimais << std::endl;

    if (qtdAtualDeAnimais >= capacidadeMaxima) {
        std::cout << "Atencao, verifique a lotacao atual do pasto, ela e atualmente de: " << calcularLotacaoArea() << "%" << std::endl;
    }
}

void Area::removerAnimal(string idAnimal) {
    Animal* animalParaAbate = extrairAnimal(idAnimal);

    if (animalParaAbate == nullptr) {
        std::cout << "Erro: Animal nao encontrado no rebanho." << std::endl;
        return;
    }

    //IMPREMENTAR SNAPSHOT AQUI

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
                    rebanho[i]->getPeso(), rebanho[i]->getIdadeMeses(), 0, rebanho[i]->getQtdvacinas(), nullptr, 0.0);
                    t->setIdPai(rebanho[i]->getPai());
                    t->setIdMae(rebanho[i]->getMae());
                    delete rebanho[i];
                    rebanho[i] = t;
                    std :: cout << "Bezerro promovido a touro com sucesso!\n";
                }
                if(rebanho[i]->getSexo() == 'F'){
                    Vaca* v = new Vaca(rebanho[i]->getId(), rebanho[i]->getSexo(),
                    rebanho[i]->getPeso(), rebanho[i]->getIdadeMeses(), 0, rebanho[i]->getQtdvacinas(), nullptr);
                    v->setIdPai(rebanho[i]->getPai());
                    v->setIdMae(rebanho[i]->getMae());
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
   return qtdAtualDeAnimais*100.0/capacidadeMaxima;
}

int Area::getCapacidadeMaxima() {
    return capacidadeMaxima;
}

Animal* Area::buscarAnimal(std::string id) {
    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] != nullptr && rebanho[i]->getId() == id) {
            return rebanho[i];
        }
    }
    return nullptr;
}

double Area::getGastoIndividual (string id, double precoLeite, SacaRacao** relatorioRacoes) {
    Animal* animalAchado = buscarAnimal(id);
    if (animalAchado == nullptr) throw std::runtime_error("Esse animal nao foi encontrado");
    if (calcularLotacaoArea() > 100) 
        return (animalAchado->calcularGastoDiario(precoLeite, relatorioRacoes))*(calcularLotacaoArea()/100);
    else 
        return (animalAchado->calcularGastoDiario(precoLeite, relatorioRacoes));
}

double Area::getGastoDaArea(double precoLeite, SacaRacao** relatorioRacoes) {
    double total = 0.0;
    for (int i = 0; i < qtdAtualDeAnimais; i++) {
        if (rebanho[i] != nullptr) {
            total += rebanho[i]->calcularGastoDiario(precoLeite, relatorioRacoes);
        }
    }
    return total;
}