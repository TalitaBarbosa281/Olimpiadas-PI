//Considere 10 países a sua escolha. Crie um ranking dos países com base no número total de medalhas conquistadas. Ordene os países pelo total de medalhas
/*
==========================================================
 PROJETO OLIMPÍADAS - ANÁLISE DE EFICIÊNCIA DE PAÍSES
----------------------------------------------------------
// Ranking por eficiencia
// A eficiencia é calculada como:
// total de medalhas / total de atletas unicos

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"


// Essa função percorre a string recebida e transforma cada caractere em maiúsculo. Isso evita erro quando o usuário digita siglas em minúsculo.
void deixarMaiusculo(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}


// Essa função verifica se o código NOC digitado existe dentro dos dados carregados. Ela percorre todos os registros comparando as siglas.
int validarNOC(char *noc, Resultado *dados, int totalDeDados) {
    for (int i = 0; i < totalDeDados; i++) {
        if (strcmp(dados[i].noc, noc) == 0) {
            return 1;  // encontrado
        }
    }
    return 0; // não encontrado
}


// Função usada pelo qsort para ordenar os países.
// A comparação é feita pela eficiência calculada.
// A ordenação é decrescente (mais eficiente primeiro).
int compararEficiencia(const void *a, const void *b) {

    PaisStats *p1 = (PaisStats*)a;
    PaisStats *p2 = (PaisStats*)b;

    double e1 = (p1->total_atletas == 0) ? 0 :
                (double)p1->total_medalhas / p1->total_atletas;

    double e2 = (p2->total_atletas == 0) ? 0 :
                (double)p2->total_medalhas / p2->total_atletas;

    if (e1 < e2) return 1;
    if (e1 > e2) return -1;
    return 0;
}


// Essa função verifica se um atleta já foi contado.
// Isso evita duplicar atletas quando aparecem em várias linhas do arquivo.
int atletaJaExiste(int *lista, int tamanho, int id) {
    for(int i=0;i<tamanho;i++){
        if(lista[i] == id)
            return 1;
    }
    return 0;
}



// Função principal responsável por montar o ranking
void ordenarEficiencia(Resultado *dados, int totalDeDados){

    printf("\nRanking por eficiencia (medalhas/atletas)\n");

    // Vetor que guarda as estatísticas dos países escolhidos
    PaisStats placar[10];

    // Matrizes auxiliares para guardar IDs únicos de atletas
    int atletasTemp[10][60000];
    int atletasCount[10] = {0};

    int count = 0;

    // Loop que permite o usuário escolher os 10 países
    while(count < 10){

        char entrada[100];

        printf("Digite o codigo do %d.o pais: ", count+1);
        scanf("%s", entrada);

        // Padroniza entrada para evitar erro de comparação
        deixarMaiusculo(entrada);

        // Verifica tamanho da sigla
        if(strlen(entrada) > 3){
            printf("Sigla deve ter 3 letras\n");
            continue;
        }

        // Verifica se existe nos dados
        if(validarNOC(entrada, dados, totalDeDados)){

            int repetido = 0;

            // Evita inserir país duplicado
            for(int j=0;j<count;j++){
                if(strcmp(placar[j].noc, entrada)==0){
                    repetido = 1;
                    break;
                }
            }

            if(repetido){
                printf("Pais repetido, escolha outro\n");
            }
            else{
                strcpy(placar[count].noc, entrada);
                placar[count].total_medalhas = 0;
                placar[count].total_atletas = 0;

                printf(" -> %s adicionado\n", entrada);
                count++;
            }
        }
        else{
            printf("Codigo nao encontrado\n");
        }
    }


    // Percorre todos os dados carregados
    // Aqui acontece a contagem de medalhas e atletas
    for(int i=0;i<totalDeDados;i++){

        for(int k=0;k<10;k++){

            // Verifica se o registro pertence a um dos países escolhidos
            if(strcmp(dados[i].noc, placar[k].noc)==0){

                // Conta atleta apenas uma vez
                if(!atletaJaExiste(atletasTemp[k],
                                   atletasCount[k],
                                   dados[i].athlete_id))
                {
                    atletasTemp[k][atletasCount[k]++] =
                        dados[i].athlete_id;

                    placar[k].total_atletas++;
                }

                // Conta medalha válida
                if(strcmp(dados[i].medal,"NA")!=0 &&
                   strcmp(dados[i].medal,"")!=0)
                {
                    placar[k].total_medalhas++;
                }

                break;
            }
        }
    }


    // Ordena os países com base na eficiência
    qsort(placar, 10, sizeof(PaisStats), compararEficiencia);


    // Impressão do ranking final
    printf("%-10s | %-10s | %-10s | %s\n",
           "PAIS","MEDALHAS","ATLETAS","EFICIENCIA");

    printf("-------------------------------------------------\n");

    for(int i=0;i<10;i++){

        double eficiencia =
            (placar[i].total_atletas==0)?0:
            (double)placar[i].total_medalhas /
            placar[i].total_atletas;

        printf("%-10s | %-10d | %-10d | %.4f\n",
               placar[i].noc,
               placar[i].total_medalhas,
               placar[i].total_atletas,
               eficiencia);
    }
}
