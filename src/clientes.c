#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/util.h"

// =======================================================
//                    MÓDULO DE CLIENTES
// =======================================================

void cadastrarCliente(Cliente *c) {
    printf("\n--- Novo Cadastro de Cliente ---\n");
    printf("Nome completo: ");
    fgets(c->nome, sizeof(c->nome), stdin);
    c->nome[strcspn(c->nome, "\n")] = '\0';

    printf("CPF: ");
    fgets(c->cpf, sizeof(c->cpf), stdin);
    c->cpf[strcspn(c->cpf, "\n")] = '\0';

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
    for (int i = 0; i < total; i++) {
        if (strcmp(busca, clientes[i].cpf) == 0 || strcmp(busca, clientes[i].nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Verifica se o cliente possui alguma venda registrada em seu historico
int clienteTemVendaAssociada(const Venda vendas[], int totalVendas, const char cpf[]) {
    for (int i = 0; i < totalVendas; i++) {
        if (strcmp(vendas[i].cliente.cpf, cpf) == 0) {
            return 1;
        }
    }
    return 0;
}

// Verifica se o cliente possui alguma locacao com status Ativa (1)
int clienteTemLocacaoAtiva(const Locacao locacoes[], int totalLocacoes, const char cpf[]) {
    for (int i = 0; i < totalLocacoes; i++) {
        if (strcmp(locacoes[i].cpfCliente, cpf) == 0 && locacoes[i].status == 1) {
            return 1;
        }
    }
    return 0;
}

void editarCliente(Cliente *c) {
    char temp[100];
    printf("\n--- Edicao de Cliente (Pressione a tecla ENTER para manter o valor atual) ---\n");
    
    printf("Nome completo ('%s'): ", c->nome);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->nome, temp);

    printf("CPF ('%s'): ", c->cpf);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->cpf, temp);

    printf("Data de Nascimento ('%s'): ", c->dataNascimento);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->dataNascimento, temp);

    printf("Estado Civil ('%s'): ", c->estadoCivil);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->estadoCivil, temp);

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

    printf("Renda Mensal ('%.2f'): R$ ", c->rendaMensal);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        float novaRenda;
        if (sscanf(temp, "%f", &novaRenda) == 1) c->rendaMensal = novaRenda;
    }

    printf("\n-- Edicao de Endereco --\n");

    printf("Rua/Av ('%s'): ", c->endereco.rua);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->endereco.rua, temp);

    printf("Numero ('%d'): ", c->endereco.numero);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        int novoNum;
        if (sscanf(temp, "%d", &novoNum) == 1) c->endereco.numero = novoNum;
    }

    printf("Complemento ('%s'): ", c->endereco.complemento);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->endereco.complemento, temp);

    printf("Bairro ('%s'): ", c->endereco.bairro);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->endereco.bairro, temp);

    printf("Cidade ('%s'): ", c->endereco.cidade);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->endereco.cidade, temp);

    printf("UF ('%s'): ", c->endereco.uf);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->endereco.uf, temp);

    printf("CEP ('%s'): ", c->endereco.cep);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->endereco.cep, temp);
}

void menuClientes(Cliente **clientes, int *total, int *cap,
                   const Venda vendas[], int totalVendas,
                   const Locacao locacoes[], int totalLocacoes) {
    int opcao;
    do {
        printf("\n====================================");
        printf("\n|           MENU CLIENTE           |");
        printf("\n====================================");
        printf("\n| 1 - Cadastrar pessoa             |");
        printf("\n| 2 - Listar pessoas               |");
        printf("\n| 3 - Buscar pessoa (Nome ou CPF)  |");
        printf("\n| 4 - Editar pessoa (Nome ou CPF)  |");
        printf("\n| 5 - Apagar pessoa (Nome ou CPF)  |");
        printf("\n| 0 - Sair                         |");
        printf("\n====================================");
        printf("\n[Status] Capacidade: %d | Cadastrados: %d", *cap, *total);
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            if (*total == *cap) {
                *cap *= 2;
                *clientes = (Cliente *) realloc(*clientes, *cap * sizeof(Cliente));
            }
            cadastrarCliente(&(*clientes)[*total]);
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
            if (idx != -1) {
                exibirDetalhesCliente(&(*clientes)[idx]);
            } else {
                printf("\nCliente nao encontrado.\n");
            }
        } else if (opcao == 4) {
            char busca[80];
            printf("Digite o CPF ou Nome para editar: ");
            fgets(busca, sizeof(busca), stdin);
            busca[strcspn(busca, "\n")] = '\0';
            int idx = buscarCliente(*clientes, *total, busca);
            if (idx != -1) {
                editarCliente(&(*clientes)[idx]);
                printf("\nCadastro do cliente atualizado com sucesso!\n");
            } else {
                printf("\nCliente nao encontrado.\n");
            }
        } else if (opcao == 5) {
            char busca[80];
            printf("Digite o CPF ou Nome para remover: ");
            fgets(busca, sizeof(busca), stdin);
            busca[strcspn(busca, "\n")] = '\0';
            int idx = buscarCliente(*clientes, *total, busca);
            if (idx != -1) {
                if (clienteTemVendaAssociada(vendas, totalVendas, (*clientes)[idx].cpf)) {
                    printf("\n[ERRO] Nao e possivel apagar: este cliente ja comprou um veiculo (existe no historico de vendas).\n");
                } else if (clienteTemLocacaoAtiva(locacoes, totalLocacoes, (*clientes)[idx].cpf)) {
                    printf("\n[ERRO] Nao e possivel apagar: este cliente possui uma locacao ATIVA no momento.\n");
                } else {
                    for (int i = idx; i < *total - 1; i++) (*clientes)[i] = (*clientes)[i + 1];
                    (*total)--;
                    printf("\nCliente removido!\n");

                    if (*total > 0 && *total <= *cap / 4 && *cap / 2 >= 2) {
                        *cap /= 2;
                        *clientes = (Cliente *) realloc(*clientes, *cap * sizeof(Cliente));
                    }
                }
            } else {
                printf("\nCliente nao encontrado.\n");
            }
        }
    } while (opcao != 0);
}
