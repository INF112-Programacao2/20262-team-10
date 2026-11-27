#ifndef SNAPSHOTANCESTRAL_HPP
#define SNAPSHOTANCESTRAL_HPP

#include <string>
#include <iosfwd>

class SnapshotAncestral {
    private: 
        bool registrado_;
        std::string id_;
        char sexo_;
        double ultimoPeso_;
        std::string status_; 
        double scoreBase_; 
        std::string idPai;
        std::string idMae;

    public: 
        SnapshotAncestral();
        SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status,
                          double scoreBase = 0.0, std::string idPai = "", std::string idMae = "");

        bool get_Registrado() const;
        std::string get_id() const;
        char get_Sexo() const;
        double get_UltimoPeso() const;
        std::string get_Status() const; 
        double get_ScoreBase() const;
        std::string getIdPai() const;
        std::string getIdMae() const;

        //setters 
        void set_ScoreBase(double s);
        void setUltimoPeso(double p);
        void setStatus(const std::string& s);
        void setIdPai(const std::string& idPai);
        void setIdMae(const std::string& idMae);

        void atualizarDados(double ultimoPeso, const std::string& status, double scoreBase,
                            const std::string& idPai, const std::string& idMae);

        void escrever(std::ostream& out) const;
        bool ler(std::istream& in);
};

#endif