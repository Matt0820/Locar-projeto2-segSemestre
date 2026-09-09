#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include "../include/veiculos.h"

void cadastrarVeiculo(Veiculo *v, const Veiculo veiculos[], int totalVeiculos) {
    printf("\n--- Novo Cadastro de Veiculo ---\n");
    
    do {
        printf("Placa: ");
        fgets(v->placa, sizeof(v->placa), stdin);
        v->placa[strcspn(v->placa, "\n")] = '\0';

        if (buscarVeiculo(veiculos, totalVeiculos, v->placa) != -1) {
            printf("[ERRO] Esta placa ja esta cadastrada no sistema!\n");
        } else {
            break;
        }
    } while (1);

    printf("Marca: ");
    fgets(v->marca, sizeof(v->marca), stdin);
    v->marca[strcspn(v->marca, "\n")] = '\0';

    printf("Modelo: ");
    fgets(v->modelo, sizeof(v->modelo), stdin);
    v->modelo[strcspn(v->modelo, "\n")] = '\0';

    printf("Ano: ");
    scanf("%d", &v->ano);

    do {
        printf("Valor de Compra R$: ");
        scanf("%f", &v->valorCompra);
        printf("Valor de Venda R$: ");
        scanf("%f", &v->valor);
        if (v->valor <= 0 || v->valorCompra <= 0) {
            printf("[ERRO] Os valores de compra e venda devem ser maiores que zero!\n");
        } else break;
    } while (1);

    printf("Quilometragem atual: ");
    scanf("%d", &v->km);
    
    printf("Valor Diaria Locacao R$: ");
    scanf("%f", &v->valorDiaria);
    limparBuffer();
    
    v->status = 0; // Disponível por padrão
}

void listarVeiculosDisponiveis(const Veiculo veiculos[], int total) {
    int encontrados = 0;
    printf("\n=== Veiculos Disponiveis no Estoque ===\n");
    for (int i = 0; i < total; i++) {
        if (veiculos[i].status == 0) {
            printf("%d) %s %s (%d) | Placa: %s | Valor: R$ %.2f | KM: %d\n", 
                   i + 1, veiculos[i].marca, veiculos[i].modelo, veiculos[i].ano, 
                   veiculos[i].placa, veiculos[i].valor, veiculos[i].km);
            encontrados++;
        }
    }
    if (encontrados == 0) printf("Nenhum veiculo disponivel no momento.\n");
}

int buscarVeiculo(const Veiculo veiculos[], int total, const char placa[]) {
    for (int i = 0; i < total; i++) {
        if (strcmp(veiculos[i].placa, placa) == 0) return i;
    }
    return -1;
}

void procurarVeiculosAvancado(const Veiculo veiculos[], int total) {
    int opcao;
    printf("\n--- BUSCA DE VEICULOS ---");
    printf("\n1 - Por Nome/Modelo");
    printf("\n2 - Por Marca");
    printf("\n3 - Por Placa");
    printf("\n4 - Por Valor (Maior, Menor ou Igual)");
    printf("\nOpcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 1) {
        char modelo[50];
        printf("Digite o Modelo: ");
        fgets(modelo, sizeof(modelo), stdin);
        modelo[strcspn(modelo, "\n")] = '\0';
        for (int i = 0; i < total; i++) {
            if (strstr(veiculos[i].modelo, modelo) != NULL) {
                printf("Placa: %s | %s %s | Valor: R$%.2f\n", veiculos[i].placa, veiculos[i].marca, veiculos[i].modelo, veiculos[i].valor);
            }
        }
    } else if (opcao == 2) {
        char marca[50];
        printf("Digite a Marca: ");
        fgets(marca, sizeof(marca), stdin);
        marca[strcspn(marca, "\n")] = '\0';
        for (int i = 0; i < total; i++) {
            if (strstr(veiculos[i].marca, marca) != NULL) {
                printf("Placa: %s | %s %s | Valor: R$%.2f\n", veiculos[i].placa, veiculos[i].marca, veiculos[i].modelo, veiculos[i].valor);
            }
        }
    } else if (opcao == 3) {
        char placa[20];
        printf("Digite a Placa: ");
        fgets(placa, sizeof(placa), stdin);
        placa[strcspn(placa, "\n")] = '\0';
        int idx = buscarVeiculo(veiculos, total, placa);
        if (idx != -1) {
            printf("Placa: %s | %s %s | Valor: R$%.2f\n", veiculos[idx].placa, veiculos[idx].marca, veiculos[idx].modelo, veiculos[idx].valor);
        } else printf("Veiculo nao encontrado.\n");
    } else if (opcao == 4) {
        int comp;
        float valorRef;
        printf("1 - Maior que | 2 - Menor que | 3 - Igual a: ");
        scanf("%d", &comp);
        printf("Digite o Valor R$: ");
        scanf("%f", &valorRef);
        limparBuffer();

        for (int i = 0; i < total; i++) {
            if ((comp == 1 && veiculos[i].valor > valorRef) ||
                (comp == 2 && veiculos[i].valor < valorRef) ||
                (comp == 3 && veiculos[i].valor == valorRef)) {
                printf("Placa: %s | %s %s | Valor: R$%.2f\n", veiculos[i].placa, veiculos[i].marca, veiculos[i].modelo, veiculos[i].valor);
            }
        }
    }
}

void editarVeiculo(Veiculo *v) {
    char temp[100];
    printf("\n--- Edicao de Veiculo (ENTER para manter) ---\n");

    printf("Marca ('%s'): ", v->marca);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(v->marca, temp);

    printf("Modelo ('%s'): ", v->modelo);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(v->modelo, temp);

    printf("Valor ('%.2f'): R$ ", v->valor);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        float nVal;
        if (sscanf(temp, "%f", &nVal) == 1 && nVal > 0) v->valor = nVal;
    }
}

void apagarVeiculo(Veiculo **veiculos, int *total, int *cap, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    char placa[20];
    printf("Digite a Placa do veiculo a remover: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    int idx = buscarVeiculo(*veiculos, *total, placa);
    if (idx == -1) {
        printf("Veiculo nao encontrado.\n");
        return;
    }

    if ((*veiculos)[idx].status != 0) {
        printf("[ERRO] Nao e possivel apagar veiculo alugado, vendido ou em manutencao!\n");
        return;
    }

    for (int i = idx; i < *total - 1; i++) (*veiculos)[i] = (*veiculos)[i + 1];
    (*total)--;
    printf("Veiculo removido do estoque!\n");

    if (*total > 0 && *total <= *cap / 4 && *cap / 2 >= 2) {
        *cap /= 2;
        *veiculos = (Veiculo *) realloc(*veiculos, *cap * sizeof(Veiculo));
    }
}

void menuVeiculos(Veiculo **veiculos, int *total, int *cap, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|             MENU VEICULOS        |");
        printf("\n====================================");
        printf("\n| 1 - Cadastrar veiculo            |");
        printf("\n| 2 - Listar veiculos disponiveis  |");
        printf("\n| 3 - Procurar veiculos            |");
        printf("\n| 4 - Editar veiculo               |");
        printf("\n| 5 - Apagar veiculo               |");
        printf("\n| 0 - Sair                         |");
        printf("\n====================================");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            if (*total == *cap) {
                *cap *= 2;
                *veiculos = (Veiculo *) realloc(*veiculos, *cap * sizeof(Veiculo));
            }
            cadastrarVeiculo(&(*veiculos)[*total], *veiculos, *total);
            (*total)++;
            printf("\nVeiculo cadastrado com sucesso!\n");
        } else if (opcao == 2) {
            listarVeiculosDisponiveis(*veiculos, *total);
        } else if (opcao == 3) {
            procurarVeiculosAvancado(*veiculos, *total);
        } else if (opcao == 4) {
            char placa[20];
            printf("Placa do veiculo a editar: ");
            fgets(placa, sizeof(placa), stdin);
            placa[strcspn(placa, "\n")] = '\0';
            int idx = buscarVeiculo(*veiculos, *total, placa);
            if (idx != -1) {
                editarVeiculo(&(*veiculos)[idx]);
                printf("Veiculo atualizado!\n");
            } else printf("Veiculo nao encontrado.\n");
        } else if (opcao == 5) {
            apagarVeiculo(veiculos, total, cap, vendas, totalVendas, locacoes, totalLocacoes);
        }
    } while (opcao != 0);
}