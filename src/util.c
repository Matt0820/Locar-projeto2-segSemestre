#include "util.h"

#include <stdio.h>
#include <string.h>

void limparBuffer(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}