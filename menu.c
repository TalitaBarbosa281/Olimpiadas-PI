#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "leitura.h"


// isso avisa ao compilador a presença dessa função
void ordernarPontuacao(Resultado *dados, int totalDeDados);

// aqui vai conter as outras questões

/*
Quando usamos scanf, ele lê o número mas deixa o "Enter" (\n) sobrando na memória. Se não limparmos, o próximo comando de leitura vai achar esse "Enter" velho e pular a vez. Essa função consome tudo o que sobrou para deixar o caminho limpo.
*/
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int qtd_res = 0;
    Resultado *dados_res = NULL;

    printf("Carregando banco de dados...\n");
    dados_res = carregar_resultados("data/results.csv", &qtd_res);

    if (!dados_res) {
        printf("[ERRO] results.csv nao encontrado.\n");
        return 1;
    }

    int opcao = 0;
    while (1) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n=== MENU OLIMPICO ===\n");
        printf("1. Medalhistas que já conquistaram mais de uma medalha\n");
        printf("2. Quantos atletas participaram de Olimpiada\n");
        printf("3. Ranking olímpico por numero de medalhas\n");
        printf("4. Ranking olimpico por media ponderada de medalhas\n");
        printf("0. Sair\n");
        printf("Escolha: ");

    free(dados_res);
    return 0;
    }
}