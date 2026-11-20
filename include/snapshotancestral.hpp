#ifndef SNAPSHOTANCESTRAL_HPP
#define SNAPSHOTANCESTRAL_HPP

#include <string>

class SnapshotAncestral {
    private: 
        bool registrado_;
        std::string id_;
        char sexo_;
        double ultimoPeso_;
        std::string status_; // foi vendido ou morreu 

        SnapshotAncestral* snapshotPai_;
        SnapshotAncestral* snapshotMae_;

    public: 
        SnapshotAncestral();
        SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status);
        SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status, SnapshotAncestral* snapPai, SnapshotAncestral* snapMae);

        bool get_Registrado() const;
        std::string get_id() const;
        char get_Sexo() const;
        double get_UltimoPeso() const;
        std::string get_Status() const; 

        //getters
        SnapshotAncestral* get_SnapshotPai() const;
        SnapshotAncestral* get_SnapshotMae() const;

        //setters
        void set_SnapshotPai(SnapshotAncestral* snapPai);
        void set_SnapshotMae(SnapshotAncestral* snapMae);
};

#endif