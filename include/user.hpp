#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include "sacaRacao.hpp"

class User {
    private:
        std::string idUsuario;
        std::string login; //usuario
        std::string senhaHash;
        std::string nomeProprietario;
        std::string perfilAcesso;
        
        SacaRacao** TiposRacoes;
        double cotacaoDolar;
        double precoLeite;
        double precoArrobaBoi;
        double precoArrobaVaca;

    public:
        User(std::string login, std::string senha, std::string nome, std::string perfil);
        static std::string gerarHash(const std::string& senha); //gera o hash da senha>
        static bool autenticar(std::string login, std::string senha);
        void atualizarCotacaoDolar();
        void atualizarCotacaoArroba();
        void atualizarCotacoes();
        void salvarEmArquivo();
        

        double getPrecoLeite();
        double getCotacaoDolar();
        double getCotacaoArrobaBoi();
        double getSacaRacaoEngorda();
        double getSacaRacaoRecria();
        double getSacaRacaoAvulsa();
        double getKgRacaoEngorda();
        double getKgRacaoAvulsa();
        double getKgRacaoRecria();
        double getCotacaoArrobaVaca();

        //sets
        void setPrecoArrobaBoi(double p);
        void setPrecoArrobaVaca(double p);
        void setCotacaoDolar(double p);
        void setPrecoLeite(double p);
        void setPrecoSacaRacaoRecria(double p);
        void setPrecoSacaRacaoEngorda(double p);
        void setPrecoSacaRacaoAvulsa(double p);

        void setKgRacaoRecria(double p);
        void setKgRacaoAvulsa(double p);
        void setKgRacaoEngorda(double p);

        SacaRacao** getRelatorioRacao();
};

#endif 