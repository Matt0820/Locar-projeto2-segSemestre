#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/clientes.h"
#include "../include/veiculos.h"
#include "../include/util.h"

// =======================================================
//                    MÓDULO DE LOCAÇÃO
// =======================================================

int buscarLocacaoAtiva(const Locacao locacoes[], int total, const char placa[]) {
    for (int i = 0; i < total; i++) {
        if (strcmp(locacoes[i].placaVeiculo, placa) == 0 && locacoes[i].status == 1) {
            return i;
        }
    }
    return -1;
}

void iniciarLocacao(Veiculo **veiculos, int totalVeiculos, Cliente **clientes, int *totalClientes, int *capClientes, Locacao **locacoes, int *totalLocacoes, int *capLocacoes) {
    char placa[20];
    printf("\n--- INICIAR LOCACAO ---\n");
    printf("Placa do veiculo: ");
    lerLinha(placa, sizeof(placa));

    int idxV = buscarVeiculo(*veiculos, totalVeiculos, placa);
    if (idxV == -1) {
        printf("Erro: Veiculo nao encontrado.\n");
        return;
    }

    if ((*veiculos)[idxV].status != 0) {

        char *statusTexto;

        if ((*veiculos)[idxV].status == 1)
            statusTexto = "Alugado";
        else if ((*veiculos)[idxV].status == 2)
            statusTexto = "Vendido";
        else
            statusTexto = "Em Manutencao";

        printf("Erro: Veiculo indisponivel (Status atual: %s).\n", statusTexto);
        return;
    }

    Locacao nova;
    strcpy(nova.placaVeiculo, (*veiculos)[idxV].placa);
    
    do {
        printf("CPF do Cliente: ");
        lerLinha(nova.cpfCliente, sizeof(nova.cpfCliente));

        if (!cpfValido(nova.cpfCliente)) {
            printf("[ERRO] CPF invalido! Digite 11 numeros correspondentes a um CPF real.\n");
        } else {
            break;
        }
    } while (1);

    if (buscarCliente(*clientes, *totalClientes, nova.cpfCliente) == -1) {
        int opcaoCliente;
        do {
            printf("\nCliente com este CPF nao encontrado no sistema.\n");
            printf("1 - Cadastrar novo cliente agora\n");
            printf("2 - Voltar ao menu de locacoes (cancelar)\n");
            printf("Opcao: ");
            scanf("%d", &opcaoCliente);
            limparBuffer();

            if (opcaoCliente != 1 && opcaoCliente != 2) {
                printf("[ERRO] Opcao invalida! Escolha 1 ou 2.\n");
            }
        } while (opcaoCliente != 1 && opcaoCliente != 2);

        if (opcaoCliente == 2) {
            printf("\nLocacao cancelada.\n");
            return;
        }

        if (*totalClientes == *capClientes) {
            *capClientes *= 2;
            *clientes = (Cliente *) realloc(*clientes, *capClientes * sizeof(Cliente));
        }
        cadastrarCliente(&(*clientes)[*totalClientes], *clientes, *totalClientes);
        (*totalClientes)++;
        printf("\nCliente cadastrado com sucesso!\n");
    }

    if (*totalLocacoes == *capLocacoes) {
        *capLocacoes *= 2;
        *locacoes = (Locacao *) realloc(*locacoes, *capLocacoes * sizeof(Locacao));
    }

    nova.idLocacao = (*totalLocacoes) + 1;

    do {
        printf("Data de Inicio (DD/MM/AAAA): ");
        lerLinha(nova.dataInicio, sizeof(nova.dataInicio));

        if (!dataValida(nova.dataInicio)) {
            printf("[ERRO] Data invalida! Use o formato DD/MM/AAAA.\n");
        } else {
            break;
        }
    } while (1);
    
    do {
        printf("Dias Previstos: ");
        scanf("%d", &nova.diasPrevistos);
        limparBuffer();

        if (nova.diasPrevistos <= 0) {
            printf("[ERRO] A quantidade de dias deve ser maior que zero!\n");
        } else {
            break;
        }
    } while (1);

    nova.kmInicial = (*veiculos)[idxV].km;
    nova.valorDiaria = (*veiculos)[idxV].valorDiaria;
    nova.valorTotal = nova.diasPrevistos * nova.valorDiaria;
    nova.status = 1; 
    nova.taxaAvariaMulta = 0.0;
    nova.kmFinal = 0;
    nova.diasUtilizados = 0;
    nova.formaPagamento = 0; 

    (*locacoes)[*totalLocacoes] = nova;
    (*totalLocacoes)++;
    (*veiculos)[idxV].status = 1; // Marca como alugado

    printf("\nLocacao #%d iniciada com sucesso! Valor Previsto: R$ %.2f\n", nova.idLocacao, nova.valorTotal);
}

void estenderLocacao(Locacao **locacoes, int totalLocacoes) {
    char placa[20];
    printf("\n--- ESTENDER LOCACAO ---\n");
    printf("Placa do veiculo: ");
    lerLinha(placa, sizeof(placa));

    int idxL = buscarLocacaoAtiva(*locacoes, totalLocacoes, placa);
    if (idxL == -1) {
        printf("Erro: Nenhuma locacao ativa encontrada para esta placa.\n");
        return;
    }

    int diasExtras;
    do {
        printf("Quantos dias deseja adicionar? ");
        scanf("%d", &diasExtras);
        limparBuffer();

        if (diasExtras <= 0) {
            printf("[ERRO] A quantidade de dias deve ser maior que zero!\n");
        } else {
            break;
        }
    } while (1);

    (*locacoes)[idxL].diasPrevistos += diasExtras;
    (*locacoes)[idxL].valorTotal = (*locacoes)[idxL].diasPrevistos * (*locacoes)[idxL].valorDiaria;

    printf("Locacao estendida! Novo prazo: %d dias. Novo valor estimado: R$ %.2f\n", 
           (*locacoes)[idxL].diasPrevistos, (*locacoes)[idxL].valorTotal);
}

void finalizarLocacao(Veiculo **veiculos, int totalVeiculos, Locacao **locacoes, int totalLocacoes) {
    char placa[20];
    printf("\n--- FINALIZAR LOCACAO ---\n");
    printf("Placa do veiculo: ");
    lerLinha(placa, sizeof(placa));

    int idxL = buscarLocacaoAtiva(*locacoes, totalLocacoes, placa);
    if (idxL == -1) {
        printf("Erro: Nenhuma locacao ativa encontrada para esta placa.\n");
        return;
    }

    int idxV = buscarVeiculo(*veiculos, totalVeiculos, placa);

    do {
        printf("Dias efetivamente utilizados: ");
        scanf("%d", &(*locacoes)[idxL].diasUtilizados);

        if ((*locacoes)[idxL].diasUtilizados < 0) {
            printf("[ERRO] A quantidade de dias nao pode ser negativa!\n");
        } else {
            break;
        }
    } while (1);

    printf("Quilometragem final de devolucao: ");
    scanf("%d", &(*locacoes)[idxL].kmFinal);

    while ((*locacoes)[idxL].kmFinal < (*locacoes)[idxL].kmInicial) {
        printf("Erro: KM final não pode ser menor que a inicial (%d km). Digite novamente: ", (*locacoes)[idxL].kmInicial);
        scanf("%d", &(*locacoes)[idxL].kmFinal);
    }

    do {
        printf("Taxas Adicionais / Multas / Avarias (R$ 0.00 se nenhuma): ");
        scanf("%f", &(*locacoes)[idxL].taxaAvariaMulta);

        if ((*locacoes)[idxL].taxaAvariaMulta < 0) {
            printf("[ERRO] O valor nao pode ser negativo!\n");
        } else {
            break;
        }
    } while (1);

    (*locacoes)[idxL].valorTotal = ((*locacoes)[idxL].diasUtilizados * (*locacoes)[idxL].valorDiaria) + (*locacoes)[idxL].taxaAvariaMulta;

    do {
        printf("Forma de Pagamento (1-PIX, 2-Cartao, 3-Dinheiro): ");
        scanf("%d", &(*locacoes)[idxL].formaPagamento);
        limparBuffer();

        if ((*locacoes)[idxL].formaPagamento < 1 || (*locacoes)[idxL].formaPagamento > 3) {
            printf("[ERRO] Opcao invalida! Escolha 1, 2 ou 3.\n");
        } else {
            break;
        }
    } while (1);

    (*locacoes)[idxL].status = 0;              
    (*veiculos)[idxV].status = 0;                 
    (*veiculos)[idxV].km = (*locacoes)[idxL].kmFinal; 

    printf("\nLOCACAO FINALIZADA COM SUCESSO!\n");
    printf("Total a Pagar: R$ %.2f\n", (*locacoes)[idxL].valorTotal);
    printf("Status do veiculo alterado para DISPONIVEL.\n");
}

void listarLocacoesAtivas(const Locacao locacoes[], int totalLocacoes) {
    printf("\n--- LOCACOES ATIVAS NO MOMENTO ---\n");
    int encontradas = 0;
    for (int i = 0; i < totalLocacoes; i++) {
        if (locacoes[i].status == 1) {
            printf("ID: %d | Veiculo: %s | CPF: %s | Data Início: %s | Previsao: %d dias | Valor/Dia: R$%.2f\n",
                   locacoes[i].idLocacao, locacoes[i].placaVeiculo, locacoes[i].cpfCliente, 
                   locacoes[i].dataInicio, locacoes[i].diasPrevistos, locacoes[i].valorDiaria);
            encontradas++;
        }
    }
    if (encontradas == 0) printf("Nenhuma locacao ativa.\n");
}

void historicoLocacoes(const Locacao locacoes[], int totalLocacoes) {
    printf("\n--- HISTORICO DE LOCACOES FINALIZADAS ---\n");
    int encontradas = 0;
    for (int i = 0; i < totalLocacoes; i++) {
        if (locacoes[i].status == 0) {
            printf("ID: %d | Veiculo: %s | CPF: %s | Dias: %d | KM Rodado: %d | Extras: R$%.2f | Total: R$%.2f\n",
                   locacoes[i].idLocacao, locacoes[i].placaVeiculo, locacoes[i].cpfCliente,
                   locacoes[i].diasUtilizados, (locacoes[i].kmFinal - locacoes[i].kmInicial),
                   locacoes[i].taxaAvariaMulta, locacoes[i].valorTotal);
            encontradas++;
        }
    }
    if (encontradas == 0) printf("Nenhum historico encontrado.\n");
}

void menuLocacoes(Veiculo **veiculos, int totalVeiculos, Cliente **clientes, int *totalClientes, int *capClientes, Locacao **locacoes, int *totalLocacoes, int *capLocacoes) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|          MENU LOCAÇÃO             |");
        printf("\n====================================");
        printf("\n| 1 - Iniciar Locacao               |");
        printf("\n| 2 - Estender Locacao              |");
        printf("\n| 3 - Finalizar Locacao             |");
        printf("\n| 4 - Listar Locacoes Ativas        |");
        printf("\n| 5 - Historico de Locacoes         |");
        printf("\n| 0 - Voltar ao Menu Principal      |");
        printf("\n====================================");
         printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) iniciarLocacao(veiculos, totalVeiculos, clientes, totalClientes, capClientes, locacoes, totalLocacoes, capLocacoes);
        else if (opcao == 2) estenderLocacao(locacoes, *totalLocacoes);
        else if (opcao == 3) finalizarLocacao(veiculos, totalVeiculos, locacoes, *totalLocacoes);
        else if (opcao == 4) listarLocacoesAtivas(*locacoes, *totalLocacoes);
        else if (opcao == 5) historicoLocacoes(*locacoes, *totalLocacoes);
    } while (opcao != 0);
}
