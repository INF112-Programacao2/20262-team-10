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

    public: 
        SnapshotAncestral();
        SnapshotAncestral(std::string id, char sexo, double ultimoPeso, std::string status);

        bool get_Registrado() const;
        std::string get_id() const;
        char get_Sexo() const;
        double get_UltimoPeso() const;
        std::string get_Status() const; 
};

#endif