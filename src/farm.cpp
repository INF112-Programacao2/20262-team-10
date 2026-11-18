#include "farm.hpp"
#include "animal.hpp"

#include <iostream>

Farm::Farm(std::string s, std::string l, User* propr) {
    nome = s;
    localizacao = l;
    qtdAreas = 0;
    proprietario = propr;
    area = nullptr;
    areaAtual = "";
}

Farm::~Farm() {}

void Farm::adicionarArea(Area* a) {
        Area** areaNovaAlocacao = new Area*[qtdAreas+1];

        //copiando enderecos dos animais para o novo rebanho
        for (int i = 0; i < qtdAreas; i++) {
            areaNovaAlocacao[i] = area[i];

        }
        //coloca o animal na ultima posicao de index
        areaNovaAlocacao[qtdAreas] = a;
        qtdAreas++;

        //deleta os enderecos antigos que faltavam o ultimo que rebanho aponta
        delete[] area;

        //agora rebanho aponta exatamente para os enderecos de todos os animais, incluindo o inserido;
        area = areaNovaAlocacao;

        std::cout << "nova area: --" << a->getNomeArea()  << "-- foi aidicionada com sucesso!"
        << std::endl;
};


void Farm::removerArea(Area* a) {
    int indexAreaRemovido = -1;

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i]->getNomeArea() == a->getNomeArea()) {
            indexAreaRemovido = i;
            if (a->getQtdAnimais() > 0) {
                //fazer logica para permitir remover a area se confirmar que realmente quer
                //destruir todos os rebanhos da area
                std::cout << "Voce nao pode remover essa area, existem animais nela!" << std::endl;
                indexAreaRemovido = -2;
            }
            break;
        }
    }

    if (indexAreaRemovido == -2) {
        std::cout << "Erro: Area nao foi removida pois haviam animais ainda presentes." << std::endl;
        return;
    }

    if (indexAreaRemovido == -1) {
        std::cout << "Erro: Area nao encontrado na fazenda ou nao existe." << std::endl;
        return;
    }

    Area** areaNovaAlocacao = new Area*[qtdAreas-1];

    for (int i = 0; i < indexAreaRemovido; i++) {
        areaNovaAlocacao[i] = area[i];
    }

    for (int i = indexAreaRemovido + 1; i < qtdAreas; i++) {
        areaNovaAlocacao[i - 1] = area[i];
    }

    delete area[indexAreaRemovido];
    delete[] area;

    area = areaNovaAlocacao;

    qtdAreas--;
    std::cout << "Area removida com sucesso!" << std::endl;
}


int Farm::getQuantidadeTotalAnimais() {
    for (int i = 0; i < qtdAreas; i++) {
        qtdTotalDeAnimais+= area[i]->getQtdAnimais();
    }
    return qtdTotalDeAnimais;
}

void Farm::selecionarArea(std::string n) {
    for(int i=0; i < qtdAreas; i++){
        if(area[i]->getNomeArea() == n){
            std :: cout << "Area: "<< area[i]->getNomeArea() << " Selecionada" << std :: endl;
            areaAtual = n;
        }
    }
}

void Farm::promoverAnimalGlobal() {
    if(area==nullptr) throw std::runtime_error("Area da fazenda não foi alocada!");

    for(int i=0 ; i < qtdAreas ; i++){
        if(area[i]==nullptr) continue;
        area[i]->promoverAnimal();
    } 
}

Animal* Farm::buscarAnimalGlobal(string identificador) {
    for (int i = 0; i < qtdAreas; i++) {
        if (area[i]->buscarAnimal(identificador)) {
            return area[i]->buscarAnimal(identificador);
        } else {
            throw std::out_of_range("Animal nao encontrado em nenhuma area!");
        }
    }
}

bool Farm::transferirAnimalEntreAreas(string idAnimal, string nomeArea) {
    Animal* temporario = nullptr;
    bool achouAnimal = false;
    bool achouAreaDestino = false;

    for (int i = 0; i < qtdAreas; i++) {
        if(area[i]->buscarAnimal(idAnimal) != nullptr) {
            temporario = area[i]->buscarAnimal(idAnimal);
            area[i]->removerAnimal(idAnimal);
            achouAnimal = true;
            for (int j = 0; j < qtdAreas; j++) {
                if (area[i]->getNomeArea() == nomeArea) {
                    achouAreaDestino = true;
                    area[j]->adicionarAnimal(temporario);
                }
            }
        };
    }
    if (!achouAnimal) throw std::runtime_error("Nao foi encontrado um animal com esse id em nenhuma area");
    if (!achouAreaDestino) throw std::runtime_error("Nao foi encontrada a area de destino");
    if (achouAnimal && achouAreaDestino) std::cout << "Animal enviado com sucesso para area de destino" << std::endl;

}

std::string Farm::getAreaAtual() const {return areaAtual;}

