//Considere 10 países. Crie um ranking ordenado pela pontuação de medalhas, em que Ouro vale 3 pontos, Prata vale 2 pontos e Bronze vale 1 ponto 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"

// Como o C não possue funções como toUpper ou toLower, é preciso criar uma função para ele não criar diferenças por causa de teclas maiúsculas ou minúsculas
void deixarMaiusculo(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

// É basicamente um booleano que vai percorrer o arquivo e observar se o NOC é válido ou não (por exemplo, se eu digitar YTS, claramente não é um noc presente em noc.csv, mas o código só vai saber percorrendo tudo e descobrindo)
int validarNOC(char *noc, Resultado *dados, int totalDeDados) {
    for (int i = 0; i < totalDeDados; i++) {
        if (strcmp(dados[i].noc, noc) == 0) {
            return 1;
        }
    }
    return 0;
}

// Função de comparação para o qsort (ordena por pontos, do maior para o menor)
int compararPontos(const void *a, const void *b) {
    //Aqui é feito um trabalho de conversão de ponteiros genéricos para ponteiros do tipo 'PaisStats'. É um casting, mas não de tipos primitivos
    PaisStats *ponteiro1 = (PaisStats *)a;
    PaisStats *ponteiro2 = (PaisStats *)b;
    PaisStats pais1 = *ponteiro1;
    PaisStats pais2 = *ponteiro2;
    // Subtração invertida (B - A) gera ordem decrescente
    return pais2.pontuacao_total - pais1.pontuacao_total;
}

// Essa é a função que organiza a lógica da questão de fato, que é ordenar avaliando a pontuação de medalhas
void ordernarPontuacao(Resultado *dados, int totalDeDados){
    printf("Ranking por Pontuacao Ouro, Prata e Bronze\n");

    //Aqui estou definindo a quantidade de países que podem ser usados, junto a um contador para realizar a atividade de loop while, onde ocorre a seleção de países. Como pode ver, acontecem algumas verificações no meio do caminho para evitar erros de código
    PaisStats placar[10];
    int count = 0;
        while (count < 10) {
        char entrada[100];
        
        printf("Digite o codigo do %d.o pais: ", count + 1);
        scanf("%s", entrada);

        deixarMaiusculo(entrada);
        //Precisamos de uma verificação para, caso o usuário digite mais de 3 letras, o continue agirá para voltar o código loop para o início, mandando repetir
        if (strlen(entrada) > 3) {
            printf("Siglas olimpicas devem ter 3 letras\n");
            continue;
        }
        //aqui chamamos a função validarNOC. Caso o noc seja válido, ele continua para testar se esse noc já foi ou não digitado anteriormente. Caso não tenha sido digitado, o "objeto" país é criado, até completar 10
        if (validarNOC(entrada, dados, totalDeDados)) {
            int repetido = 0;
            for(int j=0; j < count; j++) {
                if (strcmp(placar[j].noc, entrada) == 0) {
                    repetido = 1; 
                    break;
                }
            }

            if (repetido) {
                printf("Esse pais ja foi adicionado na lista. Escolha outro\n");
            } else {
                strcpy(placar[count].noc, entrada);
                placar[count].qtd_ouros = 0;
                placar[count].qtd_pratas = 0;
                placar[count].qtd_bronzes = 0;
                placar[count].pontuacao_total = 0;
                placar[count].total_medalhas = 0;
                
                printf("    -> %s adicionado com sucesso.\n", entrada);
                count++;
            }
        } else {
            printf("[!] O codigo '%s' nao foi encontrado nos registros. Tente outro.\n", entrada);
        }
    }

    // Loop de Varredura, percorrendo todas as linhas que foram carregadas do arquivo
    for (int i = 0; i < totalDeDados; i++) {
        
        // Tem medalha? Se a medalha for "NA" ou vazia, ou seja, se não tem, pula para o próximo
        if (strcmp(dados[i].medal, "NA") == 0 || strcmp(dados[i].medal, "") == 0) {
            continue; 
        }

        // Aqui fazemos a pergunta "É um dos nossos 10 países?""
        for (int k = 0; k < 10; k++) {
            // Se o NOC do resultado for igual ao NOC do nosso placar, imprementa-se o contador específico (seja ouro, prata ou bronze)
            if (strcmp(dados[i].noc, placar[k].noc) == 0) {
                if (strcmp(dados[i].medal, "Gold") == 0) {
                    placar[k].qtd_ouros++;
                } 
                else if (strcmp(dados[i].medal, "Silver") == 0) {
                    placar[k].qtd_pratas++;
                } 
                else if (strcmp(dados[i].medal, "Bronze") == 0) {
                    placar[k].qtd_bronzes++;
                }
                // Como já achamos o país, não precisa testar os outros 9
                break; 
            }
        }
    }
    // Loop matemático. Agora que contamos tudo, aplicamos os pesos
    for (int k = 0; k < 10; k++) {
        placar[k].pontuacao_total = (placar[k].qtd_ouros * 3) + 
                                    (placar[k].qtd_pratas * 2) + 
                                    (placar[k].qtd_bronzes * 1);        
        // Aproveitamos para guardar o total de medalhas simples também
        placar[k].total_medalhas = placar[k].qtd_ouros + placar[k].qtd_pratas + placar[k].qtd_bronzes;
    }

    // Ordenamos os países com um qsort, que utiliza a função compararPontos criada no início do código. Passamos o vetor, o tamanho 10, o tamanho de cada elemento e a função compararPontos
    qsort(placar, 10, sizeof(PaisStats), compararPontos);

    //Exibição formal
    printf("%-10s | %-6s | %-6s | %-6s | %s\n", "PAIS", "OURO", "PRATA", "BRONZE", "PONTOS");
    printf("---------------------------------------------------\n");
    //Laço que percorre para exibir cada país e seus resultados
    for (int i = 0; i < 10; i++) {
        printf("%-10s | %-6d | %-6d | %-6d | %d\n", 
               placar[i].noc, 
               placar[i].qtd_ouros, 
               placar[i].qtd_pratas, 
               placar[i].qtd_bronzes, 
               placar[i].pontuacao_total);
    }
}
