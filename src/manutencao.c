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
    lerLinha(placa, sizeof(placa));

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
    
    do {
        printf("Descricao do problema/servico: ");
        lerLinha(m.descricao, sizeof(m.descricao));

        if (strlen(m.descricao) == 0) {
            printf("[ERRO] A descricao nao pode ficar em branco!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("Data de Entrada (DD/MM/AAAA): ");
        lerLinha(m.dataEntrada, sizeof(m.dataEntrada));

        if (!dataValida(m.dataEntrada)) {
            printf("[ERRO] Data invalida! Use o formato DD/MM/AAAA.\n");
        } else {
            break;
        }
    } while (1);

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
    lerLinha(placa, sizeof(placa));

    for (int i = 0; i < totalManutencoes; i++) {
        if (strcmp((*manutencoes)[i].placaVeiculo, placa) == 0 && (*manutencoes)[i].status == 1) {
            do {
                printf("Data de Saida (DD/MM/AAAA): ");
                lerLinha((*manutencoes)[i].dataSaida, sizeof((*manutencoes)[i].dataSaida));

                if (!dataValida((*manutencoes)[i].dataSaida)) {
                    printf("[ERRO] Data invalida! Use o formato DD/MM/AAAA.\n");
                } else {
                    break;
                }
            } while (1);

            do {
                printf("Custo total do servico R$: ");
                scanf("%f", &(*manutencoes)[i].custo);
                limparBuffer();

                if ((*manutencoes)[i].custo < 0) {
                    printf("[ERRO] O custo nao pode ser negativo!\n");
                } else {
                    break;
                }
            } while (1);

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
