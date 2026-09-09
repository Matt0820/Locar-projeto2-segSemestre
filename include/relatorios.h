#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "modelos.h"

void relatorioVendasLocacoesPorPeriodo(const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes);
void resumoFinanceiroGeral(const Veiculo veiculos[], int totalVeiculos, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes, const Manutencao manutencoes[], int totalManutencoes);
void relatorioVeiculosVendidos(const Venda vendas[], int totalVendas);
void relatorioDespesasManutencao(const Manutencao manutencoes[], int totalManutencoes);
void menuRelatorios(const Veiculo veiculos[], int totalVeiculos, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes, const Manutencao manutencoes[], int totalManutencoes);

#endif