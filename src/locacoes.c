#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/clientes.h"
#include "../include/util.h"
#include "../include/veiculos.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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

void iniciarLocacao(Veiculo **veiculos, int totalVeiculos, Cliente **clientes, int totalClientes, Locacao **locacoes, int *totalLocacoes, int *capLocacoes) {
    char placa[20];
    printf("\n--- INICIAR LOCACAO ---\n");
    printf("Placa do veiculo: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    int idxV = buscarVeiculo(*veiculos, totalVeiculos, placa);
    if (idxV == -1) {
        printf("Erro: Veiculo nao encontrado.\n");
        return;
    }

    if ((*veiculos)[idxV].status != 0) {
        printf("Erro: Veiculo indisponivel (Status atual: %s).\n", (*veiculos)[idxV].status == 1 ? "Alugado" : "Vendido");
        return;
    }

    if (*totalLocacoes == *capLocacoes) {
        *capLocacoes *= 2;
        *locacoes = (Locacao *) realloc(*locacoes, *capLocacoes * sizeof(Locacao));
    }

    Locacao nova;
    nova.idLocacao = (*totalLocacoes) + 1;
    strcpy(nova.placaVeiculo, (*veiculos)[idxV].placa);
    
    printf("CPF do Cliente: ");
    fgets(nova.cpfCliente, sizeof(nova.cpfCliente), stdin);
    nova.cpfCliente[strcspn(nova.cpfCliente, "\n")] = '\0';
    
    if(buscarCliente(*clientes, totalClientes, nova.cpfCliente) == -1){
        printf("Aviso: CPF nao encontrado no cadastro de clientes. Registre o cliente posteriormente.\n");
    }

    printf("Data de Inicio (DD/MM/AAAA): ");
    fgets(nova.dataInicio, sizeof(nova.dataInicio), stdin);
    nova.dataInicio[strcspn(nova.dataInicio, "\n")] = '\0';
    
    printf("Dias Previstos: ");
    scanf("%d", &nova.diasPrevistos);
    limparBuffer();

    nova.kmInicial = (*veiculos)[idxV].km;
    nova.valorDiaria = (*veiculos)[idxV].valorDiaria;
    nova.valorTotal = nova.diasPrevistos * nova.valorDiaria;
    nova.status = 1; 
    nova.taxaAvariaMulta = 0.0;
    nova.kmFinal = 0;
    nova.diasUtilizados = 0;

    (*locacoes)[*totalLocacoes] = nova;
    (*totalLocacoes)++;
    (*veiculos)[idxV].status = 1; // Marca como alugado

    printf("\nLocacao #%d iniciada com sucesso! Valor Previsto: R$ %.2f\n", nova.idLocacao, nova.valorTotal);
}

void estenderLocacao(Locacao **locacoes, int totalLocacoes) {
    char placa[20];
    printf("\n--- ESTENDER LOCACAO ---\n");
    printf("Placa do veiculo: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    int idxL = buscarLocacaoAtiva(*locacoes, totalLocacoes, placa);
    if (idxL == -1) {
        printf("Erro: Nenhuma locacao ativa encontrada para esta placa.\n");
        return;
    }

    int diasExtras;
    printf("Quantos dias deseja adicionar? ");
    scanf("%d", &diasExtras);
    limparBuffer();

    (*locacoes)[idxL].diasPrevistos += diasExtras;
    (*locacoes)[idxL].valorTotal = (*locacoes)[idxL].diasPrevistos * (*locacoes)[idxL].valorDiaria;

    printf("Locacao estendida! Novo prazo: %d dias. Novo valor estimado: R$ %.2f\n", 
           (*locacoes)[idxL].diasPrevistos, (*locacoes)[idxL].valorTotal);
}

void finalizarLocacao(Veiculo **veiculos, int totalVeiculos, Locacao **locacoes, int totalLocacoes) {
    char placa[20];
    printf("\n--- FINALIZAR LOCACAO ---\n");
    printf("Placa do veiculo: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    int idxL = buscarLocacaoAtiva(*locacoes, totalLocacoes, placa);
    if (idxL == -1) {
        printf("Erro: Nenhuma locacao ativa encontrada para esta placa.\n");
        return;
    }

    int idxV = buscarVeiculo(*veiculos, totalVeiculos, placa);

    printf("Dias efetivamente utilizados: ");
    scanf("%d", &(*locacoes)[idxL].diasUtilizados);

    printf("Quilometragem final de devolucao: ");
    scanf("%d", &(*locacoes)[idxL].kmFinal);

    while ((*locacoes)[idxL].kmFinal < (*locacoes)[idxL].kmInicial) {
        printf("Erro: KM final não pode ser menor que a inicial (%d km). Digite novamente: ", (*locacoes)[idxL].kmInicial);
        scanf("%d", &(*locacoes)[idxL].kmFinal);
    }

    printf("Taxas Adicionais / Multas / Avarias (R$ 0.00 se nenhuma): ");
    scanf("%f", &(*locacoes)[idxL].taxaAvariaMulta);

    (*locacoes)[idxL].valorTotal = ((*locacoes)[idxL].diasUtilizados * (*locacoes)[idxL].valorDiaria) + (*locacoes)[idxL].taxaAvariaMulta;

    printf("Forma de Pagamento (1-PIX, 2-Cartao, 3-Dinheiro): ");
    scanf("%d", &(*locacoes)[idxL].formaPagamento);
    limparBuffer();

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

void menuLocacoes(Veiculo **veiculos, int totalVeiculos, Cliente **clientes, int totalClientes, Locacao **locacoes, int *totalLocacoes, int *capLocacoes) {
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

        if (opcao == 1) iniciarLocacao(veiculos, totalVeiculos, clientes, totalClientes, locacoes, totalLocacoes, capLocacoes);
        else if (opcao == 2) estenderLocacao(locacoes, *totalLocacoes);
        else if (opcao == 3) finalizarLocacao(veiculos, totalVeiculos, locacoes, *totalLocacoes);
        else if (opcao == 4) listarLocacoesAtivas(*locacoes, *totalLocacoes);
        else if (opcao == 5) historicoLocacoes(*locacoes, *totalLocacoes);
    } while (opcao != 0);
}
