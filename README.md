# Locar — Sistema de Gestão para Concessionária e Locadora de Veículos

O **Locar** é um sistema de gestão desenvolvido em **linguagem C**, com funcionamento por meio de uma **interface de linha de comando (CLI, Command Line Interface)**. O projeto foi desenvolvido como parte das atividades do segundo semestre da disciplina: **Desenvolvimento de Sistemas em Linguagem C**, tendo como objetivo representar, de forma organizada, as principais operações realizadas no cotidiano de uma concessionária que também atua no segmento de locação de veículos.

O sistema permite realizar o cadastro e o gerenciamento de clientes e veículos, registrar vendas à vista ou financiadas, controlar locações, acompanhar manutenções e consultar informações financeiras da empresa.

Para garantir a continuidade das informações entre diferentes execuções do programa, os dados são armazenados em arquivos **CSV (dados organizados em campos delimitados)**. Dessa forma, o sistema não depende exclusivamente da memória do computador para manter os registros realizados pelo usuário.

## Funcionalidades

### Clientes

O módulo de clientes é responsável pelo cadastro, consulta, alteração e remoção das informações dos clientes.

Durante o cadastro, são realizadas validações para garantir a consistência dos dados informados, incluindo:

* **CPF**, com verificação dos dígitos verificadores e prevenção de duplicidade;
* **CNH**, composta por 11 dígitos numéricos e sem duplicidade;
* Nome, permitindo somente letras;
* Telefone;
* E-mail;
* CEP;
* Unidade Federativa (UF);
* Data de nascimento, considerando idade mínima de 18 anos.

O ano de nascimento não é solicitado separadamente, pois é obtido automaticamente a partir da **Data de Nascimento** informada pelo usuário.

O sistema também disponibiliza:

* Busca de clientes por nome ou CPF;
* Edição dos dados cadastrados;
* Listagem dos clientes;
* Remoção de clientes.

A remoção de um cliente é bloqueada quando existem vendas ou locações associadas ao seu cadastro, preservando a relação entre os registros existentes no sistema.

### Veículos

O módulo de veículos permite cadastrar e consultar os veículos disponíveis no sistema, realizando validações sobre suas principais informações.

Entre os dados considerados estão:

* Placa;
* Ano de fabricação;
* Valor de compra;
* Demais valores relacionados ao veículo.

O **valor de venda não é definido durante o cadastro do veículo**. Esse valor é estabelecido posteriormente, no momento em que a venda é registrada, permitindo que o preço seja definido de acordo com a negociação realizada.

O sistema também realiza um **cálculo de depreciação**, utilizado para estimar o valor de mercado do veículo. Essa estimativa considera:

* O valor de compra do veículo;
* A idade do veículo;
* Uma redução de 10% ao ano;
* Um limite mínimo correspondente a 30% do valor originalmente pago.

O valor de mercado calculado é apresentado nas listagens e utilizado como referência para a composição das informações financeiras do estoque.

Também é possível realizar buscas de veículos por:

* Modelo;
* Marca;
* Placa;
* Faixa de valor.

### Vendas

O módulo de vendas permite registrar a comercialização dos veículos cadastrados no sistema.

Quando o CPF informado durante uma venda não pertence a um cliente cadastrado, o sistema oferece a possibilidade de realizar o cadastro do cliente naquele momento. Caso o usuário opte por não realizar o cadastro, é possível retornar ao menu e cancelar a operação de venda.

As vendas podem ser realizadas nas seguintes modalidades:

* **À vista**;
* **Financiada**.

No caso de financiamento, são apresentadas as seguintes opções de parcelamento:

* 12 parcelas;
* 24 parcelas;
* 36 parcelas;
* 48 parcelas;
* 60 parcelas.

Cada opção apresenta previamente o valor estimado da parcela e sua situação, indicada pelos estados `[APROVADO]` ou `[REPROVADO]`.

Para que uma opção de financiamento seja aprovada, o valor da parcela não pode ultrapassar **30% da renda mensal do cliente**.

Caso nenhuma das opções disponíveis seja compatível com a renda informada, o usuário pode cancelar o financiamento e prosseguir com a venda na modalidade à vista.

### Locações

O módulo de locações permite controlar o processo de aluguel dos veículos, desde seu início até sua finalização.

O processo de identificação do cliente segue a mesma lógica utilizada nas vendas. Caso o CPF informado não esteja cadastrado, o sistema permite realizar o cadastro do cliente no próprio fluxo da locação ou cancelar a operação.

São contempladas as seguintes operações:

* Início de uma locação;
* Extensão de uma locação existente;
* Finalização da locação;
* Registro da quilometragem final;
* Aplicação de taxas relacionadas a avarias ou multas;
* Definição da forma de pagamento;
* Consulta ao histórico de locações.

### Manutenção

O módulo de manutenção permite registrar e finalizar serviços realizados nos veículos.

Quando um veículo se encontra com uma manutenção em andamento, seu estado é alterado para **"Em Manutenção"**. Nessa situação, o veículo fica temporariamente indisponível para novas vendas ou locações.

Após a finalização da manutenção, o veículo pode voltar a ser utilizado nas demais operações do sistema.

### Relatórios

O módulo de relatórios apresenta informações consolidadas sobre as atividades da concessionária e locadora.

Entre os dados disponibilizados estão:

* Investimento realizado;
* Faturamento;
* Lucro ou prejuízo;
* Valor de mercado do estoque;
* Vendas realizadas em determinado período;
* Veículos vendidos;
* Despesas relacionadas às manutenções.

Além da apresentação das informações na tela, o sistema possui uma função de **exportação para TXT (arquivo de texto simples)**.

Quando solicitada, essa função gera o arquivo:

```text
data/relatorio.txt
```

O relatório contém, de forma organizada e adequada para impressão ou envio, informações referentes a:

* Resumo financeiro;
* Estoque;
* Vendas;
* Locações;
* Manutenções.

## Persistência de dados

A **persistência de dados** corresponde ao armazenamento das informações para que elas permaneçam disponíveis mesmo após o encerramento do programa.

No Locar, os dados são armazenados automaticamente em arquivos `.csv` localizados na pasta `data/`.

A estrutura utilizada é:

```text
data/
├── clientes.csv
├── veiculos.csv
├── vendas.csv
├── locacoes.csv
├── manutencoes.csv
└── relatorio.txt   (gerado sob demanda pelo menu Relatórios)
```

Os arquivos `.csv` são utilizados para armazenar os registros dos diferentes módulos do sistema, enquanto o arquivo `relatorio.txt` é criado somente quando a exportação de relatório é solicitada.

Ao iniciar o programa, os dados existentes são carregados automaticamente. Durante a utilização do sistema, as informações são salvas a cada retorno ao menu principal e também é realizado um salvamento final no momento do encerramento.

Dessa forma, não é necessário realizar procedimentos manuais para preservar o progresso das operações realizadas.

## Estrutura do projeto

## Ajustes de legibilidade e manutencao

Esta versao preserva as funcionalidades e regras de negocio originais do projeto.
Foram aplicados somente ajustes estruturais necessarios para que a organizacao
proposta no arquivo principal funcione corretamente:

* O estado do sistema permanece reunido em `ConcessionariaDB`, evitando que o
  menu principal manipule diversos vetores e contadores diretamente.
* Foram criadas funcoes internas de adaptacao para carregar, salvar e encaminhar
  os dados aos modulos. Assim, os modulos existentes nao precisaram ser
  reescritos nem tiveram suas regras alteradas.
* A alocacao inicial passou a liberar a memoria ja reservada se alguma das
  alocacoes falhar.
* O menu principal agora identifica uma opcao nao numerica e retorna ao menu,
  sem repetir a leitura indefinidamente.

Os comentarios marcados como `ALTERACAO` no arquivo `main.c` indicam a relacao
entre o codigo anterior e o comportamento refatorado.

O projeto foi organizado em diferentes arquivos e módulos, de modo que cada parte do sistema possua uma responsabilidade específica. Essa organização facilita a manutenção e a compreensão do código.

```text
.
├── main.c                 # Ponto de entrada e menu principal
├── include/               # Arquivos de cabeçalho (.h) dos módulos
├── src/
│   ├── clientes.c         # Cadastro, edição, busca e remoção de clientes
│   ├── veiculos.c         # Cadastro de veículos e cálculo de depreciação
│   ├── vendas.c           # Vendas à vista e financiadas
│   ├── locacoes.c         # Controle das locações de veículos
│   ├── manutencao.c       # Controle das manutenções
│   ├── relatorios.c       # Relatórios em tela e exportação em TXT
│   └── util.c             # Funções compartilhadas, validações, leitura de
│                           # entradas e operações auxiliares com arquivos
├── data/
│   └── dados.c            # Leitura e escrita dos arquivos CSV
└── CMakeLists.txt
```

O arquivo `main.c` concentra o ponto de entrada do programa e a apresentação do menu principal. Os demais arquivos são distribuídos de acordo com a área de responsabilidade correspondente.

Os arquivos `.h`, localizados na pasta `include/`, contêm as declarações necessárias para que os diferentes módulos possam utilizar funções e estruturas definidas em outros arquivos.

## Como compilar

O projeto pode ser compilado utilizando o **CMake**, uma ferramenta utilizada para configurar e automatizar o processo de compilação, ou diretamente por meio do **GCC**, compilador utilizado para programas escritos em C.

### Compilação com CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Compilação diretamente com GCC

```bash
gcc -std=c99 -Wall -Wextra -Iinclude -o Locar \
    main.c src/clientes.c src/locacoes.c src/relatorios.c \
    src/veiculos.c src/vendas.c src/util.c data/dados.c src/manutencao.c
```

O executável deve ser iniciado a partir da raiz do projeto ou de uma pasta na qual possua permissão para criar e utilizar a subpasta `data/`.

Essa condição é necessária para que os arquivos responsáveis pelo armazenamento dos dados possam ser encontrados e atualizados corretamente.

## Validações implementadas

O sistema realiza validações nos principais dados fornecidos pelo usuário, buscando reduzir inconsistências nos registros armazenados.

| Campo                                     | Regra                                                                        |
| ----------------------------------------- | ---------------------------------------------------------------------------- |
| CPF                                       | 11 dígitos, com dígitos verificadores válidos e sem duplicidade              |
| CNH                                       | 11 dígitos numéricos e sem duplicidade                                       |
| Nome / Estado Civil / Profissão / Cidade  | Somente letras e espaços                                                     |
| Datas                                     | Formato `DD/MM/AAAA`, com dia, mês e ano coerentes, incluindo anos bissextos |
| Data de Nascimento                        | O cliente deve possuir no mínimo 18 anos                                     |
| Telefone                                  | 10 ou 11 dígitos numéricos, incluindo o DDD                                  |
| E-mail                                    | Formato `usuario@dominio.extensao`                                           |
| CEP                                       | 8 dígitos numéricos                                                          |
| UF                                        | 2 letras                                                                     |
| Placa                                     | 7 caracteres alfanuméricos                                                   |
| Valores monetários / quilometragem / dias | Valores maiores que zero ou não negativos, conforme a finalidade do campo    |

Essas validações são realizadas antes que os dados sejam efetivamente registrados, contribuindo para a integridade das informações utilizadas pelas demais funcionalidades do sistema.

## Observações técnicas

Algumas decisões foram adotadas durante o desenvolvimento para garantir o funcionamento adequado do armazenamento e da leitura dos dados.

* O mecanismo de leitura dos arquivos CSV foi desenvolvido manualmente, em vez da utilização direta de `sscanf` com o formato `%[^;]`. Essa decisão permite tratar corretamente campos vazios, como um campo de "Complemento" que não tenha sido preenchido, evitando que os valores dos campos seguintes sejam interpretados de forma incorreta.

* Novos campos, como **CNH** e **forma de pagamento**, são adicionados ao final de cada linha dos arquivos CSV. Essa estratégia busca manter a compatibilidade com arquivos que tenham sido armazenados por versões anteriores do sistema.

* O caractere `;` é utilizado como delimitador nos arquivos CSV. Dessa forma, caso o usuário informe esse caractere em algum campo de texto, ele é automaticamente substituído por um espaço, evitando que a estrutura dos arquivos seja comprometida.

Essas decisões fazem parte da organização interna do sistema e foram adotadas com o objetivo de preservar a consistência dos dados, facilitar sua manutenção e permitir a evolução do projeto sem comprometer os registros já existentes.
