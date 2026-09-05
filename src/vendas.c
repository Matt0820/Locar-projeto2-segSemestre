#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include"../include/vendas.h"
#include "../include/clientes.h"
#include "../include/veiculos.h"

// =======================================================
//                    MÓDULO DE VENDAS
// =======================================================

void realizarVenda(Veiculo **veiculos, int *totalVeiculos, Cliente **clientes, int *totalClientes, int *capClientes, Venda **vendas, int *totalVendas, int *capVendas) {
    if (*totalVeiculos == 0) {
        printf("\nNenhum veiculo disponivel no estoque para venda.\n");
        return;
    }

    char placa[20];
    printf("\nDigite a Placa do veiculo a ser vendido: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    int idxVeiculo = buscarVeiculo(*veiculos, *totalVeiculos, placa);
    if (idxVeiculo == -1) {
        printf("\nVeiculo nao encontrado no estoque!\n");
        return;
    }

    if ((*veiculos)[idxVeiculo].status != 0) {
        printf("\nErro: Veiculo nao esta disponivel para venda. (Status: %s)\n", (*veiculos)[idxVeiculo].status == 1 ? "Alugado" : "Vendido");
        return;
    }

    char buscaCliente[80];
    printf("Digite o CPF ou Nome do comprador: ");
    fgets(buscaCliente, sizeof(buscaCliente), stdin);
    buscaCliente[strcspn(buscaCliente, "\n")] = '\0';

    int idxCliente = buscarCliente(*clientes, *totalClientes, buscaCliente);
    if (idxCliente == -1) {
        int opcaoCliente;
        printf("\nCliente nao encontrado.\n");
        printf("1 - Cadastrar novo cliente\n");
        printf("2 - Retornar ao menu\n");
        printf("Opcao: ");
        scanf("%d", &opcaoCliente);
        limparBuffer();

        if (opcaoCliente == 1) {
            if (*totalClientes == *capClientes) {
                *capClientes *= 2;
                *clientes = (Cliente *) realloc(*clientes, *capClientes * sizeof(Cliente));
            }
            cadastrarCliente(&(*clientes)[*totalClientes]);
            idxCliente = *totalClientes;
            (*totalClientes)++;
        } else {
            printf("\nVenda cancelada. Retornando ao menu...\n");
            return;
        }
    }

    if (*totalVendas == *capVendas) {
        *capVendas *= 2;
        *vendas = (Venda *) realloc(*vendas, *capVendas * sizeof(Venda));
    }

    Venda v;
    v.veiculo = (*veiculos)[idxVeiculo];
    v.cliente = (*clientes)[idxCliente];

    printf("Digite a data da venda (DD/MM/AAAA): ");
    fgets(v.dataVenda, sizeof(v.dataVenda), stdin);
    v.dataVenda[strcspn(v.dataVenda, "\n")] = '\0';

    (*vendas)[*totalVendas] = v;
    (*totalVendas)++;

    // Atualiza status do veículo para Vendido (2) em vez de apagar do array base
    (*veiculos)[idxVeiculo].status = 2;

    printf("\n[SUCESSO] Venda realizada para %s!\n", v.cliente.nome);
}

void listarVendas(const Venda vendas[], int totalVendas) {
    if (totalVendas == 0) {
        printf("\nNenhuma venda realizada ainda.\n");
        return;
    }
    printf("\n=== Historico de Vendas Realizadas (%d) ===\n", totalVendas);
    for (int i = 0; i < totalVendas; i++) {
        printf("\nVenda #%d | Data: %s\n", i + 1, vendas[i].dataVenda);
        printf("  Veiculo Vendido: %s %s (%d) | Placa: %s | Valor: R$ %.2f\n", 
               vendas[i].veiculo.marca, vendas[i].veiculo.modelo, vendas[i].veiculo.ano,
               vendas[i].veiculo.placa, vendas[i].veiculo.valor);
        printf("  Comprador: %s | CPF: %s | Tel: %s\n", 
               vendas[i].cliente.nome, vendas[i].cliente.cpf, vendas[i].cliente.telefone);
    }
}

void menuVendas(Veiculo **veiculos, int *totalVeiculos, Cliente **clientes, int *totalClientes, int *capClientes, Venda **vendas, int *totalVendas, int *capVendas) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|              MENU VENDA           |");
        printf("\n====================================");
        printf("\n| 1 - Realizar Nova Venda           |");
        printf("\n| 2 - Historico de Vendas           |");
        printf("\n| 0 - Voltar ao Menu Principal      |");
        printf("\n====================================");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            realizarVenda(veiculos, totalVeiculos, clientes, totalClientes, capClientes, vendas, totalVendas, capVendas);
        } else if (opcao == 2) {
            listarVendas(*vendas, *totalVendas);
        }
    } while (opcao != 0);
}
