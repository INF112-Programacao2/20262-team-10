#ifndef PARSERPLANILHA_HPP
#define PARSERPLANILHA_HPP

#include <string>

/*vitar depend. circular eventualmente (trem grande dmais vai que ne) O seguro morreu de velho*/

class Farm;


 // leitura, escrita e tokenizacao manual de arquivos CSV.
 
class ParserPlanilha {
public:
    /**
     *  Importa areas e animais de um arquivo CSV para a fazenda.
     *  @param arquivo Caminho do arquivo CSV de entrada.
     *  @param f Ponteiro para a instancia de Farm que recebera os dados.
     */
    static int importarCSV(std::string arquivo, Farm* f);

    /*
     * @param arquivo Caminho do arquivo CSV de saida.
     * @param f Ponteiro para a instancia de Farm com os dados atuais.
     * @return true se a exportacao foi concluida, false caso contrario.
     */
    static bool exportarCSV(std::string arquivo, Farm* f);

    /**
     * @brief Extrai uma coluna especifica de uma linha delimitada sem uso de STL avancada.
     * @param linha Texto completo da linha lida.
     * @param delim Caractere separador (ex: ';').
     * @param tamOut Indice (base 0) da coluna desejada.
     * @return Conteudo textual da coluna extraida.
     */
    static std::string splitManual(std::string linha, char delim, int tamOut);
};

#endif // PARSERPLANILHA_HPP