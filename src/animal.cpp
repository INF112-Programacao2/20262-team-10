#include "animal.hpp"
#include <fstream>

Animal::Animal(std::string id, char s, double p) : _id(id), sexo(s), peso(p) {
    idadeMeses = qtdFilhos = qtdVacinas = 0;
    EmQuarentena = false;
    idFilhos = nullptr;
}

//inicializador overload 
Animal::Animal(std::string id, char s, double p, int iM, int qtdF, int qtdV, std::string* filhosIniciais) : _id(id), sexo(s), peso(p), idadeMeses(iM), qtdFilhos(qtdF), qtdVacinas(qtdV) {
   if(qtdFilhos >0 && filhosIniciais!=nullptr){
        idFilhos = new std::string[qtdFilhos];
        for(int i=0; i<qtdFilhos; i++){
            idFilhos[i] = filhosIniciais[i];
        } 
   }
   else{
        qtdFilhos=0;
        idFilhos=nullptr;
   }
}

//gets
std::string Animal::getId() const{return _id;}
std::string Animal::getEstadoSaude() const {return estadoSaude;}
std::string Animal::getTemperamento() const {return temperamento;}
char Animal::getSexo() const {return sexo;}
double Animal::getPeso() const {return peso;}
int Animal::getIdadeMeses() const {return idadeMeses;}    
int Animal::getQtdFilhos() const {return qtdFilhos;}
int Animal::getQtdvacinas() const {return qtdVacinas;}
bool Animal::getQuarentena() const{return EmQuarentena;}
std::string Animal::getPai() const{return idPai;} 
std::string Animal::getMae() const{return idMae;}
std::string* Animal::getFilhos() const{return idFilhos;}

//sets
void Animal::setIdPai(std::string idPai) {this->idPai = idPai;}
void Animal::setIdMae(std::string idMae) {this->idMae = idMae;}

Animal::~Animal() {
    delete[] idFilhos;
}

Animal::Animal() {
    sexo = ' ';
    peso = 0.0;
    idadeMeses = qtdFilhos = qtdVacinas = 0;
    EmQuarentena = false;
    idFilhos = nullptr;
}

std::string Animal::formatarTextoSalvar(std::string texto) const {
    std::string formatado = texto.empty() ? "_" : texto;
    for (size_t i = 0; i < formatado.length(); i++) {
        if (formatado[i] == ' ') formatado[i] = '_';
    }
    return formatado;
}

std::string Animal::formatarTextoCarregar(std::string texto) const {
    if (texto == "_") return "";
    for (size_t i = 0; i < texto.length(); i++) {
        if (texto[i] == '_') texto[i] = ' ';
    }
    return texto;
}

void Animal::escreverBase(std::ofstream& out) const {
    char sexoFmt = (sexo == ' ' || sexo == '\0') ? '_' : sexo;
    char sexoPaiFmt = (snapshotPai.get_Sexo() == ' ' || snapshotPai.get_Sexo() == '\0') ? '_' : snapshotPai.get_Sexo();
    char sexoMaeFmt = (snapshotMae.get_Sexo() == ' ' || snapshotMae.get_Sexo() == '\0') ? '_' : snapshotMae.get_Sexo();

    out << formatarTextoSalvar(_id) << " "
        << formatarTextoSalvar(estadoSaude) << " "
        << formatarTextoSalvar(temperamento) << " "
        << sexoFmt << " "
        << peso << " "
        << idadeMeses << " "
        << qtdFilhos << " "
        << qtdVacinas << " "
        << EmQuarentena << " "
        << formatarTextoSalvar(idPai) << " "
        << formatarTextoSalvar(idMae);

    for (int i = 0; i < qtdFilhos; i++) {
        out << " " << formatarTextoSalvar(idFilhos[i]);
    }

    out << " " << snapshotPai.get_Registrado() << " "
        << formatarTextoSalvar(snapshotPai.get_id()) << " "
        << sexoPaiFmt << " "
        << snapshotPai.get_UltimoPeso() << " "
        << formatarTextoSalvar(snapshotPai.get_Status()) << " "
        << snapshotMae.get_Registrado() << " "
        << formatarTextoSalvar(snapshotMae.get_id()) << " "
        << sexoMaeFmt << " "
        << snapshotMae.get_UltimoPeso() << " "
        << formatarTextoSalvar(snapshotMae.get_Status());
}

bool Animal::carregarBase(std::ifstream& in) {
    if (idFilhos != nullptr) {
        delete[] idFilhos;
        idFilhos = nullptr;
    }
    qtdFilhos = 0;

    std::string idLido, saudeLida, tempLido, paiLido, maeLida;
    char sexoLido;
    double pesoLido;
    int idadeLida, qtdFilhosLida, qtdVacinasLida;
    bool quarentenaLida;

    if (!(in >> idLido >> saudeLida >> tempLido >> sexoLido >> pesoLido
             >> idadeLida >> qtdFilhosLida >> qtdVacinasLida >> quarentenaLida
             >> paiLido >> maeLida)) {
        return false;
    }

    _id = formatarTextoCarregar(idLido);
    estadoSaude = formatarTextoCarregar(saudeLida);
    temperamento = formatarTextoCarregar(tempLido);
    sexo = (sexoLido == '_') ? ' ' : sexoLido;
    peso = pesoLido;
    idadeMeses = idadeLida;
    qtdFilhos = qtdFilhosLida;
    qtdVacinas = qtdVacinasLida;
    EmQuarentena = quarentenaLida;
    idPai = formatarTextoCarregar(paiLido);
    idMae = formatarTextoCarregar(maeLida);

    if (qtdFilhos > 0) {
        idFilhos = new std::string[qtdFilhos];
        for (int i = 0; i < qtdFilhos; i++) {
            std::string filhoLido;
            if (!(in >> filhoLido)) {
                delete[] idFilhos;
                idFilhos = nullptr;
                qtdFilhos = 0;
                return false;
            }
            idFilhos[i] = formatarTextoCarregar(filhoLido);
        }
    }

    bool regPai, regMae;
    std::string idSnapPai, statusSnapPai, idSnapMae, statusSnapMae;
    char sexoSnapPai, sexoSnapMae;
    double pesoSnapPai, pesoSnapMae;

    if (!(in >> regPai >> idSnapPai >> sexoSnapPai >> pesoSnapPai >> statusSnapPai
             >> regMae >> idSnapMae >> sexoSnapMae >> pesoSnapMae >> statusSnapMae)) {
        if (idFilhos != nullptr) {
            delete[] idFilhos;
            idFilhos = nullptr;
            qtdFilhos = 0;
        }
        return false;
    }

    if (regPai) {
        snapshotPai = SnapshotAncestral(
            formatarTextoCarregar(idSnapPai),
            (sexoSnapPai == '_') ? ' ' : sexoSnapPai,
            pesoSnapPai,
            formatarTextoCarregar(statusSnapPai)
        );
    } else {
        snapshotPai = SnapshotAncestral();
    }

    if (regMae) {
        snapshotMae = SnapshotAncestral(
            formatarTextoCarregar(idSnapMae),
            (sexoSnapMae == '_') ? ' ' : sexoSnapMae,
            pesoSnapMae,
            formatarTextoCarregar(statusSnapMae)
        );
    } else {
        snapshotMae = SnapshotAncestral();
    }

    return true;
}