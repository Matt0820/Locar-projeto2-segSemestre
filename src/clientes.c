#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/modelos.h"
#include "../include/util.h"
#include "../include/clientes.h"

static int cpfApenasNumeros(const char *cpf) {
    if (strlen(cpf) == 0) return 0;
    for (int i = 0; cpf[i] != '\0'; i++) {
        if (!isdigit(cpf[i])) return 0;
    }
    return 1;
}

static int cpfJaExiste(const Cliente clientes[], int total, const char *cpf) {
    for (int i = 0; i < total; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) return 1;
    }
    return 0;
}

void cadastrarCliente(Cliente *c, const Cliente clientes[], int totalClientes) {
    printf("\n--- Novo Cadastro de Cliente ---\n");
    
    printf("Nome completo: ");
    fgets(c->nome, sizeof(c->nome), stdin);
    c->nome[strcspn(c->nome, "\n")] = '\0';

    do {
        printf("CPF (Apenas numeros): ");
        fgets(c->cpf, sizeof(c->cpf), stdin);
        c->cpf[strcspn(c->cpf, "\n")] = '\0';

        if (!cpfApenasNumeros(c->cpf)) {
            printf("[ERRO] O CPF deve conter apenas numeros!\n");
        } else if (cpfJaExiste(clientes, totalClientes, c->cpf)) {
            printf("[ERRO] Este CPF ja esta cadastrado no sistema!\n");
        } else {
            break;
        }
    } while (1);

    int anoAtual = 2026;
    do {
        printf("Ano de Nascimento (AAAA): ");
        scanf("%d", &c->anoNascimento);
        limparBuffer();

        if (anoAtual - c->anoNascimento < 18) {
            printf("[ERRO] O cliente deve ter no minimo 18 anos!\n");
        } else {
            break;
        }
    } while (1);

    printf("Data de Nascimento (DD/MM/AAAA): ");
    fgets(c->dataNascimento, sizeof(c->dataNascimento), stdin);
    c->dataNascimento[strcspn(c->dataNascimento, "\n")] = '\0';

    printf("Estado Civil: ");
    fgets(c->estadoCivil, sizeof(c->estadoCivil), stdin);
    c->estadoCivil[strcspn(c->estadoCivil, "\n")] = '\0';

    printf("Telefone: ");
    fgets(c->telefone, sizeof(c->telefone), stdin);
    c->telefone[strcspn(c->telefone, "\n")] = '\0';

    printf("E-mail: ");
    fgets(c->email, sizeof(c->email), stdin);
    c->email[strcspn(c->email, "\n")] = '\0';

    printf("Profissao: ");
    fgets(c->profissao, sizeof(c->profissao), stdin);
    c->profissao[strcspn(c->profissao, "\n")] = '\0';

    printf("Renda Mensal: R$ ");
    scanf("%f", &c->rendaMensal);
    limparBuffer();

    printf("\n-- Endereco do Cliente --\n");
    printf("Rua/Av: ");
    fgets(c->endereco.rua, sizeof(c->endereco.rua), stdin);
    c->endereco.rua[strcspn(c->endereco.rua, "\n")] = '\0';

    printf("Numero: ");
    scanf("%d", &c->endereco.numero);
    limparBuffer();

    printf("Complemento: ");
    fgets(c->endereco.complemento, sizeof(c->endereco.complemento), stdin);
    c->endereco.complemento[strcspn(c->endereco.complemento, "\n")] = '\0';

    printf("Bairro: ");
    fgets(c->endereco.bairro, sizeof(c->endereco.bairro), stdin);
    c->endereco.bairro[strcspn(c->endereco.bairro, "\n")] = '\0';

    printf("Cidade: ");
    fgets(c->endereco.cidade, sizeof(c->endereco.cidade), stdin);
    c->endereco.cidade[strcspn(c->endereco.cidade, "\n")] = '\0';

    printf("UF: ");
    fgets(c->endereco.uf, sizeof(c->endereco.uf), stdin);
    c->endereco.uf[strcspn(c->endereco.uf, "\n")] = '\0';

    printf("CEP: ");
    fgets(c->endereco.cep, sizeof(c->endereco.cep), stdin);
    c->endereco.cep[strcspn(c->endereco.cep, "\n")] = '\0';
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
    printf("Nome: %s | CPF: %s\n", c->nome, c->cpf);
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
    
    printf("Nome completo ('%s'): ", c->nome);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->nome, temp);

    printf("Telefone ('%s'): ", c->telefone);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->telefone, temp);

    printf("E-mail ('%s'): ", c->email);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->email, temp);

    printf("Profissao ('%s'): ", c->profissao);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->profissao, temp);
}

void apagarCliente(Cliente **clientes, int *total, int *cap, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes) {
    char busca[80];
    printf("Digite o CPF ou Nome para remover: ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0';

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
        printf("\n| 0 - Sair                         |");
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
            fgets(busca, sizeof(busca), stdin);
            busca[strcspn(busca, "\n")] = '\0';
            int idx = buscarCliente(*clientes, *total, busca);
            if (idx != -1) exibirDetalhesCliente(&(*clientes)[idx]);
            else printf("\nCliente nao encontrado.\n");
        } else if (opcao == 4) {
            char busca[80];
            printf("Digite o CPF ou Nome para editar: ");
            fgets(busca, sizeof(busca), stdin);
            busca[strcspn(busca, "\n")] = '\0';
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