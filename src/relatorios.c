#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include "../include/relatorios.h"

void relatorioVendasLocacoesPorPeriodo(const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    char periodo[20];
    printf("\nDigite o periodo para buscar (Ex: '2026', '03/2026' ou '15/03/2026'): ");
    fgets(periodo, sizeof(periodo), stdin);
    periodo[strcspn(periodo, "\n")] = '\0';

    printf("\n--- Vendas Encontradas no Periodo ---\n");
    for (int i = 0; i < totalVendas; i++) {
        if (strstr(vendas[i].dataVenda, periodo) != NULL) {
            printf("ID: %d | Data: %s | Placa: %s | Valor: R$%.2f\n",
                   vendas[i].idVenda, vendas[i].dataVenda, vendas[i].veiculo.placa, vendas[i].valorFinal);
        }
    }

    printf("\n--- Locacoes Encontradas no Periodo ---\n");
    for (int i = 0; i < totalLocacoes; i++) {
        if (strstr(locacoes[i].dataInicio, periodo) != NULL) {
            printf("ID: %d | Data Inicio: %s | Placa: %s | Total: R$%.2f\n",
                   locacoes[i].idLocacao, locacoes[i].dataInicio, locacoes[i].placaVeiculo, locacoes[i].valorTotal);
        }
    }
}

void resumoFinanceiroGeral(const Veiculo veiculos[], int totalVeiculos, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes, const Manutencao manutencoes[], int totalManutencoes) {
    float investido = 0.0;
    float arrecadadoVendas = 0.0;
    float arrecadadoLocacoes = 0.0;
    float despesasManutencao = 0.0;

    for (int i = 0; i < totalVeiculos; i++) investido += veiculos[i].valorCompra;
    for (int i = 0; i < totalVendas; i++) arrecadadoVendas += vendas[i].valorFinal;
    for (int i = 0; i < totalLocacoes; i++) {
        if (locacoes[i].status == 0) arrecadadoLocacoes += locacoes[i].valorTotal;
    }
    for (int i = 0; i < totalManutencoes; i++) despesasManutencao += manutencoes[i].custo;

    float faturamentoTotal = arrecadadoVendas + arrecadadoLocacoes;
    float lucroLiquido = faturamentoTotal - investido - despesasManutencao;

    printf("\n====================================");
    printf("\n       RESUMO FINANCEIRO GERAL      ");
    printf("\n====================================");
    printf("\nTotal Investido em Veiculos: R$ %.2f", investido);
    printf("\nFaturamento em Vendas:       R$ %.2f", arrecadadoVendas);
    printf("\nFaturamento em Locacoes:     R$ %.2f", arrecadadoLocacoes);
    printf("\nGastos com Manutencao:       R$ %.2f", despesasManutencao);
    printf("\n------------------------------------");
    printf("\nLUCRO / PREJUIZO LIQUIDO:    R$ %.2f", lucroLiquido);
    printf("\n====================================\n");
}

void relatorioVeiculosVendidos(const Venda vendas[], int totalVendas) {
    printf("\n=== RELATORIO DE VEICULOS VENDIDOS ===\n");
    for (int i = 0; i < totalVendas; i++) {
        printf("Placa: %s | Modelo: %s %s | Comprador: %s | Valor: R$%.2f\n",
               vendas[i].veiculo.placa, vendas[i].veiculo.marca, 
               vendas[i].veiculo.modelo, vendas[i].cliente.nome, vendas[i].valorFinal);
    }
}

void relatorioDespesasManutencao(const Manutencao manutencoes[], int totalManutencoes) {
    float total = 0.0;
    printf("\n=== DESPESAS COM MANUTENCAO ===\n");
    for (int i = 0; i < totalManutencoes; i++) {
        printf("ID: %d | Placa: %s | Servico: %s | Custo: R$%.2f\n",
               manutencoes[i].idManutencao, manutencoes[i].placaVeiculo,
               manutencoes[i].descricao, manutencoes[i].custo);
        total += manutencoes[i].custo;
    }
    printf("Total Gasto com Manutencoes: R$ %.2f\n", total);
}

void menuRelatorios(const Veiculo veiculos[], int totalVeiculos, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes, const Manutencao manutencoes[], int totalManutencoes) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|          MENU RELATORIOS         |");
        printf("\n====================================");
        printf("\n| 1 - Vendas/Locacoes por Periodo  |");
        printf("\n| 2 - Resumo Financeiro Geral      |");
        printf("\n| 3 - Veiculos Vendidos            |");
        printf("\n| 4 - Despesas com Manutencao      |");
        printf("\n| 0 - Sair                         |");
        printf("\n====================================");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) relatorioVendasLocacoesPorPeriodo(vendas, totalVendas, locacoes, totalLocacoes);
        else if (opcao == 2) resumoFinanceiroGeral(veiculos, totalVeiculos, vendas, totalVendas, locacoes, totalLocacoes, manutencoes, totalManutencoes);
        else if (opcao == 3) relatorioVeiculosVendidos(vendas, totalVendas);
        else if (opcao == 4) relatorioDespesasManutencao(manutencoes, totalManutencoes);
    } while (opcao != 0);
}