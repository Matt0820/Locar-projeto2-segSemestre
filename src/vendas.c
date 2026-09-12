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
    lerLinha(placa, sizeof(placa));

    int idxV = buscarVeiculo(*veiculos, *totalVeiculos, placa);
    if (idxV == -1) {
        printf("[ERRO] Veiculo nao encontrado!\n");
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

    char buscaC[80];
    printf("Digite o CPF ou Nome do comprador: ");
    lerLinha(buscaC, sizeof(buscaC));

    int idxC = buscarCliente(*clientes, *totalClientes, buscaC);
    if (idxC == -1) {
        int opcaoCliente;
        do {
            printf("\nCliente nao encontrado no sistema.\n");
            printf("1 - Cadastrar novo cliente agora\n");
            printf("2 - Voltar ao menu de vendas (cancelar)\n");
            printf("Opcao: ");
            scanf("%d", &opcaoCliente);
            limparBuffer();

            if (opcaoCliente != 1 && opcaoCliente != 2) {
                printf("[ERRO] Opcao invalida! Escolha 1 ou 2.\n");
            }
        } while (opcaoCliente != 1 && opcaoCliente != 2);

        if (opcaoCliente == 2) {
            printf("\nVenda cancelada.\n");
            return;
        }

        if (*totalClientes == *capClientes) {
            *capClientes *= 2;
            *clientes = (Cliente *) realloc(*clientes, *capClientes * sizeof(Cliente));
        }
        cadastrarCliente(&(*clientes)[*totalClientes], *clientes, *totalClientes);
        idxC = *totalClientes;
        (*totalClientes)++;
        printf("\nCliente cadastrado com sucesso!\n");
    }

    if (*totalVendas == *capVendas) {
        *capVendas *= 2;
        *vendas = (Venda *) realloc(*vendas, *capVendas * sizeof(Venda));
    }

    Venda v;
    v.idVenda = (*totalVendas) + 1;
    v.veiculo = (*veiculos)[idxV];
    v.cliente = (*clientes)[idxC];

    float valorMercado = calcularValorDepreciado(&(*veiculos)[idxV]);
    printf("\nValor de mercado estimado (com depreciacao): R$ %.2f\n", valorMercado);

    do {
        printf("Valor de Venda R$: ");
        scanf("%f", &v.valorFinal);
        limparBuffer();

        if (v.valorFinal <= 0) {
            printf("[ERRO] O valor de venda deve ser maior que zero!\n");
        } else {
            break;
        }
    } while (1);

    // --- Forma de Pagamento ---
    int formaPagamento = 0;
    int numParcelas = 0;
    float valorParcela = 0.0f;
    int negociado = 0;

    while (!negociado) {
        do {
            printf("\nForma de Pagamento:\n");
            printf("1 - A Vista\n");
            printf("2 - Financiado\n");
            printf("Opcao: ");
            scanf("%d", &formaPagamento);
            limparBuffer();

            if (formaPagamento != 1 && formaPagamento != 2) {
                printf("[ERRO] Opcao invalida! Escolha 1 ou 2.\n");
            }
        } while (formaPagamento != 1 && formaPagamento != 2);

        if (formaPagamento == 1) {
            numParcelas = 0;
            valorParcela = 0.0f;
            negociado = 1;
        } else {
            int opcoesParcelas[5] = {12, 24, 36, 48, 60};
            float limiteParcela = (*clientes)[idxC].rendaMensal * 0.30f;
            int escolha;
            int cancelado = 0;

            do {
                printf("\n--- Opcoes de Financiamento (Valor: R$ %.2f) ---\n", v.valorFinal);
                printf("Limite de parcela aprovado (30%% da renda mensal do cliente): R$ %.2f\n", limiteParcela);
                for (int i = 0; i < 5; i++) {
                    float parcela = v.valorFinal / opcoesParcelas[i];
                    int aprovado = parcela <= limiteParcela;
                    printf("%d) %dx de R$ %.2f %s\n", i + 1, opcoesParcelas[i], parcela,
                           aprovado ? "[APROVADO]" : "[REPROVADO]");
                }
                printf("Escolha o numero de parcelas (1 a 5) ou 0 para voltar e pagar a vista: ");
                scanf("%d", &escolha);
                limparBuffer();

                if (escolha == 0) {
                    cancelado = 1;
                    break;
                }
                if (escolha < 1 || escolha > 5) {
                    printf("[ERRO] Opcao invalida!\n");
                    continue;
                }

                numParcelas = opcoesParcelas[escolha - 1];
                valorParcela = v.valorFinal / numParcelas;

                if (valorParcela > limiteParcela) {
                    printf("[ERRO] Financiamento NAO APROVADO: a parcela ultrapassa 30%% da renda mensal do cliente!\n");
                    numParcelas = 0;
                    valorParcela = 0.0f;
                } else {
                    negociado = 1;
                    break;
                }
            } while (1);

            if (cancelado) {
                printf("Financiamento cancelado. Escolha novamente a forma de pagamento.\n");
                numParcelas = 0;
                valorParcela = 0.0f;
            }
        }
    }

    v.formaPagamento = formaPagamento;
    v.numParcelas = numParcelas;
    v.valorParcela = valorParcela;
    v.veiculo.valor = v.valorFinal;

    do {
        printf("Data da venda (DD/MM/AAAA): ");
        lerLinha(v.dataVenda, sizeof(v.dataVenda));

        if (!dataValida(v.dataVenda)) {
            printf("[ERRO] Data invalida! Use o formato DD/MM/AAAA.\n");
        } else {
            break;
        }
    } while (1);

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
        char pagamento[50];
        if (vendas[i].formaPagamento == 2) {
            snprintf(pagamento, sizeof(pagamento), "Financiado %dx de R$%.2f",
                     vendas[i].numParcelas, vendas[i].valorParcela);
        } else {
            snprintf(pagamento, sizeof(pagamento), "A Vista");
        }

        printf("ID: %d | Data: %s | Veiculo: %s (%s) | Comprador: %s | Valor: R$%.2f | Pagamento: %s\n",
               vendas[i].idVenda, vendas[i].dataVenda, vendas[i].veiculo.modelo, 
               vendas[i].veiculo.placa, vendas[i].cliente.nome, vendas[i].valorFinal, pagamento);
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
    lerLinha(termo, sizeof(termo));

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
            do {
                printf("Novo Valor (Atual R$%.2f): ", vendas[i].valorFinal);
                scanf("%f", &vendas[i].valorFinal);
                limparBuffer();

                if (vendas[i].valorFinal <= 0) {
                    printf("[ERRO] O valor deve ser maior que zero!\n");
                } else {
                    break;
                }
            } while (1);

            // Se a venda era financiada, recalcula o valor da parcela com base no novo valor total, mantendo a mesma quantidade de parcelas.
            if (vendas[i].formaPagamento == 2 && vendas[i].numParcelas > 0) {
                vendas[i].valorParcela = vendas[i].valorFinal / vendas[i].numParcelas;
            }

            do {
                printf("Nova Data (Atual %s): ", vendas[i].dataVenda);
                lerLinha(vendas[i].dataVenda, sizeof(vendas[i].dataVenda));

                if (!dataValida(vendas[i].dataVenda)) {
                    printf("[ERRO] Data invalida! Use o formato DD/MM/AAAA.\n");
                } else {
                    break;
                }
            } while (1);

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
