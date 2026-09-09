#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include "../include/vendas.h"
#include "../include/clientes.h"
#include "../include/veiculos.h"

void realizarVenda(Veiculo **veiculos, int *totalVeiculos, Cliente **clientes, int *totalClientes, int *capClientes, Venda **vendas, int *totalVendas, int *capVendas) {
    char placa[20];
    printf("\nDigite a Placa do veiculo a vender: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    int idxV = buscarVeiculo(*veiculos, *totalVeiculos, placa);
    if (idxV == -1) {
        printf("[ERRO] Veiculo nao encontrado!\n");
        return;
    }

    if ((*veiculos)[idxV].status != 0) {
        printf("[ERRO] Veiculo indisponivel! Status atual: %d\n", (*veiculos)[idxV].status);
        return;
    }

    char buscaC[80];
    printf("Digite o CPF ou Nome do comprador: ");
    fgets(buscaC, sizeof(buscaC), stdin);
    buscaC[strcspn(buscaC, "\n")] = '\0';

    int idxC = buscarCliente(*clientes, *totalClientes, buscaC);
    if (idxC == -1) {
        printf("Cliente nao cadastrado. Redirecionando para cadastro...\n");
        if (*totalClientes == *capClientes) {
            *capClientes *= 2;
            *clientes = (Cliente *) realloc(*clientes, *capClientes * sizeof(Cliente));
        }
        cadastrarCliente(&(*clientes)[*totalClientes], *clientes, *totalClientes);
        idxC = *totalClientes;
        (*totalClientes)++;
    }

    if (*totalVendas == *capVendas) {
        *capVendas *= 2;
        *vendas = (Venda *) realloc(*vendas, *capVendas * sizeof(Venda));
    }

    Venda v;
    v.idVenda = (*totalVendas) + 1;
    v.veiculo = (*veiculos)[idxV];
    v.cliente = (*clientes)[idxC];
    v.valorFinal = (*veiculos)[idxV].valor;

    printf("Data da venda (DD/MM/AAAA): ");
    fgets(v.dataVenda, sizeof(v.dataVenda), stdin);
    v.dataVenda[strcspn(v.dataVenda, "\n")] = '\0';

    (*vendas)[*totalVendas] = v;
    (*totalVendas)++;
    (*veiculos)[idxV].status = 2; // Vendido

    printf("\n[SUCESSO] Venda #%d efetuada com sucesso!\n", v.idVenda);
}

void listarVendas(const Venda vendas[], int totalVendas) {
    if (totalVendas == 0) {
        printf("\nNenhuma venda realizada.\n");
        return;
    }
    printf("\n=== Vendas Realizadas (%d) ===\n", totalVendas);
    for (int i = 0; i < totalVendas; i++) {
        printf("ID: %d | Data: %s | Veiculo: %s (%s) | Comprador: %s | Valor: R$%.2f\n",
               vendas[i].idVenda, vendas[i].dataVenda, vendas[i].veiculo.modelo, 
               vendas[i].veiculo.placa, vendas[i].cliente.nome, vendas[i].valorFinal);
    }
}

void procurarVendas(const Venda vendas[], int totalVendas, const Veiculo veiculos[], int totalVeiculos) {
    int op;
    printf("\n1 - Buscar Venda por Cliente (CPF/Nome)");
    printf("\n2 - Buscar Venda por Veiculo (Placa)");
    printf("\nOpcao: ");
    scanf("%d", &op);
    limparBuffer();

    char termo[80];
    printf("Digite o termo de busca: ");
    fgets(termo, sizeof(termo), stdin);
    termo[strcspn(termo, "\n")] = '\0';

    for (int i = 0; i < totalVendas; i++) {
        if ((op == 1 && (strstr(vendas[i].cliente.nome, termo) || strcmp(vendas[i].cliente.cpf, termo) == 0)) ||
            (op == 2 && strcmp(vendas[i].veiculo.placa, termo) == 0)) {
            printf("ID Venda: %d | Data: %s | Cliente: %s | Placa: %s | Valor: R$%.2f\n",
                   vendas[i].idVenda, vendas[i].dataVenda, vendas[i].cliente.nome, 
                   vendas[i].veiculo.placa, vendas[i].valorFinal);
        }
    }
}

void editarVenda(Venda *vendas, int totalVendas) {
    int id;
    printf("Digite o ID da venda a editar: ");
    scanf("%d", &id);
    limparBuffer();

    for (int i = 0; i < totalVendas; i++) {
        if (vendas[i].idVenda == id) {
            printf("Novo Valor (Atual R$%.2f): ", vendas[i].valorFinal);
            scanf("%f", &vendas[i].valorFinal);
            limparBuffer();
            printf("Nova Data (Atual %s): ", vendas[i].dataVenda);
            fgets(vendas[i].dataVenda, sizeof(vendas[i].dataVenda), stdin);
            vendas[i].dataVenda[strcspn(vendas[i].dataVenda, "\n")] = '\0';
            printf("Venda atualizada com sucesso!\n");
            return;
        }
    }
    printf("ID de venda nao encontrado.\n");
}

void cancelarVenda(Venda **vendas, int *totalVendas, Veiculo **veiculos, int totalVeiculos) {
    int id;
    printf("Digite o ID da venda a cancelar/deletar: ");
    scanf("%d", &id);
    limparBuffer();

    for (int i = 0; i < *totalVendas; i++) {
        if ((*vendas)[i].idVenda == id) {
            int idxV = buscarVeiculo(*veiculos, totalVeiculos, (*vendas)[i].veiculo.placa);
            if (idxV != -1) (*veiculos)[idxV].status = 0; // Torna o veículo disponível novamente

            for (int j = i; j < *totalVendas - 1; j++) {
                (*vendas)[j] = (*vendas)[j + 1];
            }
            (*totalVendas)--;
            printf("Venda cancelada e veiculo retornado ao estoque!\n");
            return;
        }
    }
    printf("Venda nao encontrada.\n");
}

void menuVendas(Veiculo **veiculos, int *totalVeiculos, Cliente **clientes, int *totalClientes, int *capClientes, Venda **vendas, int *totalVendas, int *capVendas) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|              MENU VENDA           |");
        printf("\n====================================");
        printf("\n| 1 - Realizar Nova Venda           |");
        printf("\n| 2 - Historico de Vendas           |");
        printf("\n| 3 - Procurar Vendas               |");
        printf("\n| 4 - Editar Venda (Valor/Data)     |");
        printf("\n| 5 - Cancelar/Deletar Venda        |");
        printf("\n| 0 - Voltar ao Menu Principal      |");
        printf("\n====================================");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) realizarVenda(veiculos, totalVeiculos, clientes, totalClientes, capClientes, vendas, totalVendas, capVendas);
        else if (opcao == 2) listarVendas(*vendas, *totalVendas);
        else if (opcao == 3) procurarVendas(*vendas, *totalVendas, *veiculos, *totalVeiculos);
        else if (opcao == 4) editarVenda(*vendas, *totalVendas);
        else if (opcao == 5) cancelarVenda(vendas, totalVendas, veiculos, *totalVeiculos);
    } while (opcao != 0);
}