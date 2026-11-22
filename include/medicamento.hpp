#ifndef MEDICAMENTO_HPP
#define MEDICAMENTO_HPP

#include <string>

// Categorias de medicamento
enum CategoriaMedicamento {
    VACINA,
    ANTIBIOTICO,
    VERMIFUGO,
    ANTI_INFLAMATORIO,
    OUTRO
};

class Medicamento {
    private:
        std::string nome_;
        CategoriaMedicamento categoria_;
        int intervaloDoses_;   // dias entre doses (0 = dose unica)
        double custo_;         // custo por dose em reais
        int diasCarencia_;     // dias de carencia apos a aplicacao

    public:
        Medicamento(std::string nome, CategoriaMedicamento categoria, int intervaloDoses, double custo, int diasCarencia);

        //getters
        std::string get_Nome() const;
        std::string get_Tipo() const;       
        
        //categoria p/ texto
        CategoriaMedicamento get_Categoria() const;

        int get_Intervalo() const;
        double get_custo() const;
        int get_diasCarencia() const;

        bool ehVacina() const;

        //conversao de categoria pra texto
        static std::string categoriaParaTexto(CategoriaMedicamento c);
        static CategoriaMedicamento textoParaCategoria(const std::string& texto);
};

#endif