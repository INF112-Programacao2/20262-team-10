#ifndef USER_HPP
#define USER_HPP

#include <iostream>

class User {
    private:
        std::string login;
        std::string senha;
        std::string nomeProprietario;

    public:
        bool autenticar(std::string l, std::string s);


};

#endif 