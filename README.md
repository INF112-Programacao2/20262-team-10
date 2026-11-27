# 🐄 Sistema de Gestão - Fazenda Leiteira (FarmManager)

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![UFV](https://img.shields.io/badge/UFV-INF_112-red?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Em_Desenvolvimento-success?style=for-the-badge)

Projeto desenvolvido para a disciplina de **Programação Orientada a Objetos (INF 112)**, do curso de Ciência da Computação da **Universidade Federal de Viçosa (UFV)**.

O **FarmManager** é um sistema completo de gestão zootécnica, financeira, operacional e genealógica para propriedades rurais em **C++17** via terminal (CLI). Mais do que um cadastro de registros, o sistema aplica algoritmos de cálculo nutricional, projeção financeira (*Break-Even*), integração com APIs de cotação em tempo real, avaliação genética multi-geracional e apoio à decisão no manejo do rebanho.

---

## 🧠 O que diferencia o FarmManager de um gerenciador de registros comum?

Um gerenciador de registros tradicional apenas armazena e consulta dados (CRUD). O **FarmManager** processa os dados do rebanho para responder perguntas práticas do produtor e automatizar decisões de manejo:

1. **Planejamento de Gastos e Estoque de Ração (`Area`, `Farm`, `Animal`):**
   * Calcula polimorficamente o consumo diário individual de cada animal conforme peso vivo, categoria (`vaca`, `touro`, `boicorte`, `bezerro`) e fase produtiva (lactação, engorda, desmame/recria).
   * Converte o consumo em **sacos de ração por dia** e projeta a demanda para **30 dias**, separada por tipo (`Ração Engorda`, `Ração Avulsa` e `Ração Recria`), além do custo diário em reais (`R$`).
2. **Análise Financeira e Ponto de Equilíbrio (*Break-Even*):**
   * Cruza a receita diária estimada (produção de leite, ganho de peso com ágio por sexo/idade e rendimento de carcaça) contra o gasto diário com alimentação e manejo.
   * No relatório de *Break-Even* (`emitirRelatorioBreakEvenEVendas`), informa se o caixa já está positivo, **quantos dias faltam no ritmo atual para empatar o caixa**, ou alerta quando a operação diária está no prejuízo.
3. **Algoritmo de Score Genético em $O(N \log N)$ (`Farm::recalcularScoresGeneticos`):**
   * Ordena animais vivos e registros históricos (`SnapshotAncestral`) com **MergeSort** próprio e realiza cruzamentos com **Busca Binária** (sem `std::vector` ou `std::sort`), escalando com eficiência para grandes rebanhos.
   * Calcula o `scoreBase` (0 a 100) ponderando peso atual vs. peso esperado para a idade, ganho médio diário e indicador produtivo (leite na vaca / rendimento de carcaça no boi de corte).
   * Ajusta o `scoreGenetico` percorrendo recursivamente até **3 gerações de ancestrais** (com decaimento de peso por geração), somando a contribuição de **todos os descendentes** ($\ge 6$ meses, inclusive já vendidos/removidos) em relação à média do rebanho e aplicando penalidade quando falta histórico genealógico.
   * Mantém a tabela compartilhada de ancestrais enxuta via coleta/limpeza recursiva (`limpezaDeAncestrais`), preservando apenas registros ligados a animais vivos na fazenda.
4. **Apoio à Decisão Reprodutiva e de Maturidade:**
   * **Bloqueio de consanguinidade (`Vaca::emprenhar`):** impede coberturas entre parentes próximos (mesmo pai, mesma mãe ou relação pai/filha e mãe/filho) e valida idade mínima reprodutiva (15 meses) e status de quarentena.
   * **Recomendação automática na promoção (`Bezerro::recomendarDestinoMaturidade`):** ao atingir 12 meses, fêmeas são direcionadas para `vaca` e machos são recomendados para `touro` (se `scoreGenetico >= 60`) ou `boicorte` (caso contrário).
5. **Controle de Lotação de Pastagens (`Area`):**
   * Calcula a capacidade máxima de cabeças/hectare conforme a qualidade do pasto (`seco` = 0,5; `ruim` = 1,0; `intermediario` = 2,0; `bom` = 3,0; `muito bom` = 4,0) e exibe a taxa de ocupação percentual, alertando sobre superlotação.

---

## 🧭 Fluxo Completo do Sistema e Funcionalidades (`main.cpp`)

Ao executar o sistema, o usuário percorre três níveis de navegação:

### 1. Módulo de Acesso e Usuários (`moduloLogin`)
* **Primeiro acesso:** caso não existam usuários cadastrados, o sistema guia o cadastro do primeiro usuário (login, senha, nome do proprietário e cargo).
* **Login e Cadastro:** permite autenticar usuários existentes ou criar novas contas. As senhas são armazenadas com hash (`User::gerarHash`), nunca em texto puro.
* **Isolamento por proprietário:** cada usuário possui suas próprias fazendas e suas próprias configurações de cotações e preços de ração.

### 2. Seleção de Propriedade (`menuMinhasFazendas`)
Após o login, o proprietário pode:
* Listar todas as suas fazendas cadastradas (exibindo nome, localização e dia atual).
* Abrir/carregar uma fazenda existente.
* Criar uma nova fazenda (já associada ao usuário logado).
* Fazer logout e retornar à tela de login.

### 3. Painel de Gestão da Fazenda
Com a fazenda aberta, o menu principal oferece:

* **`[1] Relatórios` (`menuRelatorios`):**
  * *Relatório Operacional:* total do rebanho por categoria, receita/gasto/lucro diário e consumo diário e mensal (30 dias) de sacos de ração.
  * *Relatório de Caixa e Break-Even:* saldo em caixa, total acumulado em vendas, prejuízo acumulado, lucro diário e estimativa de dias para atingir o ponto de equilíbrio.
* **`[2] Gerenciar Áreas` (`menuGerenciarAreas`):**
  * Listar áreas/pastos (com hectares, estado do pasto, lotação atual/máxima e contagem por tipo de animal), criar nova área, selecionar área ativa e listar seu rebanho, editar estado do pasto ou hectares, consultar resumo de gasto e sacos de ração da área e remover áreas vazias.
* **`[3] Gerenciar Animais` (`menuGerenciarAnimais`):**
  * Adicionar `Vaca`, `Touro`, `Boicorte` ou `Bezerro` na área atual (com geração automática de IDs: `VC`, `TR`, `BC`, `BZ`).
  * Buscar animal por ID (exibe ficha completa e gasto diário individual).
  * Atualizar fase produtiva (lactação e produção de leite, fase de engorda ou desmame).
  * Remover animal (venda, morte ou saída).
  * Consultar genealogia (árvore de ancestrais e lista de filhos, ativos ou históricos).
* **`[4] Movimentação e Reprodução` (`menuMovimentacaoReproducao`):**
  * Transferir um animal ou um lote de animais entre áreas.
  * Realizar cobertura (`Touro x Vaca`) com validação de parentesco e idade.
  * Registrar nascimento (cria o bezerro na área escolhida, vincula pai/mãe e atualiza scores genéticos).
  * Promover bezerros elegíveis ($\ge 12$ meses) com recomendação automática de destino.
  * Recalcular manualmente os scores genéticos de todo o rebanho.
* **`[5] Cotações e Rações` (`menuCotacoesRacoes`):**
  * Visualizar valores atuais (dólar, litro do leite, arroba do boi, arroba da vaca, peso e preço das sacas de ração).
  * **Atualização via APIs + Manual:** busca automaticamente a cotação do dólar (`economia.awesomeapi.com.br`) e da arroba do boi/vaca (`agrodocai.com.br/api/v1/cotacao`) via `curl` com parser próprio de JSON, solicitando em seguida os valores locais de leite e rações.
  * **Edição individual:** permite alterar manualmente qualquer um dos 10 parâmetros de mercado e salvar imediatamente.
* **`[6] Importar Lote (.txt)`:**
  * Importa áreas (`AREA`), animais (`ANIMAL`) e registros históricos (`ANCESTRAL`) a partir de arquivos de texto estruturados, recalculando os scores genéticos e salvando ao final. Também é possível passar o caminho do arquivo diretamente como argumento na linha de comando (`./bin/main arquivo.txt`).
* **`[7] Salvar Tudo Agora` / `[8] Trocar de Fazenda` / `[0] Salvar e Voltar ao Login`:**
  * Persistência explícita e automática dos dados da fazenda e do usuário.

---

## 💾 Persistência de Dados

Todos os dados do sistema são salvos localmente no diretório `./data/` em arquivos `.txt` estruturados, sem dependência de banco de dados externo:

* `./data/usuarios.txt` — credenciais (hash da senha), perfil, cotações e preços/pesos de ração por usuário.
* `./data/fazendas.txt` — metadados das fazendas, saldos financeiros, dia da simulação e próximo ID global.
* `./data/areas.txt` — áreas/pastos de cada fazenda e vínculo dos animais alocados.
* `./data/vacas.txt`, `./data/touros.txt`, `./data/boicortes.txt`, `./data/bezerros.txt` — dados específicos de cada categoria animal.
* `./data/ancestrais.txt` — tabela compartilhada de `SnapshotAncestral` para consulta genealógica e cálculo genético mesmo após venda/remoção de parentes.

**Como funciona o salvamento:**
* Alterações feitas nos menus de áreas, animais e movimentação/reprodução marcam a sessão para **salvamento automático** ao retornar ao painel principal (inclusive em caso de exceção tratada após alteração).
* Importações em lote, edições de cotações, troca de fazenda e logout também salvam os dados automaticamente.
* A gravação em disco utiliza arquivos temporários (`*_temp.txt`) antes de substituir o arquivo final, evitando corrupção em caso de falha durante a escrita.

---

## 🏛️ Arquitetura e Restrições Acadêmicas

O projeto foi construído sob restrições acadêmicas de POO em C++17 com foco no domínio de ponteiros e memória:

* **Sem coleções da STL (`std::vector`, `std::list`, `std::map`):** toda agregação utiliza **arrays dinâmicos manuais** (`Animal**`, `Area**`, `SnapshotAncestral*`, `std::string*`, `SacaRacao**`, `RegistroSanitario**`) com realocação e liberação rigorosa nos destrutores.
* **Polimorfismo:** classe abstrata `Animal` com métodos virtuais puros especializados em `Vaca`, `Touro`, `Bezerro` e `Boicorte`, além de especialização de `Area` em `AreaQuarentena`.
* **Módulos adicionais no repositório:** classes para controle sanitário e período de carência (`Medicamento`, `RegistroSanitario`, `HistoricoSanitario`), isolamento (`AreaQuarentena`) e registro de baixas (`RegistroBaixa`).

---

## 💻 Diagrama Arquitetural de Classes

<div align="center">
  <a href="docs/Diagrama_Classes_FarmManager.pdf" target="_blank">
    <img src="docs/diagrama_preview.png" alt="Diagrama de Classes Mestre (PDF)" width="850">
  </a>
  <br>
  <p>
    <i>Clique na miniatura acima para abrir a modelagem UML, ou <a href="docs/Diagrama_Classes_FarmManager.pdf">baixe o arquivo PDF aqui</a>.</i>
  </p>
</div>

---

## 🛠️ Como Compilar, Executar e Testar

### Pré-requisitos
* Compilador `g++` (GCC com suporte a C++17) e `make`.
* `curl` instalado no sistema (opcional, apenas para atualização automática das cotações via API).
* Python 3 (opcional, apenas para gerar arquivos de teste em massa com `geradorTeste.py`).

### Compilação e Execução
No diretório raiz do projeto:

```bash
# Compilar o projeto (gera ./bin/main)
make

# Executar normalmente
./bin/main

# Ou executar já importando um arquivo de lote (.txt) após abrir a fazenda
./bin/main teste_gigante.txt
```

Para remover os arquivos objeto (`build/`) e o executável (`bin/`) antes de uma recompilação limpa:

```bash
make clean
```

---
*Documentação atualizada conforme o código-fonte do repositório — INF 112, Universidade Federal de Viçosa (UFV).*
