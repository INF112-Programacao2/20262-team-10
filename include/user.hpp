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
        User();
        
        SacaRacao** TiposRacoes;
        double cotacaoDolar;
        double precoLeite;
        double precoArrobaBoi;
        double precoArrobaVaca;

    public:
        User(std::string login, std::string senha, std::string nome, std::string perfil);
        ~User();
        static std::string gerarHash(const std::string& senha); //gera o hash da senha>
        static bool autenticar(std::string login, std::string senha);
        void atualizarCotacaoDolar();
        void atualizarCotacaoArroba();
        void atualizarCotacoes();
        void salvarEmArquivo();
        std::string gerarProximoId();
        static User* carregarUsuario(std::string login, std::string senha);

        // Getters
        double getPrecoLeite();
        double getCotacaoDolar();
        double getCotacaoArrobaBoi();
        double getCotacaoArrobaVaca();
        double getSacaRacaoEngorda();
        double getSacaRacaoAvulsa();
        double getSacaRacaoRecria();
        double getKgRacaoEngorda();
        double getKgRacaoAvulsa();
        double getKgRacaoRecria();
        SacaRacao** getRelatorioRacao();
        std::string getId();

        // Setters
        void setPrecoLeite(double p);
        void setCotacaoDolar(double p);
        void setPrecoArrobaBoi(double p);
        void setPrecoArrobaVaca(double p);
        void setPrecoSacaRacaoEngorda(double p);
        void setPrecoSacaRacaoAvulsa(double p);
        void setPrecoSacaRacaoRecria(double p);
        void setKgRacaoEngorda(double p);
        void setKgRacaoAvulsa(double p);
        void setKgRacaoRecria(double p);
};

#endif 