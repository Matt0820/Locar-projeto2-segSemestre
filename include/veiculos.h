#ifndef VEICULOS_H
#define VEICULOS_H

#include "modelos.h"

void cadastrarVeiculo(Veiculo *veiculo);

void listarVeiculos(const Veiculo veiculos[], int total, int filtro);

int buscarVeiculo(const Veiculo veiculos[], int total, const char placa[]);

void editarVeiculo(Veiculo *veiculo);

void apagarVeiculo(Veiculo **veiculos, int *total, int *capacidade);

void menuVeiculos(Veiculo **veiculos, int *total, int *capacidade);

#endif