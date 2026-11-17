/* #include <iostream>
#include "historico.hpp"

HistoricoSanitario::HistoricoSanitario(){

    capacidadeTotal_ = 5;
    quantidadeAtual_ = 0;

    registros_ = new RegistroSanitario*[capacidadeTotal_];
}

HistoricoSanitario::~HistoricoSanitario(){

    for(int i=0; i < quantidadeAtual_; i++){
        delete registros_[i];
    }

    delete[] registros_;
        
}

void HistoricoSanitario::aumentarCapacidade(){
    int novaCapacidade = capacidadeTotal_ + 500;
   
    RegistroSanitario** novosRegistros = new RegistroSanitario*[novaCapacidade];

    for(int i=0; i<quantidadeAtual_; i++){
        novosRegistros[i] = registros_[i];
    }

    delete[] registros_; 
    registros_ = novosRegistros;
    capacidadeTotal_ = novaCapacidade;
}

bool HistoricoSanitario::registrarVacina(const Vacina& vacina, int dia){

    if(quantidadeAtual_ >= capacidadeTotal_){
        aumentarCapacidade();
    }

    int dosesAnteriores = 0;
    for(int i = 0; i < quantidadeAtual_; i++){
        if(registros_[i]->get_nomeVacina() == vacina.get_Nome()){
            dosesAnteriores++;
        }
    }

    registros_[quantidadeAtual_] = new RegistroSanitario(vacina, dia, dosesAnteriores + 1);
    quantidadeAtual_++;
    return true;
}
