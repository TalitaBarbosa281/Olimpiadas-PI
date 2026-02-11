#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/structs.h" 
#include "include/leitura.h"


// isso avisa ao compilador a presença dessas funções. Cada função atua em sua respectiva questão. Isso é necessário porque o código dessas funções está em outros arquivos. Como o C lê tudo de cima pra baixo, quando chegarmos no main, o compilador precisa saber que essas questões existiam
void ordenarRanking(Resultado *dados, int totalDeDados);
void participacaoPorGenero();
void resolver_questao_3(char* pais_alvo);
void ordenarPontuacao(Resultado *dados, int totalDeDados);

/*
Quando usamos scanf, ele lê o número mas deixa o "Enter" (\n) sobrando na memória.  Se não limparmos, o próximo comando de leitura vai achar esse "Enter" velho e pular a vez. 
Essa função consome tudo o que sobrou para deixar o caminho limpo.
*/
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void pausar() {
    printf("\n[Pressione ENTER para voltar]");
    limpar_buffer(); getchar();
}
int main() {
    int qtd_res = 0;
    //Carregando o banco de dados que ficou sem ser
    Resultado *dados_res = NULL;

    printf("Carregando banco de dados...\n");
    dados_res = carregar_resultados("results.csv", &qtd_res);

    if (!dados_res) {
        printf("[ERRO] results.csv nao encontrado.\n");
        return 1;
    }

    int opcao = 0;
    char pais_escolhido[100]; // tem q declarar antes, tava dando erro quando eu colocava dentro do case 3

    while (1) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n=== MENU OLIMPICO ===\n");
        printf("1. Ranking olimpico por numero de medalhas\n");
        printf("2. Participacao por genero nas Olimpiadas\n");
        printf("3. Medalhistas que ja conquistaram mais de uma medalha\n");
        printf("4. Ranking olimpico por media ponderada de medalhas\n");
        printf("0. Sair\n");
        printf("Escolha: ");

        scanf("%d", &opcao);
        if (opcao == 0) break;

        switch (opcao) {
            case 1:
                ordenarRanking(dados_res, qtd_res);
                pausar();
                break;
            case 2:
                participacaoPorGenero();
                pausar();
                break;
            case 3:

                printf("=== SISTEMA DE ANALISE DE MEDALHAS (QUESTAO 3) ===\n");
                printf("Digite a sigla do pais (ex: BRA, USA, FRA): ");
                
                // Le o que o usuario digitou
                scanf("%s", pais_escolhido);

                // Converte para maiusculo a entrada
                for(int i = 0; pais_escolhido[i]; i++){
                    pais_escolhido[i] = toupper(pais_escolhido[i]);
                }

                // Chama a função da lógica que está no questao3.c
                resolver_questao_3(pais_escolhido);

                pausar();
                break;
                
            case 4:
                ordenarPontuacao(dados_res, qtd_res);
                pausar();
                break;
            default:
                printf("Opcao invalida.\n");
                pausar();
        }
    }
    free(dados_res);
    return 0;
}

