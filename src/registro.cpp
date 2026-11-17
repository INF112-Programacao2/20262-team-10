// #include "registro.hpp"

// RegistroSanitario::RegistroSanitario(const Vacina& vacina, int diaAplicacao, int numeroDose)
//     :vacina_(vacina), diaAplicacao_(diaAplicacao), numeroDose_(numeroDose), diasCarenciaRestantes_(vacina.get_diasCarencia()) {}

// std::string RegistroSanitario::get_nomeVacina() const{
//     return vacina_.get_Nome();
// }

// int RegistroSanitario::get_DiaAplic() const{
//     return diaAplicacao_;
// }

// int RegistroSanitario::get_NumeroDose() const{
//     return numeroDose_;
// }

// double RegistroSanitario::get_Custo() const{
//     return vacina_.get_custo();
// }

// int RegistroSanitario::get_DiasCarenciaRestantes() const{
//     return diasCarenciaRestantes_;
// }

// std::string RegistroSanitario::get_Tipo() const{
//     return vacina_.get_Tipo();
// }

// void RegistroSanitario::avancarDia(){
//     if(diasCarenciaRestantes_ >0){
//         diasCarenciaRestantes_--;
//     }
// }

// bool RegistroSanitario::emCarencia() const{
//     return diasCarenciaRestantes_ > 0;
// }

