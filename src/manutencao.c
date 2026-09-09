#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include "../include/veiculos.h"
#include "../include/manutencao.h"

void registrarManutencao(Veiculo **veiculos, int totalVeiculos, Manutencao **manutencoes, int *totalManutencoes, int *capManutencoes) {
    char placa[20];
    printf("\nDigite a placa do veiculo para manutencao: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    int idxV = buscarVeiculo(*veiculos, totalVeiculos, placa);
    if (idxV == -1) {
        printf("Veiculo nao encontrado.\n");
        return;
    }

    if ((*veiculos)[idxV].status != 0) {
        printf("[ERRO] Apenas veiculos 'Disponiveis' podem ser colocados em manutencao!\n");
        return;
    }

    if (*totalManutencoes == *capManutencoes) {
        *capManutencoes *= 2;
        *manutencoes = (Manutencao *) realloc(*manutencoes, *capManutencoes * sizeof(Manutencao));
    }

    Manutencao m;
    m.idManutencao = (*totalManutencoes) + 1;
    strcpy(m.placaVeiculo, placa);
    
    printf("Descricao do problema/servico: ");
    fgets(m.descricao, sizeof(m.descricao), stdin);
    m.descricao[strcspn(m.descricao, "\n")] = '\0';

    printf("Data de Entrada (DD/MM/AAAA): ");
    fgets(m.dataEntrada, sizeof(m.dataEntrada), stdin);
    m.dataEntrada[strcspn(m.dataEntrada, "\n")] = '\0';

    m.custo = 0.0;
    m.status = 1; // Em andamento
    strcpy(m.dataSaida, "Pendente");

    (*manutencoes)[*totalManutencoes] = m;
    (*totalManutencoes)++;
    (*veiculos)[idxV].status = 3; // Em manutencao

    printf("Manutencao #%d iniciada com sucesso! Veiculo bloqueado para operacoes.\n", m.idManutencao);
}

void finalizarManutencao(Veiculo **veiculos, int totalVeiculos, Manutencao **manutencoes, int totalManutencoes) {
    char placa[20];
    printf("\nDigite a placa do veiculo em manutencao: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    for (int i = 0; i < totalManutencoes; i++) {
        if (strcmp((*manutencoes)[i].placaVeiculo, placa) == 0 && (*manutencoes)[i].status == 1) {
            printf("Data de Saida (DD/MM/AAAA): ");
            fgets((*manutencoes)[i].dataSaida, sizeof((*manutencoes)[i].dataSaida), stdin);
            (*manutencoes)[i].dataSaida[strcspn((*manutencoes)[i].dataSaida, "\n")] = '\0';

            printf("Custo total do servico R$: ");
            scanf("%f", &(*manutencoes)[i].custo);
            limparBuffer();

            (*manutencoes)[i].status = 0; // Finalizado

            int idxV = buscarVeiculo(*veiculos, totalVeiculos, placa);
            if (idxV != -1) (*veiculos)[idxV].status = 0; // Veículo volta a ficar disponível

            printf("Manutencao finalizada! Veiculo desbloqueado.\n");
            return;
        }
    }
    printf("Nenhuma manutencao em andamento encontrada para esta placa.\n");
}

void listarManutencoesAndamento(const Manutencao manutencoes[], int totalManutencoes) {
    printf("\n=== MANUTENCOES EM ANDAMENTO ===\n");
    int encontrou = 0;
    for (int i = 0; i < totalManutencoes; i++) {
        if (manutencoes[i].status == 1) {
            printf("ID: %d | Placa: %s | Entrada: %s | Descricao: %s\n",
                   manutencoes[i].idManutencao, manutencoes[i].placaVeiculo, 
                   manutencoes[i].dataEntrada, manutencoes[i].descricao);
            encontrou++;
        }
    }
    if (!encontrou) printf("Nenhuma manutencao em andamento.\n");
}

void historicoManutencoes(const Manutencao manutencoes[], int totalManutencoes) {
    printf("\n=== HISTORICO COMPLETO DE MANUTENCOES ===\n");
    for (int i = 0; i < totalManutencoes; i++) {
        printf("ID: %d | Placa: %s | Status: %s | Custo: R$%.2f | Entrada: %s | Saida: %s\n",
               manutencoes[i].idManutencao, manutencoes[i].placaVeiculo,
               manutencoes[i].status == 1 ? "Em Andamento" : "Finalizada",
               manutencoes[i].custo, manutencoes[i].dataEntrada, manutencoes[i].dataSaida);
    }
}

void menuManutencao(Veiculo **veiculos, int totalVeiculos, Manutencao **manutencoes, int *totalManutencoes, int *capManutencoes) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|           MENU MANUTENCAO        |");
        printf("\n====================================");
        printf("\n| 1 - Registrar Manutencao         |");
        printf("\n| 2 - Finalizar Manutencao         |");
        printf("\n| 3 - Listar em Andamento          |");
        printf("\n| 4 - Historico Geral              |");
        printf("\n| 0 - Sair                         |");
        printf("\n====================================");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) registrarManutencao(veiculos, totalVeiculos, manutencoes, totalManutencoes, capManutencoes);
        else if (opcao == 2) finalizarManutencao(veiculos, totalVeiculos, manutencoes, *totalManutencoes);
        else if (opcao == 3) listarManutencoesAndamento(*manutencoes, *totalManutencoes);
        else if (opcao == 4) historicoManutencoes(*manutencoes, *totalManutencoes);
    } while (opcao != 0);
}