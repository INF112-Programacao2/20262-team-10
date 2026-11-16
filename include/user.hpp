#ifndef USER_HPP
#define USER_HPP

#include <iostream>

class User {
    private:
        std::string idUsuario;
        std::string login; //usuario
        std::string senhaHash;
        std::string nomeProprietario;
        std::string perfilAcesso;
        
        double cotacaoDolar;
        double precoLeite;
        double precoSacaRacao;
        double precoArrobaBoi;
        double precoArrobaVaca;

    public:
        User(std::string login, std::string senha, std::string nome, std::string perfil);
        //~User();
        bool autenticar(std::string login, std::string senha);
        void atualizarCotacaoDolar();
        void atualizarCotacaoArroba();
        

       // double getPrecoLeite();
        double getCotacaoDolar();
        double getCotacaoArrobaBoi();
        double getSacaRacao();
        double getCotacaoArrobaVaca();

        //sets
        void setPrecoArroba(double p);
        void setPrecoLeite(double p);
};

#endif 