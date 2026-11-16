#include <iostream>
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
    int novaCapacidade = capacidadeTotal_ + 5;
   
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
        if(registros_[i] -> get_nomeVacina() == vacina.get_Nome()){
            dosesAnteriores++;
        }
    }

    registros_[quantidadeAtual_] = new RegistroSanitario(vacina, dia, dosesAnteriores + 1);
    quantidadeAtual_++;
    return true;
}


void HistoricoSanitario::avancarDia(){

    for(int i=0; i<quantidadeAtual_; i++){
        registros_[i] -> avancarDia();
    }
}

bool HistoricoSanitario::emCarencia() const{

    return get_DiasCarenciaRestantes() > 0;
}

int HistoricoSanitario::get_DiasCarenciaRestantes() const{

    int maior = 0;

    for(int i=0; i<quantidadeAtual_; i++){
        int dias = registros_[i] -> get_DiasCarenciaRestantes();
        if(dias > maior){
            maior = dias;
        }
    }

    return maior;
}

double HistoricoSanitario::get_CustoTotal() const{

    double total=0.0;

    for(int i=0; i<quantidadeAtual_; i++){
        total += registros_[i] -> get_Custo();
    }

    return total;
}

int HistoricoSanitario::get_QuantidadeAtual() const{
    
    return quantidadeAtual_;
}

void HistoricoSanitario::exibir() const{

    if(quantidadeAtual_ == 0){
        std::cout << "Nenhuma aplicacao registrada.\n";

        return;
    }

    for(int i = 0; i < quantidadeAtual_; i++){
        std::cout << "Dia " << registros_[i] -> get_DiaAplic() << " | " << registros_[i] -> get_nomeVacina()
                  << " | dose " << registros_[i] -> get_NumeroDose() << " | carencia restante: " 
                  << registros_[i] -> get_DiasCarenciaRestantes() << "\n";
    }
}
