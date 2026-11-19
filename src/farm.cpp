#include "farm.hpp"
#include "animal.hpp"
#include "touro.hpp"
#include "vaca.hpp"
#include "bezerro.hpp"
#include <iostream>

Farm::Farm(std::string s, std::string l, User* propr) {
    nome = s;
    localizacao = l;
    qtdAreas = 0;
    proprietario = propr;
    area = nullptr;
    areaAtual = "";
}

Farm::~Farm() {
    
}

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

// double Farm::venderAnimal(string id) {
//     precoVenda = buscarAnimalGlobal(id).;
// }

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
    qtdTotalDeAnimais = 0;
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
        Animal* encontrado = area[i]->buscarAnimal(identificador);
        if (encontrado != nullptr) {
            return encontrado;
        }
    }
    throw std::out_of_range("Animal nao encontrado em nenhuma area!");
}

bool Farm::transferirAnimalEntreAreas(string idAnimal, string nomeArea) {
Animal* temporario = nullptr;
    int indexOrigem = -1;
    int indexDestino = -1;

    for (int i = 0; i < qtdAreas; i++) {
        if (area[i]->getNomeArea() == nomeArea) {
            indexDestino = i;
        }
        if (area[i]->buscarAnimal(idAnimal) != nullptr) {
            indexOrigem = i;
        }
    }

    if (indexOrigem == -1) throw std::invalid_argument("Erro: Animal de id" + idAnimal + " não encontrado em nenhuma área para transferência.");
    if (indexDestino == -1) throw std::invalid_argument("Erro: Área de destino não encontrada na fazenda.");

    temporario = area[indexOrigem]->extrairAnimal(idAnimal);
    area[indexDestino]->adicionarAnimal(temporario);
    
    std::cout << "Animal transferido com sucesso para a área " << nomeArea << std::endl;
    return true;

}

bool Farm::transferirLoteDeAnimaisEntreAreas(string* ids, int qtdAnimais, string areaDestino) {
    for (int i = 0; i < qtdAnimais; i++) {
        transferirAnimalEntreAreas(ids[i], areaDestino);
    }
    return true;
}

void Farm::realizarCobertura(const std::string& idMacho, const std::string& idFemea) {
    Animal* Macho = buscarAnimalGlobal(idMacho);
    Animal* Femea = buscarAnimalGlobal(idFemea);
    Touro* touro = dynamic_cast<Touro*>(Macho);
    Vaca* vaca = dynamic_cast<Vaca*>(Femea);
    if(touro==nullptr) throw std::invalid_argument("O id do touro nao foi encontrado!");
    if(vaca==nullptr) throw std::invalid_argument("O id da vaca nao foi encontrado!");
    touro->cobrirFemea(vaca);
}

void Farm::registrarNascimento(const std::string& idFemea, const std::string& nomeArea, std::string id, char sexo, double peso) {
    if(sexo!= 'M' && sexo!='F') throw std::invalid_argument("Sexo invalido!");
    if(peso<=0) throw std::invalid_argument("Peso invalido!");
    Animal* femea = buscarAnimalGlobal(idFemea);
    Vaca* mae = dynamic_cast<Vaca*>(femea);
    if(mae==nullptr) throw std::invalid_argument("Mae do bezerro nao encontrada!");
    Area* destino = nullptr;     
    for(int i=0; i<qtdAreas; i++){
        if(area[i]!=nullptr && area[i]->getNomeArea() == nomeArea){
            destino = area[i];
            break;
        }
    }
    if (destino == nullptr) {
        throw std::invalid_argument("Area de destino nao encontrada!");
    }
    Bezerro* filhote = new Bezerro(id, sexo, peso);
    if(filhote==nullptr) throw std::invalid_argument("Bezerro nao registrado!");
    std::string idPai = mae->parir();
    destino->adicionarAnimal(filhote);
    filhote->setIdMae(idFemea);
    filhote->setIdPai(idPai);
    mae->adicionarFilho(id);
    if(!idPai.empty()){
        Animal* macho = buscarAnimalGlobal(idPai);
        Touro* pai = dynamic_cast<Touro*>(macho);
        if(pai!= nullptr) pai->adicionarFilho(id);
    }
}

std::string Farm::getAreaAtual() const {return areaAtual;}