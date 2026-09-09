#ifndef DADOS_H
#define DADOS_H

#include "modelos.h"

void salvarDadosCSV(const Cliente clientes[], int totalClientes,
                    const Veiculo veiculos[], int totalVeiculos,
                    const Venda vendas[], int totalVendas,
                    const Locacao locacoes[], int totalLocacoes,
                    const Manutencao manutencoes[], int totalManutencoes);

void carregarDadosCSV(Cliente **clientes, int *totalClientes, int *capClientes,
                      Veiculo **veiculos, int *totalVeiculos, int *capVeiculos,
                      Venda **vendas, int *totalVendas, int *capVendas,
                      Locacao **locacoes, int *totalLocacoes, int *capLocacoes,
                      Manutencao **manutencoes, int *totalManutencoes, int *capManutencoes);

#endif