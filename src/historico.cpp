#include <iostream>
#include "historico.hpp"

HistoricoSanitario::HistoricoSanitario(){

    capacidadeTotal_ = 5;
    quantidadeAtual_ = 0;

    registros_ = new RegistroSanitario*[capacidadeTotal_];
}

HistoricoSanitario::~HistoricoSanitario(){

    for(int i=0; i<quantidadeAtual_; i++){
        delete registros_[i];
    }

    delete[] registros_;
        
}

void HistoricoSanitario::aumentarCapacidade(){
    int novaCapacidade = (capacidadeTotal_*2);
   
    RegistroSanitario** novosRegistros = new RegistroSanitario*[novaCapacidade];

    for(int i=0; i<quantidadeAtual_; i++){
        novosRegistros[i] = registros_[i];
    }

    delete[] registros_; 
    registros_ = novosRegistros;
    capacidadeTotal_ = novaCapacidade;
}

bool HistoricoSanitario::registrarMedicamento(const Medicamento& medicamento, int dia){

    if(quantidadeAtual_ >= capacidadeTotal_){
        aumentarCapacidade();
    }

    int dosesAnteriores = 0;
    for(int i=0; i<quantidadeAtual_; i++){
        if(registros_[i]->get_nomeMedicamento() == medicamento.get_Nome()){
            dosesAnteriores++;
        }
    }

    registros_[quantidadeAtual_] = new RegistroSanitario(medicamento, dia, dosesAnteriores + 1);
    quantidadeAtual_++;
    return true;
}

void HistoricoSanitario::avancarDia(){
   for (int i=0; i<quantidadeAtual_; i++){
       registros_[i]->avancarDia();
   }
}

bool HistoricoSanitario::emCarencia() const{
  for (int i=0; i<quantidadeAtual_; i++) {
       if (registros_[i]->emCarencia()) return true;
   }
   return false;
}

int HistoricoSanitario::get_DiasCarenciaRestantes() const{
  int maior = 0;
   for (int i=0; i<quantidadeAtual_; i++) {
       if (registros_[i]->get_DiasCarenciaRestantes() > maior){
           maior = registros_[i]->get_DiasCarenciaRestantes();
       }
   }
   return maior;
}

double HistoricoSanitario::get_CustoTotal() const{
   double total = 0;
   for (int i=0; i<quantidadeAtual_; i++) {
       total += registros_[i]->get_Custo();
   }
   return total;
}

int HistoricoSanitario::get_QuantidadeAtual() const{
   return quantidadeAtual_;
}

int HistoricoSanitario::contarPorCategoria(CategoriaMedicamento categoria) const{
   int total = 0;
   for (int i=0; i<quantidadeAtual_; i++) {
       if (registros_[i]->get_Categoria() == categoria) {
           total++;
       }
   }
   return total;
}

void HistoricoSanitario::exibir() const{
   if (quantidadeAtual_ == 0) {
       std::cout << "Nenhum medicamento registrado." << std::endl;
       return;
   }
   for (int i=0; i<quantidadeAtual_; i++){
       std::cout << registros_[i]->get_nomeMedicamento()
                 << " (" << registros_[i]->get_Tipo() << ")"
                 << " | dose " << registros_[i]->get_NumeroDose()
                 << " | dia " << registros_[i]->get_DiaAplic()
                 << " | custo R$ " << registros_[i]->get_Custo()
                 << " | carencia restante: " << registros_[i]->get_DiasCarenciaRestantes()
                 << " dia(s)" << std::endl;
   }
}
