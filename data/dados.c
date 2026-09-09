#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/dados.h"

void salvarDadosCSV(const Cliente clientes[], int totalClientes,
                    const Veiculo veiculos[], int totalVeiculos,
                    const Venda vendas[], int totalVendas,
                    const Locacao locacoes[], int totalLocacoes,
                    const Manutencao manutencoes[], int totalManutencoes) {

    FILE *f;

    // 1. Salvar Clientes
    f = fopen("clientes.csv", "w");
    if (f) {
        for (int i = 0; i < totalClientes; i++) {
            fprintf(f, "%s;%s;%s;%d;%s;%s;%s;%f;%s;%s;%d;%s;%s;%s;%s;%s\n",
                    clientes[i].nome, clientes[i].cpf, clientes[i].dataNascimento,
                    clientes[i].anoNascimento, clientes[i].estadoCivil, clientes[i].telefone,
                    clientes[i].email, clientes[i].rendaMensal, clientes[i].profissao,
                    clientes[i].endereco.rua, clientes[i].endereco.numero, clientes[i].endereco.complemento,
                    clientes[i].endereco.bairro, clientes[i].endereco.cidade, clientes[i].endereco.uf,
                    clientes[i].endereco.cep);
        }
        fclose(f);
    }

    // 2. Salvar Veiculos
    f = fopen("veiculos.csv", "w");
    if (f) {
        for (int i = 0; i < totalVeiculos; i++) {
            fprintf(f, "%s;%s;%d;%s;%f;%f;%d;%f;%d\n",
                    veiculos[i].marca, veiculos[i].modelo, veiculos[i].ano,
                    veiculos[i].placa, veiculos[i].valor, veiculos[i].valorCompra,
                    veiculos[i].km, veiculos[i].valorDiaria, veiculos[i].status);
        }
        fclose(f);
    }

    // 3. Salvar Vendas
    f = fopen("vendas.csv", "w");
    if (f) {
        for (int i = 0; i < totalVendas; i++) {
            fprintf(f, "%d;%s;%s;%s;%f\n",
                    vendas[i].idVenda, vendas[i].veiculo.placa,
                    vendas[i].cliente.cpf, vendas[i].dataVenda, vendas[i].valorFinal);
        }
        fclose(f);
    }

    // 4. Salvar Locacoes
    f = fopen("locacoes.csv", "w");
    if (f) {
        for (int i = 0; i < totalLocacoes; i++) {
            fprintf(f, "%d;%s;%s;%s;%d;%d;%d;%d;%f;%f;%f;%d;%d\n",
                    locacoes[i].idLocacao, locacoes[i].placaVeiculo, locacoes[i].cpfCliente,
                    locacoes[i].dataInicio, locacoes[i].diasPrevistos, locacoes[i].diasUtilizados,
                    locacoes[i].kmInicial, locacoes[i].kmFinal, locacoes[i].valorDiaria,
                    locacoes[i].taxaAvariaMulta, locacoes[i].valorTotal, locacoes[i].formaPagamento,
                    locacoes[i].status);
        }
        fclose(f);
    }

    // 5. Salvar Manutencoes
    f = fopen("manutencoes.csv", "w");
    if (f) {
        for (int i = 0; i < totalManutencoes; i++) {
            fprintf(f, "%d;%s;%s;%s;%s;%f;%d\n",
                    manutencoes[i].idManutencao, manutencoes[i].placaVeiculo,
                    manutencoes[i].descricao, manutencoes[i].dataEntrada,
                    manutencoes[i].dataSaida, manutencoes[i].custo, manutencoes[i].status);
        }
        fclose(f);
    }

    printf("\n[DADOS GRAVADOS COM SUCESSO NOS ARQUIVOS CSV!]\n");
}

void carregarDadosCSV(Cliente **clientes, int *totalClientes, int *capClientes,
                      Veiculo **veiculos, int *totalVeiculos, int *capVeiculos,
                      Venda **vendas, int *totalVendas, int *capVendas,
                      Locacao **locacoes, int *totalLocacoes, int *capLocacoes,
                      Manutencao **manutencoes, int *totalManutencoes, int *capManutencoes) {

    FILE *f;
    char linha[512];

    // 1. Carregar Clientes
    f = fopen("clientes.csv", "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (*totalClientes == *capClientes) {
                *capClientes *= 2;
                *clientes = (Cliente *) realloc(*clientes, *capClientes * sizeof(Cliente));
            }
            Cliente c;
            sscanf(linha, "%79[^;];%19[^;];%19[^;];%d;%29[^;];%19[^;];%59[^;];%f;%49[^;];%59[^;];%d;%29[^;];%39[^;];%39[^;];%9[^;];%19[^\n]",
                   c.nome, c.cpf, c.dataNascimento, &c.anoNascimento, c.estadoCivil, c.telefone,
                   c.email, &c.rendaMensal, c.profissao, c.endereco.rua, &c.endereco.numero,
                   c.endereco.complemento, c.endereco.bairro, c.endereco.cidade, c.endereco.uf, c.endereco.cep);
            (*clientes)[*totalClientes] = c;
            (*totalClientes)++;
        }
        fclose(f);
    }

    // 2. Carregar Veiculos
    f = fopen("veiculos.csv", "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (*totalVeiculos == *capVeiculos) {
                *capVeiculos *= 2;
                *veiculos = (Veiculo *) realloc(*veiculos, *capVeiculos * sizeof(Veiculo));
            }
            Veiculo v;
            sscanf(linha, "%49[^;];%49[^;];%d;%19[^;];%f;%f;%d;%f;%d",
                   v.marca, v.modelo, &v.ano, v.placa, &v.valor, &v.valorCompra,
                   &v.km, &v.valorDiaria, &v.status);
            (*veiculos)[*totalVeiculos] = v;
            (*totalVeiculos)++;
        }
        fclose(f);
    }

    // 3. Carregar Vendas
    f = fopen("vendas.csv", "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (*totalVendas == *capVendas) {
                *capVendas *= 2;
                *vendas = (Venda *) realloc(*vendas, *capVendas * sizeof(Venda));
            }
            Venda vd;
            sscanf(linha, "%d;%19[^;];%19[^;];%19[^;];%f",
                   &vd.idVenda, vd.veiculo.placa, vd.cliente.cpf, vd.dataVenda, &vd.valorFinal);
            (*vendas)[*totalVendas] = vd;
            (*totalVendas)++;
        }
        fclose(f);
    }

    // 4. Carregar Locacoes
    f = fopen("locacoes.csv", "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (*totalLocacoes == *capLocacoes) {
                *capLocacoes *= 2;
                *locacoes = (Locacao *) realloc(*locacoes, *capLocacoes * sizeof(Locacao));
            }
            Locacao l;
            sscanf(linha, "%d;%19[^;];%19[^;];%19[^;];%d;%d;%d;%d;%f;%f;%f;%d;%d",
                   &l.idLocacao, l.placaVeiculo, l.cpfCliente, l.dataInicio, &l.diasPrevistos,
                   &l.diasUtilizados, &l.kmInicial, &l.kmFinal, &l.valorDiaria, &l.taxaAvariaMulta,
                   &l.valorTotal, &l.formaPagamento, &l.status);
            (*locacoes)[*totalLocacoes] = l;
            (*totalLocacoes)++;
        }
        fclose(f);
    }

    // 5. Carregar Manutencoes
    f = fopen("manutencoes.csv", "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (*totalManutencoes == *capManutencoes) {
                *capManutencoes *= 2;
                *manutencoes = (Manutencao *) realloc(*manutencoes, *capManutencoes * sizeof(Manutencao));
            }
            Manutencao m;
            sscanf(linha, "%d;%19[^;];%99[^;];%19[^;];%19[^;];%f;%d",
                   &m.idManutencao, m.placaVeiculo, m.descricao, m.dataEntrada,
                   m.dataSaida, &m.custo, &m.status);
            (*manutencoes)[*totalManutencoes] = m;
            (*totalManutencoes)++;
        }
        fclose(f);
    }
}