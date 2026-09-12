#include <stdio.h>
#include <stdlib.h>

#include "modelos.h"
#include "clientes.h"
#include "locacoes.h"
#include "vendas.h"
#include "veiculos.h"
#include "manutencao.h"
#include "relatorios.h"
#include "util.h"
#include "dados.h"

// 1. Criação do Estado Global do Sistema
typedef struct {
    Cliente *clientes;
    int totalClientes;
    int capClientes;

    Veiculo *veiculos;
    int totalVeiculos;
    int capVeiculos;

    Venda *vendas;
    int totalVendas;
    int capVendas;

    Locacao *locacoes;
    int totalLocacoes;
    int capLocacoes;

    Manutencao *manutencoes;
    int totalManutencoes;
    int capManutencoes;
} ConcessionariaDB;

static void liberarDB(ConcessionariaDB *db);

/*
 * ALTERACAO DA REFATORACAO:
 * Antes, cada chamada do menu principal precisava receber varios vetores,
 * totais e capacidades. A estrutura ConcessionariaDB concentra esse estado.
 * Os modulos continuam com suas assinaturas originais para nao alterar a
 * regra de negocio; as funcoes abaixo fazem somente a adaptacao de dados.
 */
static void carregarDados(ConcessionariaDB *db) {
    carregarDadosCSV(&db->clientes, &db->totalClientes, &db->capClientes,
                     &db->veiculos, &db->totalVeiculos, &db->capVeiculos,
                     &db->vendas, &db->totalVendas, &db->capVendas,
                     &db->locacoes, &db->totalLocacoes, &db->capLocacoes,
                     &db->manutencoes, &db->totalManutencoes, &db->capManutencoes);
}

static void salvarDados(const ConcessionariaDB *db) {
    salvarDadosCSV(db->clientes, db->totalClientes, db->veiculos, db->totalVeiculos,
                   db->vendas, db->totalVendas, db->locacoes, db->totalLocacoes,
                   db->manutencoes, db->totalManutencoes);
}

static void executarOpcao(ConcessionariaDB *db, int opcao) {
    switch (opcao) {
        case 1:
            menuClientes(&db->clientes, &db->totalClientes, &db->capClientes,
                         db->vendas, db->totalVendas, db->locacoes, db->totalLocacoes);
            break;
        case 2:
            menuVeiculos(&db->veiculos, &db->totalVeiculos, &db->capVeiculos,
                          db->vendas, db->totalVendas, db->locacoes, db->totalLocacoes);
            break;
        case 3:
            menuVendas(&db->veiculos, &db->totalVeiculos, &db->clientes,
                       &db->totalClientes, &db->capClientes, &db->vendas,
                       &db->totalVendas, &db->capVendas);
            break;
        case 4:
            menuLocacoes(&db->veiculos, db->totalVeiculos, &db->clientes,
                          &db->totalClientes, &db->capClientes, &db->locacoes,
                          &db->totalLocacoes, &db->capLocacoes);
            break;
        case 5:
            menuManutencao(&db->veiculos, db->totalVeiculos, &db->manutencoes,
                            &db->totalManutencoes, &db->capManutencoes);
            break;
        case 6:
            menuRelatorios(db->veiculos, db->totalVeiculos, db->vendas,
                           db->totalVendas, db->locacoes, db->totalLocacoes,
                           db->manutencoes, db->totalManutencoes);
            break;
        case 0:
            printf("\nEncerrando e salvando dados...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida!\n");
            break;
    }
}

// 2. Isolamento da logica de alocacao
int inicializarDB(ConcessionariaDB *db) {
    db->capClientes = 2; db->totalClientes = 0;
    db->clientes = malloc(db->capClientes * sizeof(Cliente));

    db->capVeiculos = 2; db->totalVeiculos = 0;
    db->veiculos = malloc(db->capVeiculos * sizeof(Veiculo));

    db->capVendas = 2; db->totalVendas = 0;
    db->vendas = malloc(db->capVendas * sizeof(Venda));

    db->capLocacoes = 2; db->totalLocacoes = 0;
    db->locacoes = malloc(db->capLocacoes * sizeof(Locacao));

    db->capManutencoes = 2; db->totalManutencoes = 0;
    db->manutencoes = malloc(db->capManutencoes * sizeof(Manutencao));

    if (!db->clientes || !db->veiculos || !db->vendas || !db->locacoes || !db->manutencoes) {
        // ALTERACAO: libera qualquer alocacao parcial antes de informar erro.
        liberarDB(db);
        return 0;
    }
    return 1; // 1 significa sucesso
}

// 3. Isolamento da lógica de limpeza de memória
static void liberarDB(ConcessionariaDB *db) {
    free(db->clientes);
    free(db->veiculos);
    free(db->vendas);
    free(db->locacoes);
    free(db->manutencoes);

    // ALTERACAO: evita ponteiros pendentes caso a funcao seja reutilizada.
    db->clientes = NULL;
    db->veiculos = NULL;
    db->vendas = NULL;
    db->locacoes = NULL;
    db->manutencoes = NULL;
}

// Função Principal (Muito mais limpa e legível)
int main(void) {
    ConcessionariaDB db;

    if (!inicializarDB(&db)) {
        fprintf(stderr, "Erro na alocacao de memoria.\n");
        return 1;
    }

    carregarDados(&db);

    int opcao;
    do {
        printf("\n====================================");
        printf("\n|      SISTEMA DA CONCESSIONARIA   |");
        printf("\n====================================");
        printf("\n| 1 - Clientes                     |");
        printf("\n| 2 - Veiculos                     |");
        printf("\n| 3 - Vendas                       |");
        printf("\n| 4 - Locacoes                     |");
        printf("\n| 5 - Manutencao                   |");
        printf("\n| 6 - Relatorios                   |");
        printf("\n| 0 - Sair                         |");
        printf("\n====================================");
        printf("\nDigite a opcao: ");
        if (scanf("%d", &opcao) != 1) {
            // ALTERACAO: uma entrada textual nao deixa o menu em repeticao infinita.
            printf("\n[ERRO] Digite apenas o numero da opcao.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        executarOpcao(&db, opcao);

        // Grava tudo em CSV assim que o usuario volta ao menu principal
        if (opcao != 0) {
            salvarDados(&db);
        }

    } while (opcao != 0);

    // Salvamento final
    salvarDados(&db);

    // Limpeza encapsulada
    liberarDB(&db);

    printf("\nSistema encerrado com sucesso.\n");
    return 0;
}
