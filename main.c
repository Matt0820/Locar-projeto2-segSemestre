#include <stdio.h>
#include <stdlib.h>
#include "../include/modelos.h"
#include "../include/clientes.h"
#include "../include/locacoes.h"
#include "../include/vendas.h"
#include "../include/veiculos.h"
#include "../include/manutencao.h"
#include "../include/relatorios.h"
#include "../include/util.h"
#include "../include/dados.h"

int main(void) {
    int capacidadeClientes = 2, totalClientes = 0;
    Cliente *clientes = malloc(capacidadeClientes * sizeof(Cliente));

    int capacidadeVeiculos = 2, totalVeiculos = 0;
    Veiculo *veiculos = malloc(capacidadeVeiculos * sizeof(Veiculo));

    int capacidadeVendas = 2, totalVendas = 0;
    Venda *vendas = malloc(capacidadeVendas * sizeof(Venda));

    int capacidadeLocacoes = 2, totalLocacoes = 0;
    Locacao *locacoes = malloc(capacidadeLocacoes * sizeof(Locacao));

    int capacidadeManutencoes = 2, totalManutencoes = 0;
    Manutencao *manutencoes = malloc(capacidadeManutencoes * sizeof(Manutencao));

    if (!clientes || !veiculos || !vendas || !locacoes || !manutencoes) {
        fprintf(stderr, "Erro na alocacao de memoria.\n");
        return 1;
    }

    // Carrega os dados salvos em execucoes anteriores (clientes.csv,
    // veiculos.csv, vendas.csv, locacoes.csv, manutencoes.csv), se existirem.
    carregarDadosCSV(&clientes, &totalClientes, &capacidadeClientes,
                      &veiculos, &totalVeiculos, &capacidadeVeiculos,
                      &vendas, &totalVendas, &capacidadeVendas,
                      &locacoes, &totalLocacoes, &capacidadeLocacoes,
                      &manutencoes, &totalManutencoes, &capacidadeManutencoes);

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
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) menuClientes(&clientes, &totalClientes, &capacidadeClientes, vendas, totalVendas, locacoes, totalLocacoes);
        else if (opcao == 2) menuVeiculos(&veiculos, &totalVeiculos, &capacidadeVeiculos, vendas, totalVendas, locacoes, totalLocacoes);
        else if (opcao == 3) menuVendas(&veiculos, &totalVeiculos, &clientes, &totalClientes, &capacidadeClientes, &vendas, &totalVendas, &capacidadeVendas);
        else if (opcao == 4) menuLocacoes(&veiculos, totalVeiculos, &clientes, &totalClientes, &capacidadeClientes, &locacoes, &totalLocacoes, &capacidadeLocacoes);
        else if (opcao == 5) menuManutencao(&veiculos, totalVeiculos, &manutencoes, &totalManutencoes, &capacidadeManutencoes);
        else if (opcao == 6) menuRelatorios(veiculos, totalVeiculos, vendas, totalVendas, locacoes, totalLocacoes, manutencoes, totalManutencoes);
        else if (opcao != 0) printf("\n[ERRO] Opcao invalida!\n");

        // Grava tudo em CSV assim que o usuario volta ao menu principal
        if (opcao != 0) {
            salvarDadosCSV(clientes, totalClientes, veiculos, totalVeiculos, vendas, totalVendas, locacoes, totalLocacoes, manutencoes, totalManutencoes);
        }

    } while (opcao != 0);

    // Salvamento final ao encerrar o sistema normalmente (opcao 0).
    salvarDadosCSV(clientes, totalClientes, veiculos, totalVeiculos, vendas, totalVendas, locacoes, totalLocacoes, manutencoes, totalManutencoes);

    free(clientes);
    free(veiculos);
    free(vendas);
    free(locacoes);
    free(manutencoes);

    printf("\nSistema encerrado com sucesso.\n");
    return 0;
}
