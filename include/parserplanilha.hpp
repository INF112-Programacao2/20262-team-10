#ifndef PARSERPLANILHA_HPP
#define PARSERPLANILHA_HPP

#include <iosfwd>
#include <sstream>
#include <string>

//class declaration for evitar dependência circular(chamar varias vzs a mesma class)

class Farm;
class Animal;
class SnapshotAncestral;

struct ResumoIportacao {
    int areas = 0;
    int animais = 0;
    int ancestrais = 0; // registros lidos (linhas ANCESTRAL + os anexados no fim de linhas ANIMAL)
};

class ParserPlanilha {
    private:
        Farm& fazenda; // referencia para nn sobrecarregar o parser com posse da fazenda
        std::istringstream entrada; //linha atual reaproveitada a cada linha, nao cria stream por linha
        int numLinha;
        ResumoIportacao resumo;
        explicit ParserPlanilha(Farm& f); //so metodos estaticos criam um parser
        //leitura
        void lerFluxo(std::istream& fonte);
        void processarlinha(std::string& texto);
        void lerArea();
        void lerAncestral();
        void lerAnimal();
        void esperaFimDaLinha();
        bool hamaiscampos();
        //genealogia que trem doidimais da conta gente sô
        // acestrais anexados tem q voltar por referencia ( nn pode trazer nada se nn houver nd a ser "trazido"? )
        void lerGenealogia(Animal& animal, SnapshotAncestral& snapPai, SnapshotAncestral& snapMae);
        void LerSnapshot(SnapshotAncestral& destino, const char* campo);
        void conferirAncestralAnexado(const SnapshotAncestral& snap, const std::string& idEsperado, const char* campo) const;
        // volta de animal, porque? porque retorna o animal do new
        Animal* criarAnimal(const std::string& tipo, const std::string& idToken);
        Animal* criarVaca(const std::string& tipo, const std::string& idToken);
        Animal* criarTouro(const std::string& tipo, const std::string& idToken);
        Animal* criarBoicorte(const std::string& tipo, const std::string& idToken);
        Animal* criarBezerro(const std::string& tipo, const std::string& idToken);
        //leitura volta na referencia
        void ler(std::string& destino, const char* campo);
        void ler(double& destino, const char* campo);
        

};


#endif 