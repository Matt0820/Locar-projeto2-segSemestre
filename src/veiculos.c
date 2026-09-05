#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/util.h"


// =======================================================
//                    MÓDULO DE VEÍCULOS
// =======================================================

void cadastrarVeiculo(Veiculo *v) {
    printf("\nMarca: ");
    fgets(v->marca, sizeof(v->marca), stdin);
    v->marca[strcspn(v->marca, "\n")] = '\0';

    printf("Modelo: ");
    fgets(v->modelo, sizeof(v->modelo), stdin);
    v->modelo[strcspn(v->modelo, "\n")] = '\0';

    printf("Ano: ");
    scanf("%d", &v->ano);
    limparBuffer();

    printf("Placa: ");
    fgets(v->placa, sizeof(v->placa), stdin);
    v->placa[strcspn(v->placa, "\n")] = '\0';

    printf("Valor Venda R$: ");
    scanf("%f", &v->valor);
    
    printf("Quilometragem atual: ");
    scanf("%d", &v->km);
    
    printf("Valor Diaria Locacao R$: ");
    scanf("%f", &v->valorDiaria);
    limparBuffer();
    
    v->status = 0; // Disponível por padrão
}

void listarVeiculos(const Veiculo veiculos[], int total, int filtro) {
    // filtro: -1 = Todos, 0 = Disponivel, 1 = Alugado, 2 = Vendido
    if (total == 0) {
        printf("\nNenhum veiculo cadastrado no estoque.\n");
        return;
    }

    char tituloFiltro[20];
    if (filtro == 0) strcpy(tituloFiltro, "Disponiveis");
    else if (filtro == 1) strcpy(tituloFiltro, "Alugados");
    else if (filtro == 2) strcpy(tituloFiltro, "Vendidos");
    else strcpy(tituloFiltro, "Todos");

    printf("\n=== Estoque de Veiculos - Filtro: %s ===\n", tituloFiltro);

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (filtro != -1 && veiculos[i].status != filtro) continue;

        char statusStr[20];
        if (veiculos[i].status == 0) strcpy(statusStr, "Disponivel");
        else if (veiculos[i].status == 1) strcpy(statusStr, "Alugado");
        else strcpy(statusStr, "Vendido");

        printf("%d) %s %s (%d) | Placa: %-8s | KM: %d | Status: %s\n", 
               i + 1, veiculos[i].marca, veiculos[i].modelo, veiculos[i].ano, 
               veiculos[i].placa, veiculos[i].km, statusStr);
        encontrados++;
    }

    if (encontrados == 0) {
        printf("Nenhum veiculo encontrado para este filtro.\n");
    }
}

int buscarVeiculo(const Veiculo veiculos[], int total, const char placa[]) {
    for (int i = 0; i < total; i++) {
        if (strcmp(veiculos[i].placa, placa) == 0) return i;
    }
    return -1;
}

void editarVeiculo(Veiculo *v) {
    char temp[100];
    printf("\n--- Edicao de Veiculo (Pressione ENTER para manter o valor atual) ---\n");

    printf("Marca ('%s'): ", v->marca);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(v->marca, temp);

    printf("Modelo ('%s'): ", v->modelo);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(v->modelo, temp);

    printf("Ano ('%d'): ", v->ano);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        int novoAno;
        if (sscanf(temp, "%d", &novoAno) == 1) v->ano = novoAno;
    }

    printf("Placa ('%s'): ", v->placa);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(v->placa, temp);

    printf("Valor ('%.2f'): R$ ", v->valor);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        float novoValor;
        if (sscanf(temp, "%f", &novoValor) == 1) v->valor = novoValor;
    }
}

void apagarVeiculo(Veiculo **veiculos, int *total, int *cap) {
    char placa[20];
    printf("\nDigite a Placa do veiculo a ser removido: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    int idx = buscarVeiculo(*veiculos, *total, placa);
    if (idx == -1) {
        printf("\nVeiculo nao encontrado no estoque.\n");
        return;
    }

    if ((*veiculos)[idx].status == 1) {
        printf("\n[ERRO] Nao e possivel apagar: este veiculo esta atualmente ALUGADO.\n");
        return;
    }

    if ((*veiculos)[idx].status == 2) {
        printf("\n[ERRO] Nao e possivel apagar: este veiculo esta atualmente VENDIDO.\n");
        return;
    }

    for (int i = idx; i < *total - 1; i++) (*veiculos)[i] = (*veiculos)[i + 1];
    (*total)--;
    printf("\nVeiculo removido do estoque!\n");

    if (*total > 0 && *total <= *cap / 4 && *cap / 2 >= 2) {
        *cap /= 2;
        *veiculos = (Veiculo *) realloc(*veiculos, *cap * sizeof(Veiculo));
    }
}

void menuVeiculos(Veiculo **veiculos, int *total, int *cap) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|             MENU VEICULOS        |");
        printf("\n====================================");
        printf("\n| 1 - Cadastrar veiculos           |");
        printf("\n| 2 - Listar veiculos              |");
        printf("\n| 3 - Buscar veiculos              |");
        printf("\n| 4 - Editar veiculos              |");
        printf("\n| 5 - Apagar veiculos              |");
        printf("\n| 0 - Sair                         |");
        printf("\n====================================");
        printf("\n[Status] Capacidade: %d | Cadastrados: %d", *cap, *total);
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            if (*total == *cap) {
                *cap *= 2;
                *veiculos = (Veiculo *) realloc(*veiculos, *cap * sizeof(Veiculo));
            }
            cadastrarVeiculo(&(*veiculos)[*total]);
            (*total)++;
            printf("\nVeiculo adicionado ao estoque!\n");
        } else if (opcao == 2) {
            int subOpcao;
            printf("\n--- LISTAR VEICULOS ---");
            printf("\n1 - Disponiveis");
            printf("\n2 - Alugados");
            printf("\n3 - Vendidos");
            printf("\n4 - Todos");
            printf("\n0 - Voltar");
            printf("\nOpcao: ");
            scanf("%d", &subOpcao);
            limparBuffer();

            if (subOpcao == 1) listarVeiculos(*veiculos, *total, 0);
            else if (subOpcao == 2) listarVeiculos(*veiculos, *total, 1);
            else if (subOpcao == 3) listarVeiculos(*veiculos, *total, 2);
            else if (subOpcao == 4) listarVeiculos(*veiculos, *total, -1);
        } else if (opcao == 3) {
            char placa[20];
            printf("Digite a Placa: ");
            fgets(placa, sizeof(placa), stdin);
            placa[strcspn(placa, "\n")] = '\0';
            int idx = buscarVeiculo(*veiculos, *total, placa);
            if (idx != -1) {
                printf("\nVeiculo: %s %s (%d) | Placa: %s | Valor Venda: R$ %.2f | Valor Diaria: R$ %.2f\n", 
                       (*veiculos)[idx].marca, (*veiculos)[idx].modelo, (*veiculos)[idx].ano,
                       (*veiculos)[idx].placa, (*veiculos)[idx].valor, (*veiculos)[idx].valorDiaria);
            } else {
                printf("\nVeiculo nao encontrado no estoque.\n");
            }
        } else if (opcao == 4) {
            char placa[20];
            printf("Digite a Placa para editar: ");
            fgets(placa, sizeof(placa), stdin);
            placa[strcspn(placa, "\n")] = '\0';
            int idx = buscarVeiculo(*veiculos, *total, placa);
            if (idx != -1) {
                editarVeiculo(&(*veiculos)[idx]);
                printf("\nVeiculo atualizado com sucesso!\n");
            } else {
                printf("\nVeiculo nao encontrado.\n");
            }
        } else if (opcao == 5) {
            apagarVeiculo(veiculos, total, cap);
        }
    } while (opcao != 0);
}
