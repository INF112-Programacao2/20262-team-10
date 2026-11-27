#include "snapshotancestral.hpp"
#include <istream>
#include <ostream>

namespace {
    std::string codificar(const std::string& texto) {
        if (texto.empty()) return "_";
        std::string r = texto;
        for (size_t i = 0; i < r.size(); i++) if (r[i] == ' ') r[i] = '_';
        return r;
    }
    std::string decodificar(const std::string& texto) {
        if (texto == "_") return "";
        std::string r = texto;
        for (size_t i = 0; i < r.size(); i++) if (r[i] == '_') r[i] = ' ';
        return r;
    }
}

SnapshotAncestral::SnapshotAncestral()
    : registrado_(false), id_(""), sexo_(' '), ultimoPeso_(0.0), status_(""), scoreBase_(0.0), idPai(""), idMae("") {}

SnapshotAncestral::SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status,
                                     double scoreBase, std::string idPai, std::string idMae)
    : registrado_(true), id_(id), sexo_(sexo), ultimoPeso_(ultimoPeso),
      status_(status), scoreBase_(scoreBase), idPai(idPai), idMae(idMae) {}

bool SnapshotAncestral::get_Registrado() const { return registrado_; }
std::string SnapshotAncestral::get_id() const { return id_; }
char SnapshotAncestral::get_Sexo() const { return sexo_; }
double SnapshotAncestral::get_UltimoPeso() const { return ultimoPeso_; }
std::string SnapshotAncestral::get_Status() const { return status_; }
double SnapshotAncestral::get_ScoreBase() const { return scoreBase_; }
std::string SnapshotAncestral::getIdPai() const { return idPai; }
std::string SnapshotAncestral::getIdMae() const { return idMae; }

void SnapshotAncestral::set_ScoreBase(double s) { scoreBase_ = s; }
void SnapshotAncestral::setUltimoPeso(double p) { ultimoPeso_ = p; }
void SnapshotAncestral::setStatus(const std::string& s) { status_ = s; }
void SnapshotAncestral::setIdPai(const std::string& idPai) { this->idPai = idPai; }
void SnapshotAncestral::setIdMae(const std::string& idMae) { this->idMae = idMae; }

void SnapshotAncestral::atualizarDados(double ultimoPeso, const std::string& status, double scoreBase,
                                       const std::string& idPai, const std::string& idMae) {
    registrado_ = true;
    ultimoPeso_ = ultimoPeso;
    status_ = status;
    scoreBase_ = scoreBase;
    if (!idPai.empty()) this->idPai = idPai;
    if (!idMae.empty()) this->idMae = idMae;
}

void SnapshotAncestral::escrever(std::ostream& out) const {
    char sexoFmt = (sexo_ == ' ' || sexo_ == '\0') ? '_' : sexo_;
    out << codificar(id_) << ' '
        << sexoFmt << ' '
        << ultimoPeso_ << ' '
        << codificar(status_) << ' '
        << scoreBase_ << ' '
        << codificar(idPai) << ' '
        << codificar(idMae);
}

bool SnapshotAncestral::ler(std::istream& in) {
    std::string idLido, statusLido, idPaiLido, idMaeLido;
    char sexoLido;
    double pesoLido, scoreLido;

    if (!(in >> idLido >> sexoLido >> pesoLido >> statusLido >> scoreLido >> idPaiLido >> idMaeLido)) {
        return false;
    }

    registrado_ = true;
    id_ = decodificar(idLido);
    sexo_ = (sexoLido == '_') ? ' ' : sexoLido;
    ultimoPeso_ = pesoLido;
    status_ = decodificar(statusLido);
    scoreBase_ = scoreLido;
    idPai = decodificar(idPaiLido);
    idMae = decodificar(idMaeLido);
    return true;
}