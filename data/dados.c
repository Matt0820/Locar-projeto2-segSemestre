#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/modelos.h"
#include "../include/dados.h"
#include "../include/util.h"

// Todos os arquivos CSV do sistema sao gravados e lidos dentro da pasta "data".
#define CAMINHO_CLIENTES     "data/clientes.csv"
#define CAMINHO_VEICULOS     "data/veiculos.csv"
#define CAMINHO_VENDAS       "data/vendas.csv"
#define CAMINHO_LOCACOES     "data/locacoes.csv"
#define CAMINHO_MANUTENCOES  "data/manutencoes.csv"

void salvarDadosCSV(const Cliente clientes[], int totalClientes,
                    const Veiculo veiculos[], int totalVeiculos,
                    const Venda vendas[], int totalVendas,
                    const Locacao locacoes[], int totalLocacoes,
                    const Manutencao manutencoes[], int totalManutencoes) {

    FILE *f;

    criarPastaDados(); // garante que a pasta "data" exista antes de gravar

    // 1. Salvar Clientes
    f = fopen(CAMINHO_CLIENTES, "w");
    if (f) {
        for (int i = 0; i < totalClientes; i++) {
            fprintf(f, "%s;%s;%s;%d;%s;%s;%s;%f;%s;%s;%d;%s;%s;%s;%s;%s;%s\n",
                    clientes[i].nome, clientes[i].cpf, clientes[i].dataNascimento,
                    clientes[i].anoNascimento, clientes[i].estadoCivil, clientes[i].telefone,
                    clientes[i].email, clientes[i].rendaMensal, clientes[i].profissao,
                    clientes[i].endereco.rua, clientes[i].endereco.numero, clientes[i].endereco.complemento,
                    clientes[i].endereco.bairro, clientes[i].endereco.cidade, clientes[i].endereco.uf,
                    clientes[i].endereco.cep, clientes[i].cnh);
        }
        fclose(f);
    }

    // 2. Salvar Veiculos
    f = fopen(CAMINHO_VEICULOS, "w");
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
    f = fopen(CAMINHO_VENDAS, "w");
    if (f) {
        for (int i = 0; i < totalVendas; i++) {
            fprintf(f, "%d;%s;%s;%s;%f;%d;%d;%f\n",
                    vendas[i].idVenda, vendas[i].veiculo.placa,
                    vendas[i].cliente.cpf, vendas[i].dataVenda, vendas[i].valorFinal,
                    vendas[i].formaPagamento, vendas[i].numParcelas, vendas[i].valorParcela);
        }
        fclose(f);
    }

    // 4. Salvar Locacoes
    f = fopen(CAMINHO_LOCACOES, "w");
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
    f = fopen(CAMINHO_MANUTENCOES, "w");
    if (f) {
        for (int i = 0; i < totalManutencoes; i++) {
            fprintf(f, "%d;%s;%s;%s;%s;%f;%d\n",
                    manutencoes[i].idManutencao, manutencoes[i].placaVeiculo,
                    manutencoes[i].descricao, manutencoes[i].dataEntrada,
                    manutencoes[i].dataSaida, manutencoes[i].custo, manutencoes[i].status);
        }
        fclose(f);
    }

    printf("\n[DADOS GRAVADOS COM SUCESSO NA PASTA data/!]\n");
}

// Divide "linha" em ponteiros de campos separados por ';'.
static int dividirCampos(char *linha, char *campos[], int maxCampos) {
    int qtd = 0;
    linha[strcspn(linha, "\r\n")] = '\0';

    char *inicio = linha;
    while (qtd < maxCampos) {
        campos[qtd++] = inicio;
        char *sep = strchr(inicio, ';');
        if (sep == NULL) break;
        *sep = '\0';
        inicio = sep + 1;
    }
    return qtd;
}

static void campoTexto(char *destino, size_t tamanho, int indice, char *campos[], int totalCampos) {
    if (indice < totalCampos) {
        snprintf(destino, tamanho, "%s", campos[indice]);
    } else {
        destino[0] = '\0';
    }
}

static int campoInt(int indice, char *campos[], int totalCampos, int padrao) {
    return (indice < totalCampos) ? atoi(campos[indice]) : padrao;
}

static float campoFloat(int indice, char *campos[], int totalCampos, float padrao) {
    return (indice < totalCampos) ? (float) atof(campos[indice]) : padrao;
}

void carregarDadosCSV(Cliente **clientes, int *totalClientes, int *capClientes,
                      Veiculo **veiculos, int *totalVeiculos, int *capVeiculos,
                      Venda **vendas, int *totalVendas, int *capVendas,
                      Locacao **locacoes, int *totalLocacoes, int *capLocacoes,
                      Manutencao **manutencoes, int *totalManutencoes, int *capManutencoes) {

    FILE *f;
    char linha[512];
    char *campos[20];
    int n;

    // 1. Carregar Clientes
    // Ordem: nome;cpf;dataNascimento;anoNascimento;estadoCivil;telefone;email;rendaMensal;profissao;rua;numero;complemento;bairro;cidade;uf;cep;cnh
    f = fopen(CAMINHO_CLIENTES, "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (linha[0] == '\0' || linha[0] == '\n' || linha[0] == '\r') continue;
            if (*totalClientes == *capClientes) {
                *capClientes *= 2;
                *clientes = (Cliente *) realloc(*clientes, *capClientes * sizeof(Cliente));
            }
            n = dividirCampos(linha, campos, 20);

            Cliente c;
            memset(&c, 0, sizeof(Cliente));
            campoTexto(c.nome, sizeof(c.nome), 0, campos, n);
            campoTexto(c.cpf, sizeof(c.cpf), 1, campos, n);
            campoTexto(c.dataNascimento, sizeof(c.dataNascimento), 2, campos, n);
            c.anoNascimento = campoInt(3, campos, n, 0);
            campoTexto(c.estadoCivil, sizeof(c.estadoCivil), 4, campos, n);
            campoTexto(c.telefone, sizeof(c.telefone), 5, campos, n);
            campoTexto(c.email, sizeof(c.email), 6, campos, n);
            c.rendaMensal = campoFloat(7, campos, n, 0.0f);
            campoTexto(c.profissao, sizeof(c.profissao), 8, campos, n);
            campoTexto(c.endereco.rua, sizeof(c.endereco.rua), 9, campos, n);
            c.endereco.numero = campoInt(10, campos, n, 0);
            campoTexto(c.endereco.complemento, sizeof(c.endereco.complemento), 11, campos, n);
            campoTexto(c.endereco.bairro, sizeof(c.endereco.bairro), 12, campos, n);
            campoTexto(c.endereco.cidade, sizeof(c.endereco.cidade), 13, campos, n);
            campoTexto(c.endereco.uf, sizeof(c.endereco.uf), 14, campos, n);
            campoTexto(c.endereco.cep, sizeof(c.endereco.cep), 15, campos, n);
            campoTexto(c.cnh, sizeof(c.cnh), 16, campos, n);

            (*clientes)[*totalClientes] = c;
            (*totalClientes)++;
        }
        fclose(f);
    }

    // 2. Carregar Veiculos
    // Ordem: marca;modelo;ano;placa;valor;valorCompra;km;valorDiaria;status
    f = fopen(CAMINHO_VEICULOS, "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (linha[0] == '\0' || linha[0] == '\n' || linha[0] == '\r') continue;
            if (*totalVeiculos == *capVeiculos) {
                *capVeiculos *= 2;
                *veiculos = (Veiculo *) realloc(*veiculos, *capVeiculos * sizeof(Veiculo));
            }
            n = dividirCampos(linha, campos, 20);

            Veiculo v;
            memset(&v, 0, sizeof(Veiculo));
            campoTexto(v.marca, sizeof(v.marca), 0, campos, n);
            campoTexto(v.modelo, sizeof(v.modelo), 1, campos, n);
            v.ano = campoInt(2, campos, n, 0);
            campoTexto(v.placa, sizeof(v.placa), 3, campos, n);
            v.valor = campoFloat(4, campos, n, 0.0f);
            v.valorCompra = campoFloat(5, campos, n, 0.0f);
            v.km = campoInt(6, campos, n, 0);
            v.valorDiaria = campoFloat(7, campos, n, 0.0f);
            v.status = campoInt(8, campos, n, 0);

            (*veiculos)[*totalVeiculos] = v;
            (*totalVeiculos)++;
        }
        fclose(f);
    }

    // 3. Carregar Vendas
    // Ordem: idVenda;placaVeiculo;cpfCliente;dataVenda;valorFinal;formaPagamento;numParcelas;valorParcela.
    f = fopen(CAMINHO_VENDAS, "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (linha[0] == '\0' || linha[0] == '\n' || linha[0] == '\r') continue;
            if (*totalVendas == *capVendas) {
                *capVendas *= 2;
                *vendas = (Venda *) realloc(*vendas, *capVendas * sizeof(Venda));
            }
            n = dividirCampos(linha, campos, 20);

            Venda vd;
            memset(&vd, 0, sizeof(Venda));
            vd.idVenda = campoInt(0, campos, n, 0);
            campoTexto(vd.veiculo.placa, sizeof(vd.veiculo.placa), 1, campos, n);
            campoTexto(vd.cliente.cpf, sizeof(vd.cliente.cpf), 2, campos, n);
            campoTexto(vd.dataVenda, sizeof(vd.dataVenda), 3, campos, n);
            vd.valorFinal = campoFloat(4, campos, n, 0.0f);
            vd.formaPagamento = campoInt(5, campos, n, 1);
            vd.numParcelas = campoInt(6, campos, n, 0);
            vd.valorParcela = campoFloat(7, campos, n, 0.0f);

            for (int j = 0; j < *totalClientes; j++) {
                if (strcmp((*clientes)[j].cpf, vd.cliente.cpf) == 0) {
                    vd.cliente = (*clientes)[j];
                    break;
                }
            }
            for (int j = 0; j < *totalVeiculos; j++) {
                if (strcmp((*veiculos)[j].placa, vd.veiculo.placa) == 0) {
                    vd.veiculo = (*veiculos)[j];
                    break;
                }
            }
            vd.veiculo.valor = vd.valorFinal;

            (*vendas)[*totalVendas] = vd;
            (*totalVendas)++;
        }
        fclose(f);
    }

    // 4. Carregar Locacoes
    // Ordem: idLocacao;placaVeiculo;cpfCliente;dataInicio;diasPrevistos;diasUtilizados;kmInicial;kmFinal;valorDiaria;taxaAvariaMulta;valorTotal;formaPagamento;status
    f = fopen(CAMINHO_LOCACOES, "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (linha[0] == '\0' || linha[0] == '\n' || linha[0] == '\r') continue;
            if (*totalLocacoes == *capLocacoes) {
                *capLocacoes *= 2;
                *locacoes = (Locacao *) realloc(*locacoes, *capLocacoes * sizeof(Locacao));
            }
            n = dividirCampos(linha, campos, 20);

            Locacao l;
            memset(&l, 0, sizeof(Locacao));
            l.idLocacao = campoInt(0, campos, n, 0);
            campoTexto(l.placaVeiculo, sizeof(l.placaVeiculo), 1, campos, n);
            campoTexto(l.cpfCliente, sizeof(l.cpfCliente), 2, campos, n);
            campoTexto(l.dataInicio, sizeof(l.dataInicio), 3, campos, n);
            l.diasPrevistos = campoInt(4, campos, n, 0);
            l.diasUtilizados = campoInt(5, campos, n, 0);
            l.kmInicial = campoInt(6, campos, n, 0);
            l.kmFinal = campoInt(7, campos, n, 0);
            l.valorDiaria = campoFloat(8, campos, n, 0.0f);
            l.taxaAvariaMulta = campoFloat(9, campos, n, 0.0f);
            l.valorTotal = campoFloat(10, campos, n, 0.0f);
            l.formaPagamento = campoInt(11, campos, n, 0);
            l.status = campoInt(12, campos, n, 0);

            (*locacoes)[*totalLocacoes] = l;
            (*totalLocacoes)++;
        }
        fclose(f);
    }

    // 5. Carregar Manutencoes
    // Ordem: idManutencao;placaVeiculo;descricao;dataEntrada;dataSaida;custo;status
    f = fopen(CAMINHO_MANUTENCOES, "r");
    if (f) {
        while (fgets(linha, sizeof(linha), f)) {
            if (linha[0] == '\0' || linha[0] == '\n' || linha[0] == '\r') continue;
            if (*totalManutencoes == *capManutencoes) {
                *capManutencoes *= 2;
                *manutencoes = (Manutencao *) realloc(*manutencoes, *capManutencoes * sizeof(Manutencao));
            }
            n = dividirCampos(linha, campos, 20);

            Manutencao m;
            memset(&m, 0, sizeof(Manutencao));
            m.idManutencao = campoInt(0, campos, n, 0);
            campoTexto(m.placaVeiculo, sizeof(m.placaVeiculo), 1, campos, n);
            campoTexto(m.descricao, sizeof(m.descricao), 2, campos, n);
            campoTexto(m.dataEntrada, sizeof(m.dataEntrada), 3, campos, n);
            campoTexto(m.dataSaida, sizeof(m.dataSaida), 4, campos, n);
            m.custo = campoFloat(5, campos, n, 0.0f);
            m.status = campoInt(6, campos, n, 0);

            (*manutencoes)[*totalManutencoes] = m;
            (*totalManutencoes)++;
        }
        fclose(f);
    }
}
