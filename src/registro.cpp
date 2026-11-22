#include "registro.hpp"

RegistroSanitario::RegistroSanitario(const Medicamento& medicamento, int diaAplicacao, int numeroDose)
    :medicamento_(medicamento), diaAplicacao_(diaAplicacao), numeroDose_(numeroDose), diasCarenciaRestantes_(medicamento.get_diasCarencia()){}

std::string RegistroSanitario::get_nomeMedicamento() const{
    return medicamento_.get_Nome();
}

int RegistroSanitario::get_DiaAplic() const{
    return diaAplicacao_;
}

int RegistroSanitario::get_NumeroDose() const{
    return numeroDose_;
}

double RegistroSanitario::get_Custo() const{
    return medicamento_.get_custo();
}

int RegistroSanitario::get_DiasCarenciaRestantes() const{
    return diasCarenciaRestantes_;
}

std::string RegistroSanitario::get_Tipo() const{
    return medicamento_.get_Tipo();
}

CategoriaMedicamento RegistroSanitario::get_Categoria() const{
    return medicamento_.get_Categoria();
}

void RegistroSanitario::avancarDia(){
    if(diasCarenciaRestantes_ > 0){
        diasCarenciaRestantes_--;
    }
}

bool RegistroSanitario::emCarencia() const{
    return diasCarenciaRestantes_ > 0;
}