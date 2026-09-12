#ifndef VEICULOS_H
#define VEICULOS_H

#include "modelos.h"

// Calcula o valor de mercado estimado do veiculo aplicando depreciacao
// sobre o valor de compra, de acordo com a idade do veiculo (ano de
// fabricacao x ano atual). O valor nunca cai abaixo de 30% do valor pago.
float calcularValorDepreciado(const Veiculo *veiculo);

void cadastrarVeiculo(Veiculo *veiculo, const Veiculo veiculos[], int totalVeiculos);
void listarVeiculosDisponiveis(const Veiculo veiculos[], int total);
int buscarVeiculo(const Veiculo veiculos[], int total, const char placa[]);
void procurarVeiculosAvancado(const Veiculo veiculos[], int total);
void editarVeiculo(Veiculo *veiculo);
void apagarVeiculo(Veiculo **veiculos, int *total, int *capacidade, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes);
void menuVeiculos(Veiculo **veiculos, int *total, int *capacidade, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes);

#endif