#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <string>

class RegistroSanitario{
    private:

        int idAnimal;
        std::string nomeVacina; 
        int dia_aplicacao;
        int numeroDose;

    public: 

        RegistroSanitario(int id, std::string nome, int dia_aplic, int numero_d);

        int getId();
        std::string get_nomeVacina();
        int getDiaAplic();
        int getNumeroDose();

};
#endif

