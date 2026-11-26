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

SnapshotAncestral::SnapshotAncestral() : registrado_(false), id_(""), sexo_(' '), ultimoPeso_(0.0), status_(""), scoreBase_(0.0), snapshotPai_(nullptr), snapshotMae_(nullptr) {}

SnapshotAncestral::SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status): registrado_(true), id_(id), sexo_(sexo),
    ultimoPeso_(ultimoPeso),status_(status), scoreBase_(0.0), snapshotPai_(nullptr),snapshotMae_(nullptr) {}

SnapshotAncestral::SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status, const SnapshotAncestral* snapPai, const SnapshotAncestral* snapMae, double scoreBase)
    : registrado_(true), id_(id),sexo_(sexo), ultimoPeso_(ultimoPeso), status_(status), scoreBase_(scoreBase), snapshotPai_(nullptr), snapshotMae_(nullptr) {
    if (snapPai != nullptr && snapPai->registrado_) snapshotPai_ = new SnapshotAncestral(*snapPai);
    if (snapMae != nullptr && snapMae->registrado_) snapshotMae_ = new SnapshotAncestral(*snapMae);
}

SnapshotAncestral::SnapshotAncestral(const SnapshotAncestral& outro)
    : registrado_(false), id_(""), sexo_(' '), ultimoPeso_(0.0), status_(""), scoreBase_(0.0), snapshotPai_(nullptr), snapshotMae_(nullptr) {
    copiarDe(outro);
}

SnapshotAncestral& SnapshotAncestral::operator=(const SnapshotAncestral& outro) {
    if (this != &outro) {
        SnapshotAncestral copia(outro);
        liberar();
        registrado_ = copia.registrado_;
        id_ = copia.id_;
        sexo_ = copia.sexo_;
        ultimoPeso_ = copia.ultimoPeso_;
        status_ = copia.status_;
        scoreBase_ = copia.scoreBase_;
        snapshotPai_ = copia.snapshotPai_;
        snapshotMae_ = copia.snapshotMae_;
        copia.snapshotPai_ = nullptr; 
        copia.snapshotMae_ = nullptr;
    }
    return *this;
}

SnapshotAncestral::~SnapshotAncestral() { liberar(); }

void SnapshotAncestral::copiarDe(const SnapshotAncestral& outro) {
    registrado_ = outro.registrado_;
    id_ = outro.id_;
    sexo_ = outro.sexo_;
    ultimoPeso_ = outro.ultimoPeso_;
    status_ = outro.status_;
    scoreBase_ = outro.scoreBase_;
    if (outro.snapshotPai_ != nullptr) snapshotPai_ = new SnapshotAncestral(*outro.snapshotPai_);
    if (outro.snapshotMae_ != nullptr) snapshotMae_ = new SnapshotAncestral(*outro.snapshotMae_);
}

void SnapshotAncestral::liberar() {
    delete snapshotPai_;
    delete snapshotMae_;
    snapshotPai_ = nullptr;
    snapshotMae_ = nullptr;
}

bool SnapshotAncestral::get_Registrado() const { return registrado_; }
std::string SnapshotAncestral::get_id() const { return id_; }
char SnapshotAncestral::get_Sexo() const { return sexo_; }
double SnapshotAncestral::get_UltimoPeso() const { return ultimoPeso_; }
std::string SnapshotAncestral::get_Status() const { return status_; }
double SnapshotAncestral::get_ScoreBase() const { return scoreBase_; }
void SnapshotAncestral::set_ScoreBase(double s) { scoreBase_ = s; }

const SnapshotAncestral* SnapshotAncestral::get_SnapshotPai() const { return snapshotPai_; }
const SnapshotAncestral* SnapshotAncestral::get_SnapshotMae() const { return snapshotMae_; }

void SnapshotAncestral::set_SnapshotPai(const SnapshotAncestral* snapPai) {
    SnapshotAncestral* novo = nullptr;
    if (snapPai != nullptr && snapPai->registrado_) novo = new SnapshotAncestral(*snapPai);
    delete snapshotPai_;
    snapshotPai_ = novo;
}

void SnapshotAncestral::set_SnapshotMae(const SnapshotAncestral* snapMae) {
    SnapshotAncestral* novo = nullptr;
    if (snapMae != nullptr && snapMae->registrado_) novo = new SnapshotAncestral(*snapMae);
    delete snapshotMae_;
    snapshotMae_ = novo;
}

bool SnapshotAncestral::atualizarAncestral(const SnapshotAncestral& novo) {
    bool achou = false;
    if (registrado_ && id_ == novo.id_) {
        ultimoPeso_ = novo.ultimoPeso_;
        status_ = novo.status_;
        scoreBase_ = novo.scoreBase_;
        achou = true;
    }
    if (snapshotPai_ != nullptr && snapshotPai_->atualizarAncestral(novo)) achou = true;
    if (snapshotMae_ != nullptr && snapshotMae_->atualizarAncestral(novo)) achou = true;
    return achou;
}

void SnapshotAncestral::escrever(std::ostream& out) const {
    out << (registrado_ ? 1 : 0) << ' ' << codificar(id_) << ' ' << (sexo_ == ' ' ? '_' : sexo_) << ' '
        << ultimoPeso_ << ' ' << codificar(status_) << ' ' << scoreBase_ << ' ' << (snapshotPai_ != nullptr ? 1 : 0);
    if (snapshotPai_ != nullptr) { out << ' '; snapshotPai_->escrever(out); }
    out << ' ' << (snapshotMae_ != nullptr ? 1 : 0);
    if (snapshotMae_ != nullptr) { out << ' '; snapshotMae_->escrever(out); }
}

bool SnapshotAncestral::ler(std::istream& in) {
    int reg, temPai, temMae;
    std::string id, status;
    char sexo;
    double peso, score;

    if (!(in >> reg >> id >> sexo >> peso >> status >> score >> temPai)) return false;

    liberar();
    registrado_ = (reg != 0);
    id_ = decodificar(id);
    sexo_ = (sexo == '_') ? ' ' : sexo;
    ultimoPeso_ = peso;
    status_ = decodificar(status);
    scoreBase_ = score;

    if (temPai != 0) {
        snapshotPai_ = new SnapshotAncestral();
        if (!snapshotPai_->ler(in)) {
            liberar();
            registrado_ = false;
            id_.clear();
            sexo_ = ' ';
            ultimoPeso_ = 0.0;
            status_.clear();
            return false;
        }
    }

    if (!(in >> temMae)) {
        liberar();
        registrado_ = false;
        id_.clear();
        sexo_ = ' ';
        ultimoPeso_ = 0.0;
        status_.clear();
        return false;
    }

    if (temMae != 0) {
        snapshotMae_ = new SnapshotAncestral();
        if (!snapshotMae_->ler(in)) {
            liberar();
            registrado_ = false;
            id_.clear();
            sexo_ = ' ';
            ultimoPeso_ = 0.0;
            status_.clear();
            return false;
        }
    }

    return true;
}