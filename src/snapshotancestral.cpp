#include "snapshotancestral.hpp"

SnapshotAncestral::SnapshotAncestral() : registrado_(false), id_(""), sexo_(' '), ultimoPeso_(0.0), status_(" "){}

SnapshotAncestral::SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status) 
    : registrado_(true), id_(id), sexo_(sexo), ultimoPeso_(ultimoPeso), status_(status){}

bool SnapshotAncestral::get_Registrado() const { return registrado_; }
std::string SnapshotAncestral::get_id() const { return id_;}
char SnapshotAncestral::get_Sexo() const { return sexo_;}
double SnapshotAncestral::get_UltimoPeso() const { return ultimoPeso_;}
std::string SnapshotAncestral::get_Status() const { return status_;}