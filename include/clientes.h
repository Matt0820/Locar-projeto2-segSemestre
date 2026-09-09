#ifndef CLIENTES_H
#define CLIENTES_H

#include "modelos.h"

void cadastrarCliente(Cliente *cliente, const Cliente clientes[], int totalClientes);
void listarClientes(const Cliente clientes[], int total);
void exibirDetalhesCliente(const Cliente *cliente);
int buscarCliente(const Cliente clientes[], int total, const char busca[]);
int clienteTemVendaAssociada(const Venda vendas[], int totalVendas, const char cpf[]);
int clienteTemLocacaoAtiva(const Locacao locacoes[], int totalLocacoes, const char cpf[]);
void editarCliente(Cliente *cliente);
void apagarCliente(Cliente **clientes, int *total, int *cap, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes);
void menuClientes(Cliente **clientes, int *total, int *capacidade, const Venda vendas[], int totalVendas, const Locacao locacoes[], int totalLocacoes);

#endif