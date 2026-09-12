#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include "../include/veiculos.h"

static int placaValida(const char *placa) {
    int len = strlen(placa);
    // Aceita formato antigo (ABC1234, 7 caracteres) e o formato Mercosul (ABC1D23, 7 caracteres).
    if (len != 7) return 0;

    for (int i = 0; placa[i] != '\0'; i++) {
        if (!isalnum((unsigned char) placa[i])) return 0;
    }
    return 1;
}

float calcularValorDepreciado(const Veiculo *v) {
    int anoAtual = anoAtualDoSistema();
    int idade = anoAtual - v->ano;
    if (idade < 0) idade = 0;

    // Depreciacao linear de 10% ao ano sobre o valor de compra, limitada a no maximo 70% de desvalorizacao total (o veiculo nunca vale menos que 30% do valor pago, evitando valores irreais para carros antigos).
    const float taxaAnual = 0.10f;
    const float fatorMinimo = 0.30f;

    float fator = 1.0f - (taxaAnual * idade);
    if (fator < fatorMinimo) fator = fatorMinimo;

    return v->valorCompra * fator;
}

void cadastrarVeiculo(Veiculo *v, const Veiculo veiculos[], int totalVeiculos) {
    printf("\n--- Novo Cadastro de Veiculo ---\n");
    
    do {
        printf("Placa (Ex: ABC1D23): ");
        lerLinha(v->placa, sizeof(v->placa));

        if (!placaValida(v->placa)) {
            printf("[ERRO] Placa invalida! Digite 7 caracteres alfanumericos (Ex: ABC1D23).\n");
        } else if (buscarVeiculo(veiculos, totalVeiculos, v->placa) != -1) {
            printf("[ERRO] Esta placa ja esta cadastrada no sistema!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("Marca: ");
        lerLinha(v->marca, sizeof(v->marca));

        if (strlen(v->marca) == 0) {
            printf("[ERRO] A marca nao pode ficar em branco!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("Modelo: ");
        lerLinha(v->modelo, sizeof(v->modelo));

        if (strlen(v->modelo) == 0) {
            printf("[ERRO] O modelo nao pode ficar em branco!\n");
        } else {
            break;
        }
    } while (1);

    int anoAtual = anoAtualDoSistema();
    do {
        printf("Ano: ");
        scanf("%d", &v->ano);
        limparBuffer();

        if (v->ano < 1950 || v->ano > anoAtual + 1) {
            printf("[ERRO] Ano invalido! Digite um ano entre 1950 e %d.\n", anoAtual + 1);
        } else {
            break;
        }
    } while (1);

    do {
        printf("Valor de Compra R$: ");
        scanf("%f", &v->valorCompra);
        limparBuffer();

        if (v->valorCompra <= 0) {
            printf("[ERRO] O valor de compra deve ser maior que zero!\n");
        } else {
            break;
        }
    } while (1);
 
    v->valor = calcularValorDepreciado(v);

    do {
        printf("Quilometragem atual: ");
        scanf("%d", &v->km);

        if (v->km < 0) {
            printf("[ERRO] A quilometragem nao pode ser negativa!\n");
        } else {
            break;
        }
    } while (1);
    
    do {
        printf("Valor Diaria Locacao R$: ");
        scanf("%f", &v->valorDiaria);

        if (v->valorDiaria <= 0) {
            printf("[ERRO] O valor da diaria deve ser maior que zero!\n");
        } else {
            break;
        }
    } while (1);
    limparBuffer();
    
    v->status = 0; // Disponível por padrão
}

void listarVeiculosDisponiveis(const Veiculo veiculos[], int total) {
    int encontrados = 0;
    printf("\n=== Veiculos Disponiveis no Estoque ===\n");
    for (int i = 0; i < total; i++) {
        if (veiculos[i].status == 0) {
            printf("%d) %s %s (%d) | Placa: %s | Valor de Mercado (est.): R$ %.2f | KM: %d\n", 
                   i + 1, veiculos[i].marca, veiculos[i].modelo, veiculos[i].ano, 
                   veiculos[i].placa, calcularValorDepreciado(&veiculos[i]), veiculos[i].km);
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
    printf("\n4 - Por Valor de Mercado (Maior, Menor ou Igual)");
    printf("\nOpcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 1) {
        char modelo[50];
        printf("Digite o Modelo: ");
        lerLinha(modelo, sizeof(modelo));
        for (int i = 0; i < total; i++) {
            if (strstr(veiculos[i].modelo, modelo) != NULL) {
                printf("Placa: %s | %s %s | Valor de Mercado (est.): R$%.2f\n", veiculos[i].placa, veiculos[i].marca, veiculos[i].modelo, calcularValorDepreciado(&veiculos[i]));
            }
        }
    } else if (opcao == 2) {
        char marca[50];
        printf("Digite a Marca: ");
        lerLinha(marca, sizeof(marca));
        for (int i = 0; i < total; i++) {
            if (strstr(veiculos[i].marca, marca) != NULL) {
                printf("Placa: %s | %s %s | Valor de Mercado (est.): R$%.2f\n", veiculos[i].placa, veiculos[i].marca, veiculos[i].modelo, calcularValorDepreciado(&veiculos[i]));
            }
        }
    } else if (opcao == 3) {
        char placa[20];
        printf("Digite a Placa: ");
        lerLinha(placa, sizeof(placa));
        int idx = buscarVeiculo(veiculos, total, placa);
        if (idx != -1) {
            printf("Placa: %s | %s %s | Valor de Mercado (est.): R$%.2f\n", veiculos[idx].placa, veiculos[idx].marca, veiculos[idx].modelo, calcularValorDepreciado(&veiculos[idx]));
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
            float valorAtual = calcularValorDepreciado(&veiculos[i]);
            if ((comp == 1 && valorAtual > valorRef) ||
                (comp == 2 && valorAtual < valorRef) ||
                (comp == 3 && valorAtual == valorRef)) {
                printf("Placa: %s | %s %s | Valor de Mercado (est.): R$%.2f\n", veiculos[i].placa, veiculos[i].marca, veiculos[i].modelo, valorAtual);
            }
        }
    }
}

void editarVeiculo(Veiculo *v) {
    char temp[100];
    printf("\n--- Edicao de Veiculo (ENTER para manter) ---\n");

    printf("Marca ('%s'): ", v->marca);
    lerLinha(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(v->marca, temp);

    printf("Modelo ('%s'): ", v->modelo);
    lerLinha(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(v->modelo, temp);

    printf("Valor de Compra ('%.2f'): R$ ", v->valorCompra);
    lerLinha(temp, sizeof(temp));
    if (strlen(temp) > 0) {
        float nVal;
        if (sscanf(temp, "%f", &nVal) == 1 && nVal > 0) {
            v->valorCompra = nVal;
            v->valor = calcularValorDepreciado(v); // reajusta o valor
        } else {
            printf("[AVISO] Valor invalido informado; valor anterior foi mantido.\n");
        }
    }
}

void apagarVeiculo(Veiculo **veiculos, int *total, int *cap, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    char placa[20];
    printf("Digite a Placa do veiculo a remover: ");
    lerLinha(placa, sizeof(placa));

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
        printf("\n| 0 - Voltar ao Menu Principal     |");
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
            lerLinha(placa, sizeof(placa));
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
