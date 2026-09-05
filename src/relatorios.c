#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/util.h"

// =======================================================
//                  RELATÓRIO FINANCEIRO
// =======================================================

void exibirRelatorioFinanceiro(const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    float faturamentoVendas = 0.0;
    float faturamentoLocacoes = 0.0;

    for (int i = 0; i < totalVendas; i++) faturamentoVendas += vendas[i].veiculo.valor;
    for (int i = 0; i < totalLocacoes; i++) {
        if (locacoes[i].status == 0) faturamentoLocacoes += locacoes[i].valorTotal;
    }

    printf("\n====================================");
    printf("\n    RELATORIO FINANCEIRO UNIFICADO  ");
    printf("\n====================================");
    printf("\nTotal em Vendas:   R$ %.2f", faturamentoVendas);
    printf("\nTotal em Locacoes: R$ %.2f", faturamentoLocacoes);
    printf("\n------------------------------------");
    printf("\nFATURAMENTO GERAL: R$ %.2f", faturamentoVendas + faturamentoLocacoes);
    printf("\n====================================\n");
}
