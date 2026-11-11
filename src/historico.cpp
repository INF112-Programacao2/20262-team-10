/*
#include <iostream>
#include "historico.hpp"

HistoricoSanitario::HistoricoSanitario(){

    capacidadeTotal = 10000;
    quantidadeAtual = 0;

    registros = new RegistroSanitario*[capacidadeTotal];
}

HistoricoSanitario::~HistoricoSanitario(){

    for(int i=0; i < quantidadeAtual; i++){
        delete registros[i];
    }

    delete[] registros;
        
}

void HistoricoSanitario::aumentarCapacidade(){
    capacidadeTotal = 500;
}

bool HistoricoSanitario::registrarVacina(int idAnimal, const Vacina& vacina, int dia){

    if(quantidadeAtual > capacidadeTotal){
        aumentarCapacidade();
    }

    int dosesAnteriores = 0;

    registros[quantidadeAtual] = new RegistroSanitario(idAnimal, vacina.get_Nome(), dia, dosesAnteriores+1);

    quantidadeAtual++;

    return true;
}
*/