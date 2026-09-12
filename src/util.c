#include "util.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#endif

void limparBuffer(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void lerLinha(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) == NULL) {
        // Entrada encerrada/indisponivel (EOF): evita usar memoria nao inicializada e trava a leitura como uma linha vazia.
        destino[0] = '\0';
        return;
    }
    destino[strcspn(destino, "\n")] = '\0';

    // O ';' e o delimitador usado nos arquivos CSV.
    for (int i = 0; destino[i] != '\0'; i++) {
        if (destino[i] == ';') destino[i] = ' ';
    }
}

int anoAtualDoSistema(void) {
    time_t agora = time(NULL);
    struct tm *info = localtime(&agora);
    return info->tm_year + 1900;
}

void criarPastaDados(void) {
    // Se a pasta ja existir o retorno de erro e simplesmente ignorado.
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
#endif
}

int somenteLetras(const char *texto) {
    int temLetra = 0;

    if (texto == NULL || strlen(texto) == 0) return 0;

    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char ch = (unsigned char) texto[i];

        // Aceita letras a-z/A-Z, espaco, e letras acentuadas
        if (isalpha(ch) || ch >= 0x80) {
            temLetra = 1;
        } else if (ch == ' ') {
            // espaco e permitido, mas nao conta como letra
        } else {
            return 0;
        }
    }

    return temLetra;
}

int somenteDigitos(const char *texto) {
    if (texto == NULL || strlen(texto) == 0) return 0;

    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isdigit((unsigned char) texto[i])) return 0;
    }

    return 1;
}

int cpfValido(const char *cpf) {
    if (strlen(cpf) != 11) return 0;
    if (!somenteDigitos(cpf)) return 0;

    // Rejeita sequencias repetidas (000..., 111..., etc.)
    int todosIguais = 1;
    for (int i = 1; i < 11; i++) {
        if (cpf[i] != cpf[0]) { todosIguais = 0; break; }
    }
    if (todosIguais) return 0;

    int soma = 0;
    for (int i = 0; i < 9; i++) soma += (cpf[i] - '0') * (10 - i);
    int resto = (soma * 10) % 11;
    if (resto == 10) resto = 0;
    if (resto != (cpf[9] - '0')) return 0;

    soma = 0;
    for (int i = 0; i < 10; i++) soma += (cpf[i] - '0') * (11 - i);
    resto = (soma * 10) % 11;
    if (resto == 10) resto = 0;
    if (resto != (cpf[10] - '0')) return 0;

    return 1;
}

// A CNH (Carteira Nacional de Habilitacao) e feita apenas uma checagem estrutural: 11 digitos numericos, sem sequencias invalidas (todos iguais).
int cnhValida(const char *cnh) {
    if (strlen(cnh) != 11) return 0;
    if (!somenteDigitos(cnh)) return 0;

    int todosIguais = 1;
    for (int i = 1; i < 11; i++) {
        if (cnh[i] != cnh[0]) { todosIguais = 0; break; }
    }
    if (todosIguais) return 0;

    return 1;
}

int dataValida(const char *data) {
    int dia, mes, ano;
    char sobra;

    // sscanf precisa consumir exatamente dia/mes/ano
    if (sscanf(data, "%d/%d/%d%c", &dia, &mes, &ano, &sobra) != 3) return 0;

    if (ano < 1900 || ano > 2100) return 0;
    if (mes < 1 || mes > 12) return 0;

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int limiteDia = diasNoMes[mes - 1];
    int anoBissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
    if (mes == 2 && anoBissexto) limiteDia = 29;

    if (dia < 1 || dia > limiteDia) return 0;

    return 1;
}

int anoDaData(const char *data) {
    int dia, mes, ano;
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    return ano;
}

int emailValido(const char *email) {
    if (email == NULL) return 0;

    for (int i = 0; email[i] != '\0'; i++) {
        if (isspace((unsigned char) email[i])) return 0;
    }

    const char *arroba = strchr(email, '@');
    if (arroba == NULL || arroba == email) return 0;

    const char *ponto = strchr(arroba, '.');
    if (ponto == NULL || ponto[1] == '\0') return 0;

    return 1;
}
