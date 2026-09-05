#ifndef VENDAS_H
#define VENDAS_H

#include "modelos.h"

void realizarVenda(Veiculo **veiculos, int *totalVeiculos, Cliente **clientes, int *totalClientes, int *capClientes, Venda **vendas, int *totalVendas, int *capVendas);

void listarVendas(const Venda vendas[], int totalVendas);

void menuVendas(Veiculo **veiculos, int *totalVeiculos, Cliente **clientes, int *totalClientes, int *capClientes, Venda **vendas, int *totalVendas, int *capVendas);

#endif