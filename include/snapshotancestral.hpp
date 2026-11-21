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

        SnapshotAncestral* snapshotPai_;
        SnapshotAncestral* snapshotMae_;

        void copiarDe(const SnapshotAncestral& outro);
        void liberar();

    public: 
        SnapshotAncestral();
        SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status);
        SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status, const SnapshotAncestral* snapPai, const SnapshotAncestral* snapMae);

        SnapshotAncestral(const SnapshotAncestral& outro);
        SnapshotAncestral& operator=(const SnapshotAncestral& outro);
        ~SnapshotAncestral();

        bool get_Registrado() const;
        std::string get_id() const;
        char get_Sexo() const;
        double get_UltimoPeso() const;
        std::string get_Status() const; 

        //getters 
        const SnapshotAncestral* get_SnapshotPai() const;
        const SnapshotAncestral* get_SnapshotMae() const;

        //setters 
        void set_SnapshotPai(const SnapshotAncestral* snapPai);
        void set_SnapshotMae(const SnapshotAncestral* snapMae);

        bool atualizarAncestral(const SnapshotAncestral& novo);

        // salvar a arvore inteira 
        void escrever(std::ostream& out) const;
        bool ler(std::istream& in);
};

#endif