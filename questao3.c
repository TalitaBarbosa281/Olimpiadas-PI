//Para um determinado país, determine o número de atletas do sexo masculino e feminino que conquistaram mais de uma medalha de ouro

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h" 
#include "leitura.h"

// Tamanho seguro para o mapa (maior ID observado nos dados é ~150.000)
#define MAX_MAPA_ID 200000

void resolver_questao_3(char* pais_alvo) {
    printf("\n=== INICIANDO QUESTAO 3 PARA: %s ===\n", pais_alvo);

    int qtd_resultados = 0;
    int qtd_atletas = 0;

    // 1. CARREGAR RESULTADOS
    // IMPORTANTE: O nome do arquivo aqui deve ser igual ao que está na sua pasta
    Resultado* vetor_resultados = carregar_resultados("results.csv", &qtd_resultados);
    
    if (vetor_resultados == NULL) {
        printf("Falha ao ler resultados\n");
        return;
    }

    // 2. CARREGAR ATLETAS (BIOS)
    Atleta* vetor_atletas = carregar_atletas("bios.csv", &qtd_atletas);

    if (vetor_atletas == NULL) {
        printf("Falha ao ler bios\n");
        free(vetor_resultados);
        return;
    }

    // 3. CRIAR O MAPA DE CONTAGEM
    // Calloc inicia tudo com 0. O índice do vetor é o ID do atleta.
    int* mapa_ouros = (int*) calloc(MAX_MAPA_ID, sizeof(int));

    if (mapa_ouros == NULL) {
        printf("Memoria insuficiente para o mapa\n");
        free(vetor_resultados);
        free(vetor_atletas);
        return;
    }

    // 4. PREENCHER O MAPA (Varrendo Resultados)
    printf("--> Processando medalhas de ouro...\n");
    int ouros_encontrados = 0;

    for (int acc = 0; acc < qtd_resultados; acc++) {
        // Verifica se os campos existem antes de comparar strings
        if (vetor_resultados[acc].noc[0] != '\0' && vetor_resultados[acc].medal[0] != '\0') {
            
            // Se for do país alvo E for medalha de Ouro
            if (strcmp(vetor_resultados[acc].noc, pais_alvo) == 0 && 
                strcmp(vetor_resultados[acc].medal, "Gold") == 0) {
                
                int id = vetor_resultados[acc].athlete_id;

                // Proteção de limites
                if (id >= 0 && id < MAX_MAPA_ID) {
                    mapa_ouros[id]++; 
                    ouros_encontrados++;
                }
            }
        }
    }
    printf("--> Total de ouros do pais encontrados: %d\n", ouros_encontrados);

    // 5. CRUZAR COM BIOGRAFIAS (Verificar quem tem > 1 ouro e o sexo)
    printf("--> Verificando generos dos multi-medalhistas...\n");
    
    int homens_multi = 0;
    int mulheres_multi = 0;

    for (int acumulador = 0; acumulador < qtd_atletas; acumulador++) {
        int id = vetor_atletas[acumulador].id;

        // Se o ID é valido e o atleta tem MAIS DE 1 ouro no mapa
        if (id >= 0 && id < MAX_MAPA_ID && mapa_ouros[id] > 1) {
            
            char* sexo = vetor_atletas[acumulador].sex;

            if (strcmp(sexo, "Male") == 0 || strcmp(sexo, "M") == 0) {
                homens_multi++;
            } 
            else if (strcmp(sexo, "Female") == 0 || strcmp(sexo, "F") == 0) {
                mulheres_multi++;
            }

            // Zera para não contar o mesmo atleta duas vezes (caso apareça duplicado no CSV)
            mapa_ouros[id] = 0; 
        }
    }

    // 6. EXIBIR RESULTADOS
    printf("\n#######################################\n");
    printf("RESULTADO FINAL - %s\n", pais_alvo);
    printf("Homens com mais de 1 Ouro:   %d\n", homens_multi);
    printf("Mulheres com mais de 1 Ouro: %d\n", mulheres_multi);
    printf("#######################################\n");

    // Limpeza de memória (Bagulho chato...)
    free(vetor_resultados);
    free(vetor_atletas);
    free(mapa_ouros);
}

int main() {
    // Pode trocar "BRA" por "USA", "FRA", "CHN", etc.
    resolver_questao_3("BRA");
    return 0;
}