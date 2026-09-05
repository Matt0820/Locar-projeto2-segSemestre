#ifndef LOCACOES_H
#define LOCACOES_H

#include "modelos.h"

int buscarLocacaoAtiva(const Locacao locacoes[], int total, const char placa[]);

void iniciarLocacao(Veiculo **veiculos, int totalVeiculos, Cliente **clientes, int totalClientes, Locacao **locacoes, int *totalLocacoes, int *capLocacoes);

void estenderLocacao(Locacao **locacoes, int totalLocacoes);

void finalizarLocacao(Veiculo **veiculos, int totalVeiculos, Locacao **locacoes, int totalLocacoes);

void listarLocacoesAtivas(const Locacao locacoes[], int totalLocacoes);

void historicoLocacoes(const Locacao locacoes[], int totalLocacoes);

void menuLocacoes(Veiculo **veiculos, int totalVeiculos, Cliente **clientes, int totalClientes, Locacao **locacoes, int *totalLocacoes, int *capLocacoes);

#endif