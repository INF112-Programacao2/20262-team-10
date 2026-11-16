// #ifndef HISTORICO_HPP
// #define HISTORICO_HPP

// #include <string>
// #include "registro.hpp"
// #include "vacina.hpp"

// class HistoricoSanitario{
//     private:

//         RegistroSanitario** registros_;
//         int capacidadeTotal_;
//         int quantidadeAtual_;

//         void aumentarCapacidade();

//     public: 

//         HistoricoSanitario();

//         ~HistoricoSanitario();

//         bool registrarVacina(const Vacina& vacina, int dia);

//         void avancarDia();

//         bool emCarencia() const;

//         int get_DiasCarenciaRestantes() const;

//         double get_CustoTotal() const;

        int get_QuantidadeAtual() const;

        void exibir() const;

// };

// #endif

