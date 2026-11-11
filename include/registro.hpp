#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <string>

class RegistroSanitario{
    private:

        std::string idAnimal;
        std::string nomeVacina; 
        int diaAplicacao;
        int numeroDose;

    public: 

        RegistroSanitario(int id, std::string nome, int dia_aplic, int numero_d);

        int get_Id() const;
        std::string get_nomeVacina() const;
        int get_DiaAplic() const;
        int get_NumeroDose() const;

};
#endif

