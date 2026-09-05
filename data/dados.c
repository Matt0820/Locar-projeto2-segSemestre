#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Inclusões baseadas na estrutura do projeto */
#include "../include/clientes.h"
#include "../include/veiculos.h"
#include "../include/locacoes.h"
#include "../include/vendas.h"
#include "../include/modelos.h"

#define CAMINHO_ARQUIVO "data/dados.csv"

**Gerenciamento de Arquivos**

*   **`inicializar_banco_dados()`**: Garante que o arquivo CSV exista no diretório correto antes das operações.
*   **Rotinas de Escrita**: Abrem o arquivo em modo append (`"a"`) para inserir novos registros sem sobrescrever os dados anteriores.

```c
void inicializar_banco_dados() {
    FILE *arquivo = fopen(CAMINHO_ARQUIVO, "a");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível criar ou acessar %s\n", CAMINHO_ARQUIVO);
        return;
    }
    fclose(arquivo);
}

int salvar_cliente_csv(Cliente cliente) {
    FILE *arquivo = fopen(CAMINHO_ARQUIVO, "a");
    if (arquivo == NULL) return 0;

    /* Formato: TIPO;ID;NOME;CPF */
    fprintf(arquivo, "CLIENTE;%d;%s;%s\n", cliente.id, cliente.nome, cliente.cpf);
    
    fclose(arquivo);
    return 1;
}

int salvar_veiculo_csv(Veiculo veiculo) {
    FILE *arquivo = fopen(CAMINHO_ARQUIVO, "a");
    if (arquivo == NULL) return 0;

    /* Formato: TIPO;ID;PLACA;ID_MODELO */
    fprintf(arquivo, "VEICULO;%d;%s;%d\n", veiculo.id, veiculo.placa, veiculo.id_modelo);
    
    fclose(arquivo);
    return 1;
}

int salvar_locacao_csv(Locacao locacao) {
    FILE *arquivo = fopen(CAMINHO_ARQUIVO, "a");
    if (arquivo == NULL) return 0;

    /* Formato: TIPO;ID;ID_CLIENTE;ID_VEICULO;DIAS;VALOR */
    fprintf(arquivo, "LOCACAO;%d;%d;%d;%d;%.2f\n", 
            locacao.id, locacao.id_cliente, locacao.id_veiculo, locacao.dias, locacao.valor_total);
    
    fclose(arquivo);
    return 1;
}