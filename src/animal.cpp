#include "animal.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace {
    //  parametros do score genetico (tenho que conferir ainda, mas podem mudar aqui)
    const double SCORE_REFERENCIA = 50.0;   // animal exatamente na referencia = 50 
    const double PESO_NASC_PADRAO = 35.0;   // usado quando o peso ao nascer nao e conhecido
    const double GANHO_ESPERADO_DIA = 0.75; // kg/dia, curva de crescimento esperada
    const int    IDADE_FIM_CRESCIMENTO = 24; // depois de um 5empo o ganho medio de vida cai por idade e nao entra  mais no score
    const int    GERACOES_AVALIADAS = 2;    // pais e avos

    const double PENAL_SEM_ANCESTRAIS = 8.0;    // quando n tem nenhum ancestral salvo
    const double PENAL_POUCOS_ANCESTRAIS = 3.0; // 1 a 3 ancestrais salvos
    const int    ANCESTRAIS_MINIMOS = 4;        // nao tem penalidade de duvida a partir daqui

    const double PONTOS_POR_ANCESTRAL = 2.0;   // por pai/mae
    const double TETO_AJUSTE_ANCESTRAIS = 8.0;

    // pesos da formula do score base 
    const double W_PESO = 0.30, W_GANHO = 0.35, W_PROD = 0.35;

    double limitar(double v, double lo, double hi) { return v < lo ? lo : (v > hi ? hi : v); }

    double razaoParaPontos(double razao) { return limitar(SCORE_REFERENCIA + (razao - 1.0) * 100.0, 0.0, 100.0); }

    double pesoAdultoReferencia(const std::string& tipo) {
        if (tipo == "vaca") return 550.0;
        if (tipo == "touro") return 900.0;
        if (tipo == "boicorte") return 600.0;
        return 400.0; // bezerro
    }

    double ganhoReferencia(const std::string& tipo) {
        // ganho medioesperado por tipo de animal
        if (tipo == "vaca") return 0.50;
        if (tipo == "touro") return 0.60;
        return 0.75; // bezerro e boi de corte
    }

    int contarRegistrados(const SnapshotAncestral* s, int niveis) {
        if (s == nullptr || !s->get_Registrado() || niveis <= 0) return 0;
        return 1 + contarRegistrados(s->get_SnapshotPai(), niveis - 1)
                 + contarRegistrados(s->get_SnapshotMae(), niveis - 1);
    }

    // soma o efeito dos ancestrais salvos, geracao 1 pesa 1.0, geracao 2 pesa 0.5 
    double somarEfeitoAncestrais(const SnapshotAncestral* s, int niveis, double pesoGeracao, double media) {
        if (s == nullptr || !s->get_Registrado() || niveis <= 0) return 0.0;
        double efeito = 0.0;
        if (s->get_ScoreBase() > 0.0) { // nao bonifica nem penaliza
            efeito = ((s->get_ScoreBase() - media) / SCORE_REFERENCIA) * PONTOS_POR_ANCESTRAL * pesoGeracao;
        }
        return efeito + somarEfeitoAncestrais(s->get_SnapshotPai(), niveis - 1, pesoGeracao * 0.5, media)
                      + somarEfeitoAncestrais(s->get_SnapshotMae(), niveis - 1, pesoGeracao * 0.5, media);
    }
}

Animal::Animal(std::string id, char s, double p)
    : _id(id), sexo(s), peso(p)
{
    idadeMeses = qtdFilhos = qtdVacinas = 0;
    EmQuarentena = false;
    idFilhos = nullptr;
    pesoNascimento = ganhoMedioPeso = scoreBase = ajusteDescendentes = scoreGenetico = 0.0;
}

// inicializador overload
Animal::Animal(std::string id, char s, double p, int iM, int qtdF,
               int qtdV, std::string *filhosIniciais)
    : _id(id), sexo(s), peso(p), idadeMeses(iM),
      qtdFilhos(qtdF), qtdVacinas(qtdV), EmQuarentena(false)
{
    pesoNascimento = ganhoMedioPeso = scoreBase = ajusteDescendentes = scoreGenetico = 0.0;

    if (qtdFilhos > 0 && filhosIniciais != nullptr)
    {
        idFilhos = new std::string[qtdFilhos];

        for (int i = 0; i < qtdFilhos; i++)
        {
            idFilhos[i] = filhosIniciais[i];
        }
    }
    else
    {
        qtdFilhos = 0;
        idFilhos = nullptr;
    }
}

// gets
std::string Animal::getId() const { return _id; }
std::string Animal::getEstadoSaude() const { return estadoSaude; }
std::string Animal::getTemperamento() const { return temperamento; }
char Animal::getSexo() const { return sexo; }
double Animal::getPeso() const { return peso; }
int Animal::getIdadeMeses() const { return idadeMeses; }
int Animal::getQtdFilhos() const { return qtdFilhos; }
int Animal::getQtdvacinas() const { return qtdVacinas; }
bool Animal::getQuarentena() const { return EmQuarentena; }
std::string Animal::getPai() const { return idPai; }
std::string Animal::getMae() const { return idMae; }
std::string *Animal::getFilhos() const { return idFilhos; }
double Animal::getPesoNascimento() const { return pesoNascimento; }
double Animal::getGanhoMedioPeso() const { return ganhoMedioPeso; }
double Animal::getScoreBase() const { return scoreBase; }
double Animal::getAjusteDescendentes() const { return ajusteDescendentes; }
double Animal::getScoreGenetico() const { return scoreGenetico; }

// sets
void Animal::setIdPai(std::string idPai)
{
    this->idPai = idPai;
}

void Animal::setIdMae(std::string idMae)
{
    this->idMae = idMae;
}

void Animal::setPesoNascimento(double p)
{
    if (p < 0.0) throw std::invalid_argument("O peso ao nascer nao pode ser negativo!");
    pesoNascimento = p;
}

void Animal::setAjusteDescendentes(double pontos)
{
    ajusteDescendentes = pontos;
}

//score genetico
bool Animal::obterRazaoProducao([[maybe_unused]] double &razao) const
{
    return false;
}

void Animal::registrarPesagem(double novoPeso, int novaIdadeMeses)
{
    if (novoPeso <= 0.0) throw std::invalid_argument("Peso invalido!");
    if (novaIdadeMeses < 0) throw std::invalid_argument("Idade invalida!");
    peso = novoPeso;
    idadeMeses = novaIdadeMeses;
    if (idadeMeses > 0) {
        double base = (pesoNascimento > 0.0) ? pesoNascimento : PESO_NASC_PADRAO;
        double g = (peso - base) / (idadeMeses * 30.0);
        ganhoMedioPeso = (g > 0.0) ? g : 0.0;
    }
}

int Animal::contarAncestraisSalvos() const
{
    return contarRegistrados(&snapshotPai, GERACOES_AVALIADAS) + contarRegistrados(&snapshotMae, GERACOES_AVALIADAS);
}

// ancestral acima da media do rebanho bonifica, abaixo penaliza e as doses sao menores para avos
double Animal::calcularAjusteAncestrais(double mediaRebanho) const
{
    double ajuste = somarEfeitoAncestrais(&snapshotPai, GERACOES_AVALIADAS, 1.0, mediaRebanho)
                  + somarEfeitoAncestrais(&snapshotMae, GERACOES_AVALIADAS, 1.0, mediaRebanho);
    return limitar(ajuste, -TETO_AJUSTE_ANCESTRAIS, TETO_AJUSTE_ANCESTRAIS);
}

double Animal::calcularScoreBase()
{
    const std::string tipo = getTipo();

    double pesoEsperado = std::min(pesoAdultoReferencia(tipo), PESO_NASC_PADRAO + GANHO_ESPERADO_DIA * 30.0 * idadeMeses);
    double somaPesos = W_PESO;
    double soma = W_PESO * razaoParaPontos(peso / pesoEsperado);

    // ganho medio, se n tem registrado, faz uma estimativa pelo peso e idade
    double ganho = ganhoMedioPeso;
    if (ganho <= 0.0 && idadeMeses > 0) ganho = std::max(0.0, (peso - PESO_NASC_PADRAO) / (idadeMeses * 30.0));
    if (idadeMeses <= IDADE_FIM_CRESCIMENTO && (ganho > 0.0 || idadeMeses > 0)) {
        somaPesos += W_GANHO;
        soma += W_GANHO * razaoParaPontos(ganho / ganhoReferencia(tipo));
    }

    double razaoProd;
    if (obterRazaoProducao(razaoProd)) {
        somaPesos += W_PROD;
        soma += W_PROD * razaoParaPontos(razaoProd);
    }

    scoreBase = limitar(soma / somaPesos, 0.0, 100.0);
    return scoreBase;
}

void Animal::recalcularScoreGenetico(double mediaRebanho)
{
    int n = contarAncestraisSalvos();
    double penalidadeDuvida = 0.0;
    if (n == 0){
        penalidadeDuvida = PENAL_SEM_ANCESTRAIS;
    }else if (n < ANCESTRAIS_MINIMOS){
         penalidadeDuvida = PENAL_POUCOS_ANCESTRAIS;
    }
    
    scoreGenetico = limitar(scoreBase - penalidadeDuvida + calcularAjusteAncestrais(mediaRebanho) + ajusteDescendentes, 0.0, 100.0);
}

double Animal::getFatorPrecoGenetico() const
{
    if (scoreGenetico <= 0.0) return 1.0; // score ainda nao calculado
    return 0.85 + 0.003 * scoreGenetico;
}

void Animal::copiarDadosGeneticosDe(const Animal &outro)
{
    pesoNascimento = outro.pesoNascimento;
    ganhoMedioPeso = outro.ganhoMedioPeso;
    scoreBase = outro.scoreBase;
    ajusteDescendentes = outro.ajusteDescendentes;
    scoreGenetico = outro.scoreGenetico;
}

Animal::~Animal()
{
    delete[] idFilhos;
}

Animal::Animal()
{
    sexo = ' ';
    peso = 0.0;
    idadeMeses = qtdFilhos = qtdVacinas = 0;
    EmQuarentena = false;
    idFilhos = nullptr;
    pesoNascimento = ganhoMedioPeso = scoreBase = ajusteDescendentes = scoreGenetico = 0.0;
}

std::string Animal::formatarTextoSalvar(std::string texto) const
{
    std::string formatado = texto.empty() ? "_" : texto;

    for (size_t i = 0; i < formatado.length(); i++)
    {
        if (formatado[i] == ' ')
            formatado[i] = '_';
    }

    return formatado;
}

std::string Animal::formatarTextoCarregar(std::string texto) const
{
    if (texto == "_")
        return "";

    for (size_t i = 0; i < texto.length(); i++)
    {
        if (texto[i] == '_')
            texto[i] = ' ';
    }

    return texto;
}

void Animal::escreverBase(std::ofstream &out) const
{
    char sexoFmt = (sexo == ' ' || sexo == '\0') ? '_' : sexo;

    out << formatarTextoSalvar(_id) << " "
        << formatarTextoSalvar(estadoSaude) << " "
        << formatarTextoSalvar(temperamento) << " "
        << sexoFmt << " "
        << peso << " "
        << idadeMeses << " "
        << qtdFilhos << " "
        << qtdVacinas << " "
        << EmQuarentena << " "
        << formatarTextoSalvar(idPai) << " "
        << formatarTextoSalvar(idMae);

    for (int i = 0; i < qtdFilhos; i++)
    {
        out << " " << formatarTextoSalvar(idFilhos[i]);
    }

    // score genetico
    out << " " << pesoNascimento << " " << ganhoMedioPeso << " " << scoreBase
        << " " << ajusteDescendentes << " " << scoreGenetico;

    // grava os snapshots completos, incluindo os ancestrais deles
    out << " ";
    snapshotPai.escrever(out);

    out << " ";
    snapshotMae.escrever(out);
}

bool Animal::carregarBase(std::ifstream &in)
{
    if (idFilhos != nullptr)
    {
        delete[] idFilhos;
        idFilhos = nullptr;
    }

    qtdFilhos = 0;

    std::string idLido, saudeLida, tempLido, paiLido, maeLida;
    char sexoLido;
    double pesoLido;
    int idadeLida, qtdFilhosLida, qtdVacinasLida;
    bool quarentenaLida;

    if (!(in >> idLido >> saudeLida >> tempLido >> sexoLido >> pesoLido >> idadeLida >> qtdFilhosLida >> qtdVacinasLida >> quarentenaLida >> paiLido >> maeLida))
    {
        return false;
    }

    _id = formatarTextoCarregar(idLido);
    estadoSaude = formatarTextoCarregar(saudeLida);
    temperamento = formatarTextoCarregar(tempLido);
    sexo = (sexoLido == '_') ? ' ' : sexoLido;
    peso = pesoLido;
    idadeMeses = idadeLida;
    qtdFilhos = qtdFilhosLida;
    qtdVacinas = qtdVacinasLida;
    EmQuarentena = quarentenaLida;
    idPai = formatarTextoCarregar(paiLido);
    idMae = formatarTextoCarregar(maeLida);

    if (qtdFilhos > 0)
    {
        idFilhos = new std::string[qtdFilhos];

        for (int i = 0; i < qtdFilhos; i++)
        {
            std::string filhoLido;

            if (!(in >> filhoLido))
            {
                delete[] idFilhos;
                idFilhos = nullptr;
                qtdFilhos = 0;
                return false;
            }

            idFilhos[i] = formatarTextoCarregar(filhoLido);
        }
    }

    if (!(in >> pesoNascimento >> ganhoMedioPeso >> scoreBase >> ajusteDescendentes >> scoreGenetico))
    {
        delete[] idFilhos;
        idFilhos = nullptr;
        qtdFilhos = 0;
        return false;
    }

    // crrega recursivamente os snapshots dos pais e seus ancestrais
    if (!snapshotPai.ler(in) || !snapshotMae.ler(in))
    {
        if (idFilhos != nullptr)
        {
            delete[] idFilhos;
            idFilhos = nullptr;
            qtdFilhos = 0;
        }

        return false;
    }

    return true;
}