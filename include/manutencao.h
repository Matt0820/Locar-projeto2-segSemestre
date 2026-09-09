#ifndef MANUTENCAO_H
#define MANUTENCAO_H

#include "modelos.h"

void registrarManutencao(Veiculo **veiculos, int totalVeiculos, Manutencao **manutencoes, int *totalManutencoes, int *capManutencoes);
void finalizarManutencao(Veiculo **veiculos, int totalVeiculos, Manutencao **manutencoes, int totalManutencoes);
void listarManutencoesAndamento(const Manutencao manutencoes[], int totalManutencoes);
void historicoManutencoes(const Manutencao manutencoes[], int totalManutencoes);
void menuManutencao(Veiculo **veiculos, int totalVeiculos, Manutencao **manutencoes, int *totalManutencoes, int *capManutencoes);

#endif