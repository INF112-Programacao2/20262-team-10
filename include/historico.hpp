#ifndef HISTORICO_HPP
#define HISTORICO_HPP

#include <string>

#include "registro.hpp"
#include "medicamento.hpp"

class HistoricoSanitario{
    private:

        RegistroSanitario** registros_;
        int capacidadeTotal_;
        int quantidadeAtual_;

        void aumentarCapacidade();
        void adicionarRegistro(RegistroSanitario *registro);
        int achaUltimoDia(const std::string& nomeMedicamento) const;

    public: 

        HistoricoSanitario();

        ~HistoricoSanitario();

        bool registrarMedicamento(const Medicamento& medicamento, int dia);

        void avancarDia();

        bool emCarencia() const;

        bool podeAplicar(const Medicamento& medicamento, int dia) const;

        int contVacinas() const;

        //quantos registros existem de uma categoria
        int contarPorCategoria(CategoriaMedicamento categoria) const;

        void exibir() const;

        //getters

        int get_DiasCarenciaRestantes() const;

        double get_CustoTotal() const;

        int get_QuantidadeAtual() const;

        double get_CustoNoPeriodo(int diaInicial, int diaFinal) const;

};

#endif
