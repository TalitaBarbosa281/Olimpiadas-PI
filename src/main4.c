#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "leitura.h"

// Avisa que essa função existe no outro arquivo (questao4.c)
void ordernarPontuacao(Resultado *dados, int total_dados);

int main() {
    int total = 0;
    Resultado *dados = NULL;

    printf("=== SISTEMA OLIMPICO MODULAR ===\n");

    // 1. LEITURA
    dados = carregar_resultados("data/results.csv", &total);

    if (dados == NULL) {
        printf("Falha fatal no carregamento. Verifique o arquivo results.csv\n");
        return 1; // Encerra com erro
    }

    // PROCESSAMENTO
    ordernarPontuacao(dados, total);

    // 3. LIMPEZA
    free(dados);
    printf("\nSistema finalizado com sucesso.\n");
    return 0;
}