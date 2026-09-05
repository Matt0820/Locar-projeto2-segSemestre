#ifndef MODELOS_H
#define MODELOS_H

typedef struct {
    char rua[60];
    int numero;
    char complemento[30];
    char bairro[40];
    char cidade[40];
    char uf[10];
    char cep[20];
} Endereco;

typedef struct {
    char nome[80];
    char cpf[20];
    char dataNascimento[20];
    char estadoCivil[30];
    char telefone[20];
    char email[60];
    float rendaMensal;
    char profissao[50];
    Endereco endereco;
} Cliente;

typedef struct {
    char marca[50];
    char modelo[50];
    int ano;
    char placa[20];
    float valor;
    int km;
    float valorDiaria;
    int status; // 0 = Disponivel, 1 = Alugado, 2 = Vendido
} Veiculo;

typedef struct {
    Veiculo veiculo;
    Cliente cliente;
    char dataVenda[20];
} Venda;

typedef struct {
    int idLocacao;
    char placaVeiculo[20];
    char cpfCliente[20];
    char dataInicio[20];
    int diasPrevistos;
    int diasUtilizados;
    int kmInicial;
    int kmFinal;
    float valorDiaria;
    float taxaAvariaMulta;
    float valorTotal;
    int formaPagamento; // 1-PIX, 2-Cartao, 3-Dinheiro
    int status;         // 1 = Ativa, 0 = Finalizada
} Locacao;

#endif