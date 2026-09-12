#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

// Limpa o restante do buffer de entrada (stdin) apos uma leitura com scanf
void limparBuffer(void);

// Le uma linha via fgets, remove a quebra de linha e neutraliza ';'
void lerLinha(char *destino, int tamanho);

// Retorna o ano atual do sistema (usado para validar idade/ano do veiculo)
int anoAtualDoSistema(void);

// guardar os arquivos CSV e o relatorio exportado em TXT
void criarPastaDados(void);

// Restricoes de cadastro
int somenteLetras(const char *texto);      // letras (com acentuacao) e espacos, nao vazio
int somenteDigitos(const char *texto);     // apenas digitos numericos, nao vazio
int cpfValido(const char *cpf);            // 11 digitos + digitos verificadores validos
int cnhValida(const char *cnh);            // 11 digitos numericos (checagem estrutural)
int dataValida(const char *data);          // formato DD/MM/AAAA, com dia/mes/ano coerentes
int anoDaData(const char *data);           // extrai o ano (AAAA) de uma data ja validada
int emailValido(const char *email);        // formato basico usuario@dominio.extensao

#endif
