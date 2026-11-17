 #ifndef PARSERPLANILHA_HPP
 #define PARSERPLANILHA_HPP

 #include <string>

// /*vitar depend. circular eventualmente (trem grande dmais vai que ne) O seguro morreu de velho*/

 class Farm;


//  // leitura, escrita e tokenizacao manual de arquivos CSV.
class ParserPlanilha {
public:
    static int importarCSV(std::string arquivo, Farm& f);
    static bool exportarCSV(std::string arquivo, Farm& f);
    static std::string splitManual(std::string linha, char delim, int tamOut);
 };

#endif // PARSERPLANILHA_HPP