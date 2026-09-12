# Locar — Sistema de Gestão para Concessionária/Locadora de Veículos

Sistema de linha de comando (CLI) em **C**, desenvolvido para o segundo semestre da disciplina, para gerenciar o dia a dia de uma concessionária que também trabalha com locação de veículos: cadastro de clientes e veículos, vendas (à vista ou financiadas), locações, manutenção e relatórios financeiros — tudo com persistência em arquivos CSV.

## Funcionalidades

### Clientes
- Cadastro com validação de **CPF** (dígitos verificadores reais), **CNH** (11 dígitos, sem duplicidade), nome (somente letras), telefone, e-mail, CEP, UF e data de nascimento (idade mínima de 18 anos).
- O ano de nascimento não é mais perguntado separadamente: é derivado automaticamente da Data de Nascimento informada.
- Busca por nome ou CPF, edição, listagem e remoção (bloqueada se o cliente tiver vendas ou locações associadas).

### Veículos
- Cadastro com validação de placa, ano de fabricação e valores.
- O **valor de venda não é mais definido no cadastro** — ele é negociado no momento da venda.
- **Cálculo de depreciação**: o "valor de mercado" exibido nas listagens é estimado automaticamente a partir do valor de compra e da idade do veículo (10% ao ano, com piso de 30% do valor pago).
- Busca por modelo, marca, placa ou faixa de valor.

### Vendas
- Se o CPF informado não pertence a um cliente cadastrado, o sistema pergunta se deseja cadastrar um novo cliente ali mesmo ou voltar ao menu (a venda é cancelada nesse caso).
- **Forma de pagamento**: à vista ou financiado.
  - No financiamento, são oferecidas as opções de **12, 24, 36, 48 ou 60 parcelas**, cada uma já exibida com o valor da parcela e o status `[APROVADO]`/`[REPROVADO]`.
  - O financiamento só é aprovado se a parcela não ultrapassar **30% da renda mensal do cliente**. Se nenhuma opção couber no orçamento, é possível cancelar o financiamento e prosseguir com a venda à vista.

### Locações
- Mesmo fluxo de confirmação de cadastro de cliente usado nas vendas (cadastrar agora ou cancelar a locação).
- Início, extensão, finalização (com quilometragem final, taxas de avaria/multa e forma de pagamento) e histórico.

### Manutenção
- Registro e finalização de manutenções, bloqueando o veículo para vendas/locações enquanto estiver "Em Manutenção".

### Relatórios
- Resumo financeiro geral (investimento, faturamento, lucro/prejuízo, valor de mercado do estoque).
- Vendas por período, veículos vendidos, despesas com manutenção.
- **Exportação para TXT**: gera `data/relatorio.txt`, um relatório completo e formatado (resumo financeiro, estoque, vendas, locações e manutenções) pronto para impressão ou envio.

## Persistência de dados

Todos os dados são salvos automaticamente em arquivos `.csv` dentro da pasta `data/`:

```
data/
├── clientes.csv
├── veiculos.csv
├── vendas.csv
├── locacoes.csv
├── manutencoes.csv
└── relatorio.txt   (gerado sob demanda pelo menu Relatórios)
```

Os dados são carregados automaticamente ao abrir o programa e salvos a cada retorno ao menu principal, além de um salvamento final ao sair — não é necessário fazer nada manualmente para não perder o progresso.

## Estrutura do projeto

```
.
├── main.c                 # Ponto de entrada e menu principal
├── include/                # Cabeçalhos (.h) de cada módulo
├── src/
│   ├── clientes.c          # Cadastro, edição, busca e remoção de clientes
│   ├── veiculos.c          # Cadastro de veículos e cálculo de depreciação
│   ├── vendas.c             # Vendas à vista/financiadas
│   ├── locacoes.c           # Locações de veículos
│   ├── manutencao.c         # Controle de manutenções
│   ├── relatorios.c         # Relatórios em tela e exportação em TXT
│   └── util.c                # Funções compartilhadas: validações, leitura de
│                                entrada e utilitários de arquivo
├── data/
│   └── dados.c              # Leitura e escrita dos arquivos CSV
└── CMakeLists.txt
```

## Como compilar

### Com CMake
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Com GCC diretamente
```bash
gcc -std=c99 -Wall -Wextra -Iinclude -o Locar \
    main.c src/clientes.c src/locacoes.c src/relatorios.c \
    src/veiculos.c src/vendas.c src/util.c data/dados.c src/manutencao.c
```

O executável deve ser rodado a partir da raiz do projeto (ou de uma pasta onde ele tenha permissão de criar a subpasta `data/`), para que a persistência funcione corretamente.

## Validações implementadas

| Campo | Regra |
|---|---|
| CPF | 11 dígitos com dígitos verificadores válidos, sem duplicidade |
| CNH | 11 dígitos numéricos, sem duplicidade |
| Nome / Estado Civil / Profissão / Cidade | Somente letras e espaços |
| Datas | Formato `DD/MM/AAAA`, com dia/mês/ano coerentes (incluindo anos bissextos) |
| Data de Nascimento | Cliente deve ter no mínimo 18 anos |
| Telefone | 10 ou 11 dígitos numéricos (com DDD) |
| E-mail | Formato `usuario@dominio.extensao` |
| CEP | 8 dígitos numéricos |
| UF | 2 letras |
| Placa | 7 caracteres alfanuméricos |
| Valores monetários / quilometragem / dias | Sempre maiores que zero (ou não negativos, conforme o campo) |

## Observações técnicas

- O parser de CSV foi escrito manualmente (em vez de `sscanf` com `%[^;]`) para lidar corretamente com campos vazios (como um "Complemento" em branco), que quebrariam a leitura dos campos seguintes.
- Novos campos (como CNH e forma de pagamento) são sempre adicionados ao final de cada linha do CSV, para manter compatibilidade com arquivos salvos por versões anteriores.
- O ';' é o delimitador dos arquivos CSV — caso o usuário digite um ';' em algum campo de texto, ele é automaticamente substituído por espaço para não corromper o arquivo.
