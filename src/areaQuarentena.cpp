//  #include "areaQuarentena.hpp"

// #include <iostream>
// #include <iomanip>
// #include <stdexcept>

// //custo fixo de isolamento por animal/dia em reais, pode ser alterado pelo setter
// static const double CUSTO_DIARIO_PADRAO = 25.0;

// //quanto maior o nivel de bioseguranca, mais caro e manter o animal isolado
// static double fatorBioseguranca(int nivel){
//     if(nivel == 2) return 1.25;
//     if(nivel == 3) return 1.5;
//     return 1.0;
// }

// AreaQuarentena::AreaQuarentena(std::string nome, double hectares, int nivelBioseguranca, std::string crmvResponsavel)
//     :Area(nome, "intermediario", hectares), fichas_(nullptr), qtdFichas_(0), nivelBioseguranca_(nivelBioseguranca),
//     crmvResponsavel_(crmvResponsavel), custoDiarioIsolamento_(CUSTO_DIARIO_PADRAO){

//     if (nivelBioseguranca_ < 1 || nivelBioseguranca_ > 3) throw std::invalid_argument("O nivel de bioseguranca deve ser de 1 a 3!");
//     if (crmvResponsavel_.empty()) throw std::invalid_argument("O CRMV do responsavel nao pode ser vazio!");
// }

// AreaQuarentena::~AreaQuarentena(){
//     delete[] fichas_;
// }

// //metodos privados das fichas

// int AreaQuarentena::buscarFicha(const std::string& idAnimal) const{
//     for(int i=0; i<qtdFichas_; i++){
//         if(fichas_[i].idAnimal_ == idAnimal){
//             return i;
//         }
//     }
//     return -1;
// }

// void AreaQuarentena::adicionarFicha(const FichaIsolamento& ficha){

//     FichaIsolamento* novasFichas = new FichaIsolamento[qtdFichas_ + 1];

//     for(int i=0; i<qtdFichas_; i++){
//         novasFichas[i] = fichas_[i];
//     }

//     novasFichas[qtdFichas_] = ficha;

//     delete[] fichas_;
//     fichas_ = novasFichas;
//     qtdFichas_++;
// }

// void AreaQuarentena::removerFicha(int indice){

//     if(indice < 0 || indice >= qtdFichas_){
//         throw std::out_of_range("Indice de ficha invalido!");
//     }

//     //era a ultima ficha
//     if(qtdFichas_ == 1){
//         delete[] fichas_;
//         fichas_ = nullptr;
//         qtdFichas_ = 0;
//         return;
//     }

//     FichaIsolamento* novasFichas = new FichaIsolamento[qtdFichas_ - 1];

//     for(int i=0; i<indice; i++){
//         novasFichas[i] = fichas_[i];
//     }
//     for(int i=indice+1; i<qtdFichas_; i++){
//         novasFichas[i-1] = fichas_[i];
//     }

//     delete[] fichas_;
//     fichas_ = novasFichas;
//     qtdFichas_--;
// }

// //entrada, saida e passagem de dias

// void AreaQuarentena::entradaEmQuarentena(Animal* a, Area* origem, std::string motivo, int dias, double receitaDiariaPerdida){

//     if (a == nullptr) throw std::invalid_argument("Animal invalido para quarentena!");
//     if (origem == nullptr) throw std::invalid_argument("Area de origem invalida!");
//     if (origem == this) throw std::invalid_argument("O animal ja esta nesta area!");
//     if (motivo.empty()) throw std::invalid_argument("Informe o motivo da quarentena!");
//     if (dias <= 0) throw std::invalid_argument("Os dias de quarentena devem ser maiores que zero!");
//     if (receitaDiariaPerdida < 0.0) throw std::invalid_argument("A receita perdida nao pode ser negativa!");
//     if (a->getQuarentena()) throw std::runtime_error("Este animal ja esta em quarentena!");
//     if (origem->buscarAnimal(a->getId()) != a) throw std::runtime_error("O animal nao pertence a area de origem informada!");

//     FichaIsolamento ficha;
//     ficha.idAnimal_ = a->getId();
//     ficha.motivo_ = motivo;
//     ficha.areaOrigem_ = origem->getNomeArea();
//     ficha.diasRestantes_ = dias;
//     ficha.receitaPerdidaDiaria_ = receitaDiariaPerdida;

//     //tira da origem e coloca aqui, o animal nunca fica em duas areas ao mesmo tempo
//     Animal* extraido = origem->extrairAnimal(ficha.idAnimal_);

//     try{
//         adicionarAnimal(extraido, true);
//     } catch (...) {
//         origem->adicionarAnimal(extraido, true);
//         throw;
//     }

//     extraido->setQuarentena(true);
//     adicionarFicha(ficha);

//     std::cout << "Animal --" << ficha.idAnimal_ << "-- enviado para a quarentena " << getNomeArea()
//               << " por " << dias << " dia(s). Motivo: " << motivo << std::endl;
// }

// void AreaQuarentena::avancarDia(){

//     for(int i=0; i<qtdFichas_; i++){
//         if(fichas_[i].diasRestantes_ > 0){
//             fichas_[i].diasRestantes_--;
//         }
//     }

//     //quando o Animal tiver historico sanitario, avancar o dia dele aqui tambem (carencia)
// }

// bool AreaQuarentena::prontoAlta(std::string idAnimal) const{

//     int indice = buscarFicha(idAnimal);

//     if(indice == -1){
//         return false;
//     }

//     return fichas_[indice].diasRestantes_ <= 0;
// }

// //o Farm deve pegar o getAreaOrigem antes de chamar, porque a ficha some depois da alta
// Animal* AreaQuarentena::darAlta(std::string idAnimal){

//     int indice = buscarFicha(idAnimal);

//     if(indice == -1) throw std::runtime_error("Esse animal nao esta em quarentena!");
//     if(!prontoAlta(idAnimal)) throw std::runtime_error("O animal ainda nao cumpriu o periodo de quarentena!");

//     std::string areaOrigem = fichas_[indice].areaOrigem_;

//     Animal* animal = extrairAnimal(idAnimal);

//     //ficha sem animal na area, limpa a ficha pra nao ficar lixo
//     if(animal == nullptr){
//         removerFicha(indice);
//         throw std::runtime_error("O animal da ficha nao foi encontrado na area!");
//     }

//     animal->setQuarentena(false);
//     removerFicha(indice);

//     std::cout << "Animal --" << idAnimal << "-- recebeu alta e deve voltar para a area " << areaOrigem << std::endl;

//     return animal;
// }

// //custos

// //custo de manter um animal isolado por dia (sem racao)
// double AreaQuarentena::calculaPrecoHospitalarDiarioAnimal(std::string id){

//     if(buscarFicha(id) == -1) throw std::runtime_error("Esse animal nao esta em quarentena!");

//     return custoDiarioIsolamento_ * fatorBioseguranca(nivelBioseguranca_);
// }

// double AreaQuarentena::calculaPrecoHospitalarDiarioTotal(){
//     return qtdFichas_ * custoDiarioIsolamento_ * fatorBioseguranca(nivelBioseguranca_);
// }

// //quanto ainda vai custar (isolamento + receita que deixa de entrar) ate todos terem alta
// double AreaQuarentena::estimativaPrejuizoIsolamento(){

//     double total = 0.0;
//     double custoIsolamento = custoDiarioIsolamento_ * fatorBioseguranca(nivelBioseguranca_);

//     for(int i=0; i<qtdFichas_; i++){
//         total += fichas_[i].diasRestantes_ * (fichas_[i].receitaPerdidaDiaria_ + custoIsolamento);
//     }

//     return total;
// }

// //custo economico diario: gasto real da area + receita que os animais deixaram de gerar
// double AreaQuarentena::custoDiario(double precoLeite, SacaRacao** relatorioRacoes){

//     double receitaPerdida = 0.0;

//     for(int i=0; i<qtdFichas_; i++){
//         receitaPerdida += fichas_[i].receitaPerdidaDiaria_;
//     }

//     return getGastoDaArea(precoLeite, relatorioRacoes) + receitaPerdida;
// }

// //gasto em dinheiro: racao (calculada pela Area) + isolamento. receita perdida nao entra aqui
// double AreaQuarentena::getGastoDaArea(double precoLeite, SacaRacao** relatorioRacoes){
//     return Area::getGastoDaArea(precoLeite, relatorioRacoes) + calculaPrecoHospitalarDiarioTotal();
// }

// void AreaQuarentena::listarPacientesNestaQuarentena() const{

//     if(qtdFichas_ == 0){
//         std::cout << "Nenhum animal em quarentena." << std::endl;
//         return;
//     }

//     std::cout << std::fixed << std::setprecision(2);
//     std::cout << "Quarentena " << nome << " | bioseguranca nivel " << nivelBioseguranca_
//               << " | CRMV " << crmvResponsavel_ << std::endl;

//     for(int i=0; i<qtdFichas_; i++){
//         std::cout << fichas_[i].idAnimal_
//                   << " | origem: " << fichas_[i].areaOrigem_
//                   << " | motivo: " << fichas_[i].motivo_
//                   << " | dias restantes: " << fichas_[i].diasRestantes_
//                   << " | receita perdida/dia R$ " << fichas_[i].receitaPerdidaDiaria_ << std::endl;
//     }
// }

// //getters

// std::string AreaQuarentena::getAreaOrigem(std::string idAnimal) const{

//     int indice = buscarFicha(idAnimal);

//     if(indice == -1) throw std::runtime_error("Esse animal nao esta em quarentena!");

//     return fichas_[indice].areaOrigem_;
// }

// int AreaQuarentena::getNivelBioseguranca() const{
//     return nivelBioseguranca_;
// }

// std::string AreaQuarentena::getCrmvResponsavel() const{
//     return crmvResponsavel_;
// }

// int AreaQuarentena::getQtdPacientes() const{
//     return qtdFichas_;
// }

// int AreaQuarentena::getDiasRestantes(std::string idAnimal) const{

//     int indice = buscarFicha(idAnimal);

//     if(indice == -1) throw std::runtime_error("Esse animal nao esta em quarentena!");

//     return fichas_[indice].diasRestantes_;
// }

// //setter

// void AreaQuarentena::setCustoDiarioIsolamento(double c){

//     if (c < 0.0) throw std::invalid_argument("O custo diario de isolamento nao pode ser negativo!");

//     custoDiarioIsolamento_ = c;