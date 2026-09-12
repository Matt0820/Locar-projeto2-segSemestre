#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include "../include/clientes.h"

static int cpfJaExiste(const Cliente clientes[], int total, const char *cpf) {
    for (int i = 0; i < total; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) return 1;
    }
    return 0;
}

static int cnhJaExiste(const Cliente clientes[], int total, const char *cnh) {
    for (int i = 0; i < total; i++) {
        if (strcmp(clientes[i].cnh, cnh) == 0) return 1;
    }
    return 0;
}

void cadastrarCliente(Cliente *c, const Cliente clientes[], int totalClientes) {
    printf("\n--- Novo Cadastro de Cliente ---\n");

    do {
        printf("Nome completo: ");
        lerLinha(c->nome, sizeof(c->nome));

        if (!somenteLetras(c->nome)) {
            printf("[ERRO] O nome deve conter apenas letras e espacos!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("CPF (Apenas numeros): ");
        lerLinha(c->cpf, sizeof(c->cpf));

        if (!cpfValido(c->cpf)) {
            printf("[ERRO] CPF invalido! Digite 11 numeros correspondentes a um CPF real.\n");
        } else if (cpfJaExiste(clientes, totalClientes, c->cpf)) {
            printf("[ERRO] Este CPF ja esta cadastrado no sistema!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("CNH (Numero de Registro, 11 digitos): ");
        lerLinha(c->cnh, sizeof(c->cnh));

        if (!cnhValida(c->cnh)) {
            printf("[ERRO] CNH invalida! Digite 11 numeros.\n");
        } else if (cnhJaExiste(clientes, totalClientes, c->cnh)) {
            printf("[ERRO] Esta CNH ja esta cadastrada no sistema!\n");
        } else {
            break;
        }
    } while (1);

    int anoAtual = anoAtualDoSistema();
    do {
        printf("Data de Nascimento (DD/MM/AAAA): ");
        lerLinha(c->dataNascimento, sizeof(c->dataNascimento));

        if (!dataValida(c->dataNascimento)) {
            printf("[ERRO] Data invalida! Use o formato DD/MM/AAAA.\n");
        } else if (anoAtual - anoDaData(c->dataNascimento) < 18) {
            printf("[ERRO] O cliente deve ter no minimo 18 anos!\n");
        } else {
            break;
        }
    } while (1);

    c->anoNascimento = anoDaData(c->dataNascimento);

    do {
        printf("Estado Civil: ");
        lerLinha(c->estadoCivil, sizeof(c->estadoCivil));

        if (!somenteLetras(c->estadoCivil)) {
            printf("[ERRO] Informe apenas letras!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("Telefone (Apenas numeros, com DDD): ");
        lerLinha(c->telefone, sizeof(c->telefone));

        if (!somenteDigitos(c->telefone) || strlen(c->telefone) < 10 || strlen(c->telefone) > 11) {
            printf("[ERRO] Telefone invalido! Digite 10 ou 11 numeros (com DDD).\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("E-mail: ");
        lerLinha(c->email, sizeof(c->email));

        if (!emailValido(c->email)) {
            printf("[ERRO] E-mail invalido! Use o formato usuario@dominio.com.\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("Profissao: ");
        lerLinha(c->profissao, sizeof(c->profissao));

        if (!somenteLetras(c->profissao)) {
            printf("[ERRO] Informe apenas letras!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("Renda Mensal: R$ ");
        scanf("%f", &c->rendaMensal);
        limparBuffer();

        if (c->rendaMensal <= 0) {
            printf("[ERRO] A renda mensal deve ser maior que zero!\n");
        } else {
            break;
        }
    } while (1);

    printf("\n-- Endereco do Cliente --\n");
    printf("Rua/Av: ");
    lerLinha(c->endereco.rua, sizeof(c->endereco.rua));

    do {
        printf("Numero: ");
        scanf("%d", &c->endereco.numero);
        limparBuffer();

        if (c->endereco.numero <= 0) {
            printf("[ERRO] O numero deve ser maior que zero!\n");
        } else {
            break;
        }
    } while (1);

    printf("Complemento: ");
    lerLinha(c->endereco.complemento, sizeof(c->endereco.complemento));

    printf("Bairro: ");
    lerLinha(c->endereco.bairro, sizeof(c->endereco.bairro));

    do {
        printf("Cidade: ");
        lerLinha(c->endereco.cidade, sizeof(c->endereco.cidade));

        if (!somenteLetras(c->endereco.cidade)) {
            printf("[ERRO] Informe apenas letras!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("UF (Sigla, ex: SP): ");
        lerLinha(c->endereco.uf, sizeof(c->endereco.uf));

        if (!somenteLetras(c->endereco.uf) || strlen(c->endereco.uf) != 2) {
            printf("[ERRO] Informe a sigla do estado com 2 letras!\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("CEP (Apenas numeros): ");
        lerLinha(c->endereco.cep, sizeof(c->endereco.cep));

        if (!somenteDigitos(c->endereco.cep) || strlen(c->endereco.cep) != 8) {
            printf("[ERRO] CEP invalido! Digite 8 numeros.\n");
        } else {
            break;
        }
    } while (1);
}

void listarClientes(const Cliente clientes[], int total) {
    if (total == 0) {
        printf("\nNenhum cliente cadastrado.\n");
        return;
    }
    printf("\n=== Lista de Clientes (%d) ===\n", total);
    for (int i = 0; i < total; i++) {
        printf("%d) Nome: %s | CPF: %s | Tel: %s\n", 
               i + 1, clientes[i].nome, clientes[i].cpf, clientes[i].telefone);
    }
}

void exibirDetalhesCliente(const Cliente *c) {
    printf("\n=== Detalhes do Cliente Encontrado ===\n");
    printf("Nome: %s | CPF: %s | CNH: %s\n", c->nome, c->cpf, c->cnh);
    printf("Nascimento: %s | Estado Civil: %s | Profissao: %s\n", c->dataNascimento, c->estadoCivil, c->profissao);
    printf("Contato: Tel %s | Email: %s | Renda: R$ %.2f\n", c->telefone, c->email, c->rendaMensal);
    printf("Endereco: %s, %d (%s) - Bairro %s, %s/%s - CEP: %s\n", 
           c->endereco.rua, c->endereco.numero, c->endereco.complemento,
           c->endereco.bairro, c->endereco.cidade, c->endereco.uf, c->endereco.cep);
}

int buscarCliente(const Cliente clientes[], int total, const char busca[]) {
    int indicesEncontrados[100];
    int qtdEncontrados = 0;

    for (int i = 0; i < total; i++) {
        if (strcmp(busca, clientes[i].cpf) == 0) {
            return i;
        }
        if (strstr(clientes[i].nome, busca) != NULL) {
            indicesEncontrados[qtdEncontrados++] = i;
        }
    }

    if (qtdEncontrados == 1) {
        return indicesEncontrados[0];
    } else if (qtdEncontrados > 1) {
        printf("\nForam encontrados múltiplos clientes com esse nome:\n");
        for (int i = 0; i < qtdEncontrados; i++) {
            int idx = indicesEncontrados[i];
            printf("%d) Nome: %s | CPF: %s\n", i + 1, clientes[idx].nome, clientes[idx].cpf);
        }
        int escolha;
        printf("Selecione qual cliente deseja (1 a %d): ", qtdEncontrados);
        scanf("%d", &escolha);
        limparBuffer();
        if (escolha >= 1 && escolha <= qtdEncontrados) {
            return indicesEncontrados[escolha - 1];
        }
    }

    return -1;
}

int clienteTemVendaAssociada(const Venda vendas[], int totalVendas, const char cpf[]) {
    for (int i = 0; i < totalVendas; i++) {
        if (strcmp(vendas[i].cliente.cpf, cpf) == 0) return 1;
    }
    return 0;
}

int clienteTemLocacaoAtiva(const Locacao locacoes[], int totalLocacoes, const char cpf[]) {
    for (int i = 0; i < totalLocacoes; i++) {
        if (strcmp(locacoes[i].cpfCliente, cpf) == 0) return 1;
    }
    return 0;
}

void editarCliente(Cliente *c) {
    char temp[100];
    printf("\n--- Edicao de Cliente (ENTER para manter valor) ---\n");

    do {
        printf("Nome completo ('%s'): ", c->nome);
        lerLinha(temp, sizeof(temp));

        if (strlen(temp) == 0) {
            break;
        } else if (!somenteLetras(temp)) {
            printf("[ERRO] O nome deve conter apenas letras e espacos!\n");
        } else {
            strcpy(c->nome, temp);
            break;
        }
    } while (1);

    do {
        printf("Telefone ('%s'): ", c->telefone);
        lerLinha(temp, sizeof(temp));

        if (strlen(temp) == 0) {
            break;
        } else if (!somenteDigitos(temp) || strlen(temp) < 10 || strlen(temp) > 11) {
            printf("[ERRO] Telefone invalido! Digite 10 ou 11 numeros (com DDD).\n");
        } else {
            strcpy(c->telefone, temp);
            break;
        }
    } while (1);

    do {
        printf("E-mail ('%s'): ", c->email);
        lerLinha(temp, sizeof(temp));

        if (strlen(temp) == 0) {
            break;
        } else if (!emailValido(temp)) {
            printf("[ERRO] E-mail invalido! Use o formato usuario@dominio.com.\n");
        } else {
            strcpy(c->email, temp);
            break;
        }
    } while (1);

    do {
        printf("Profissao ('%s'): ", c->profissao);
        lerLinha(temp, sizeof(temp));

        if (strlen(temp) == 0) {
            break;
        } else if (!somenteLetras(temp)) {
            printf("[ERRO] Informe apenas letras!\n");
        } else {
            strcpy(c->profissao, temp);
            break;
        }
    } while (1);
}

void apagarCliente(Cliente **clientes, int *total, int *cap, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    char busca[80];
    printf("Digite o CPF ou Nome para remover: ");
    lerLinha(busca, sizeof(busca));

    int idx = buscarCliente(*clientes, *total, busca);
    if (idx != -1) {
        if (clienteTemVendaAssociada(vendas, totalVendas, (*clientes)[idx].cpf)) {
            printf("\n[ERRO] Impossivel apagar: Cliente possui historico de vendas!\n");
        } else if (clienteTemLocacaoAtiva(locacoes, totalLocacoes, (*clientes)[idx].cpf)) {
            printf("\n[ERRO] Impossivel apagar: Cliente possui histórico de locacoes!\n");
        } else {
            for (int i = idx; i < *total - 1; i++) (*clientes)[i] = (*clientes)[i + 1];
            (*total)--;
            printf("\nCliente removido com sucesso!\n");
            if (*total > 0 && *total <= *cap / 4 && *cap / 2 >= 2) {
                *cap /= 2;
                *clientes = (Cliente *) realloc(*clientes, *cap * sizeof(Cliente));
            }
        }
    } else {
        printf("\nCliente nao encontrado.\n");
    }
}

void menuClientes(Cliente **clientes, int *total, int *cap, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|           MENU CLIENTE           |");
        printf("\n====================================");
        printf("\n| 1 - Cadastrar pessoa             |");
        printf("\n| 2 - Listar pessoas               |");
        printf("\n| 3 - Buscar pessoa                |");
        printf("\n| 4 - Editar pessoa                |");
        printf("\n| 5 - Apagar pessoa                |");
        printf("\n| 0 - Voltar ao Menu Principal     |");
        printf("\n====================================");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            if (*total == *cap) {
                *cap *= 2;
                *clientes = (Cliente *) realloc(*clientes, *cap * sizeof(Cliente));
            }
            cadastrarCliente(&(*clientes)[*total], *clientes, *total);
            (*total)++;
            printf("\nCliente cadastrado com sucesso!\n");
        } else if (opcao == 2) {
            listarClientes(*clientes, *total);
        } else if (opcao == 3) {
            char busca[80];
            printf("Digite o Nome ou CPF: ");
            lerLinha(busca, sizeof(busca));
            int idx = buscarCliente(*clientes, *total, busca);
            if (idx != -1) exibirDetalhesCliente(&(*clientes)[idx]);
            else printf("\nCliente nao encontrado.\n");
        } else if (opcao == 4) {
            char busca[80];
            printf("Digite o CPF ou Nome para editar: ");
            lerLinha(busca, sizeof(busca));
            int idx = buscarCliente(*clientes, *total, busca);
            if (idx != -1) {
                editarCliente(&(*clientes)[idx]);
                printf("\nCliente atualizado!\n");
            } else printf("\nCliente nao encontrado.\n");
        } else if (opcao == 5) {
            apagarCliente(clientes, total, cap, vendas, totalVendas, locacoes, totalLocacoes);
        }
    } while (opcao != 0);
}
