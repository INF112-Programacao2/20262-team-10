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

    public: 

        HistoricoSanitario();

        ~HistoricoSanitario();

        //nao causar duplo delete indevido
        HistoricoSanitario(const HistoricoSanitario&) = delete;
        HistoricoSanitario& operator=(const HistoricoSanitario&) = delete;

        bool registrarMedicamento(const Medicamento& medicamento, int dia);

        void avancarDia();

        bool emCarencia() const;

        int get_DiasCarenciaRestantes() const;

        double get_CustoTotal() const;

        int get_QuantidadeAtual() const;

        //quantos registros existem de uma categoria
        int contarPorCategoria(CategoriaMedicamento categoria) const;

        void exibir() const;

};

#endif
