#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include "../include/veiculos.h"
#include "../include/relatorios.h"

void relatorioVendasLocacoesPorPeriodo(const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    char periodo[20];
    printf("\nDigite o periodo para buscar (Ex: '2026', '03/2026' ou '15/03/2026'): ");
    lerLinha(periodo, sizeof(periodo));

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

static float valorEstoqueDisponivel(const Veiculo veiculos[], int totalVeiculos) {
    float total = 0.0f;
    for (int i = 0; i < totalVeiculos; i++) {
        if (veiculos[i].status == 0) total += calcularValorDepreciado(&veiculos[i]);
    }
    return total;
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
    float valorEstoque = valorEstoqueDisponivel(veiculos, totalVeiculos);

    printf("\n====================================");
    printf("\n       RESUMO FINANCEIRO GERAL      ");
    printf("\n====================================");
    printf("\nTotal Investido em Veiculos: R$ %.2f", investido);
    printf("\nFaturamento em Vendas:       R$ %.2f", arrecadadoVendas);
    printf("\nFaturamento em Locacoes:     R$ %.2f", arrecadadoLocacoes);
    printf("\nGastos com Manutencao:       R$ %.2f", despesasManutencao);
    printf("\n------------------------------------");
    printf("\nLUCRO / PREJUIZO LIQUIDO:    R$ %.2f", lucroLiquido);
    printf("\n------------------------------------");
    printf("\nValor de Mercado do Estoque");
    printf("\nDisponivel (com depreciacao): R$ %.2f", valorEstoque);
    printf("\n====================================\n");
}

void relatorioVeiculosVendidos(const Venda vendas[], int totalVendas) {
    printf("\n=== RELATORIO DE VEICULOS VENDIDOS ===\n");
    for (int i = 0; i < totalVendas; i++) {
        char pagamento[50];
        if (vendas[i].formaPagamento == 2) {
            snprintf(pagamento, sizeof(pagamento), "Financiado %dx de R$%.2f",
                     vendas[i].numParcelas, vendas[i].valorParcela);
        } else {
            snprintf(pagamento, sizeof(pagamento), "A Vista");
        }
        printf("Placa: %s | Modelo: %s %s | Comprador: %s | Valor: R$%.2f | Pagamento: %s\n",
               vendas[i].veiculo.placa, vendas[i].veiculo.marca, 
               vendas[i].veiculo.modelo, vendas[i].cliente.nome, vendas[i].valorFinal, pagamento);
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

static void escreverSeparador(FILE *f) {
    fprintf(f, "------------------------------------------------------------\n");
}

static void escreverCabecalhoSecao(FILE *f, const char *titulo) {
    fprintf(f, "\n============================================================\n");
    fprintf(f, "  %s\n", titulo);
    fprintf(f, "============================================================\n");
}

void exportarRelatorioTXT(const Veiculo veiculos[], int totalVeiculos, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes, const Manutencao manutencoes[], int totalManutencoes) {

    criarPastaDados();

    FILE *f = fopen("data/relatorio.txt", "w");
    if (!f) {
        printf("\n[ERRO] Nao foi possivel gerar o arquivo de relatorio!\n");
        return;
    }

    time_t agora = time(NULL);
    struct tm *info = localtime(&agora);
    char dataHora[40];
    strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", info);

    fprintf(f, "############################################################\n");
    fprintf(f, "#              RELATORIO GERAL - SISTEMA LOCAR            #\n");
    fprintf(f, "############################################################\n");
    fprintf(f, "Gerado em: %s\n", dataHora);

    // --- Resumo Financeiro ---
    float investido = 0.0f, arrecadadoVendas = 0.0f, arrecadadoLocacoes = 0.0f, despesasManutencao = 0.0f;
    for (int i = 0; i < totalVeiculos; i++) investido += veiculos[i].valorCompra;
    for (int i = 0; i < totalVendas; i++) arrecadadoVendas += vendas[i].valorFinal;
    for (int i = 0; i < totalLocacoes; i++) if (locacoes[i].status == 0) arrecadadoLocacoes += locacoes[i].valorTotal;
    for (int i = 0; i < totalManutencoes; i++) despesasManutencao += manutencoes[i].custo;
    float faturamentoTotal = arrecadadoVendas + arrecadadoLocacoes;
    float lucroLiquido = faturamentoTotal - investido - despesasManutencao;
    float valorEstoque = valorEstoqueDisponivel(veiculos, totalVeiculos);

    escreverCabecalhoSecao(f, "RESUMO FINANCEIRO GERAL");
    fprintf(f, "%-40s R$ %14.2f\n", "Total Investido em Veiculos:", investido);
    fprintf(f, "%-40s R$ %14.2f\n", "Faturamento em Vendas:", arrecadadoVendas);
    fprintf(f, "%-40s R$ %14.2f\n", "Faturamento em Locacoes:", arrecadadoLocacoes);
    fprintf(f, "%-40s R$ %14.2f\n", "Gastos com Manutencao:", despesasManutencao);
    escreverSeparador(f);
    fprintf(f, "%-40s R$ %14.2f\n", "LUCRO / PREJUIZO LIQUIDO:", lucroLiquido);
    fprintf(f, "%-40s R$ %14.2f\n", "Valor de Mercado do Estoque Disponivel:", valorEstoque);

    // --- Veiculos em Estoque ---
    escreverCabecalhoSecao(f, "VEICULOS EM ESTOQUE");
    fprintf(f, "%-10s %-16s %-16s %6s %10s %18s\n", "Placa", "Marca", "Modelo", "Ano", "Status", "Valor de Mercado");
    escreverSeparador(f);
    const char *statusTexto[] = {"Disponivel", "Alugado", "Vendido", "Em Manutencao"};
    for (int i = 0; i < totalVeiculos; i++) {
        const char *status = (veiculos[i].status >= 0 && veiculos[i].status <= 3) ? statusTexto[veiculos[i].status] : "Desconhecido";
        fprintf(f, "%-10s %-16s %-16s %6d %10s %15.2f\n",
                veiculos[i].placa, veiculos[i].marca, veiculos[i].modelo, veiculos[i].ano,
                status, calcularValorDepreciado(&veiculos[i]));
    }
    if (totalVeiculos == 0) fprintf(f, "Nenhum veiculo cadastrado.\n");

    // --- Vendas Realizadas ---
    escreverCabecalhoSecao(f, "VEICULOS VENDIDOS");
    fprintf(f, "%-6s %-10s %-24s %10s %-24s\n", "ID", "Placa", "Comprador", "Valor", "Pagamento");
    escreverSeparador(f);
    for (int i = 0; i < totalVendas; i++) {
        char pagamento[50];
        if (vendas[i].formaPagamento == 2) {
            snprintf(pagamento, sizeof(pagamento), "Financiado %dx de R$%.2f", vendas[i].numParcelas, vendas[i].valorParcela);
        } else {
            snprintf(pagamento, sizeof(pagamento), "A Vista");
        }
        fprintf(f, "%-6d %-10s %-24s %10.2f %-24s\n",
                vendas[i].idVenda, vendas[i].veiculo.placa, vendas[i].cliente.nome, vendas[i].valorFinal, pagamento);
    }
    if (totalVendas == 0) fprintf(f, "Nenhuma venda registrada.\n");

    // --- Locacoes Ativas ---
    escreverCabecalhoSecao(f, "LOCACOES ATIVAS");
    fprintf(f, "%-6s %-10s %-16s %-14s %10s\n", "ID", "Placa", "CPF Cliente", "Data Inicio", "Dias");
    escreverSeparador(f);
    int locacoesAtivas = 0;
    for (int i = 0; i < totalLocacoes; i++) {
        if (locacoes[i].status == 1) {
            fprintf(f, "%-6d %-10s %-16s %-14s %10d\n",
                    locacoes[i].idLocacao, locacoes[i].placaVeiculo, locacoes[i].cpfCliente,
                    locacoes[i].dataInicio, locacoes[i].diasPrevistos);
            locacoesAtivas++;
        }
    }
    if (locacoesAtivas == 0) fprintf(f, "Nenhuma locacao ativa no momento.\n");

    // --- Historico de Locacoes ---
    escreverCabecalhoSecao(f, "HISTORICO DE LOCACOES FINALIZADAS");
    fprintf(f, "%-6s %-10s %8s %14s\n", "ID", "Placa", "Dias", "Valor Total");
    escreverSeparador(f);
    int locacoesFinalizadas = 0;
    for (int i = 0; i < totalLocacoes; i++) {
        if (locacoes[i].status == 0) {
            fprintf(f, "%-6d %-10s %8d %14.2f\n",
                    locacoes[i].idLocacao, locacoes[i].placaVeiculo,
                    locacoes[i].diasUtilizados, locacoes[i].valorTotal);
            locacoesFinalizadas++;
        }
    }
    if (locacoesFinalizadas == 0) fprintf(f, "Nenhuma locacao finalizada.\n");

    // --- Despesas de Manutencao ---
    escreverCabecalhoSecao(f, "DESPESAS COM MANUTENCAO");
    fprintf(f, "%-6s %-10s %-30s %12s\n", "ID", "Placa", "Descricao", "Custo");
    escreverSeparador(f);
    for (int i = 0; i < totalManutencoes; i++) {
        fprintf(f, "%-6d %-10s %-30s %12.2f\n",
                manutencoes[i].idManutencao, manutencoes[i].placaVeiculo,
                manutencoes[i].descricao, manutencoes[i].custo);
    }
    if (totalManutencoes == 0) fprintf(f, "Nenhuma manutencao registrada.\n");
    escreverSeparador(f);
    fprintf(f, "%-40s R$ %14.2f\n", "Total Gasto com Manutencoes:", despesasManutencao);

    fprintf(f, "\n############################################################\n");
    fprintf(f, "#                      FIM DO RELATORIO                   #\n");
    fprintf(f, "############################################################\n");

    fclose(f);
    printf("\n[SUCESSO] Relatorio salvo em data/relatorio.txt\n");
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
        printf("\n| 5 - Exportar Relatorio (TXT)     |");
        printf("\n| 0 - Sair                         |");
        printf("\n====================================");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) relatorioVendasLocacoesPorPeriodo(vendas, totalVendas, locacoes, totalLocacoes);
        else if (opcao == 2) resumoFinanceiroGeral(veiculos, totalVeiculos, vendas, totalVendas, locacoes, totalLocacoes, manutencoes, totalManutencoes);
        else if (opcao == 3) relatorioVeiculosVendidos(vendas, totalVendas);
        else if (opcao == 4) relatorioDespesasManutencao(manutencoes, totalManutencoes);
        else if (opcao == 5) exportarRelatorioTXT(veiculos, totalVeiculos, vendas, totalVendas, locacoes, totalLocacoes, manutencoes, totalManutencoes);
    } while (opcao != 0);
}
