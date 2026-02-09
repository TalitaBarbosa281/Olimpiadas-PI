//Para um determinado país, determine o número de atletas do sexo masculino e feminino que conquistaram mais de uma medalha de ouro

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Incluindo o arquivo que contém as funções de ler CSV
#include "leitura.c" 

// Define o tamanho do Mapa de IDs
// Coloquei 200.000 para ter uma margem de segurança e evitar erros de memória (ACONTECEU)
#define MAX_MAPA_ID 200000

void resolver_questao_3(char* pais_alvo) {
    printf("\n=== INICIANDO ANALISE PARA O PAIS: %s ===\n", pais_alvo);

    // Guardando a quantidade de linhas/atletas carregados para usar nos loops depois
    int qtd_resultados = 0;
    int qtd_atletas = 0;

    // ---------------------------------------------------------
    // PASSO 1: Carregar os dados do arquivo de Resultados (as medalhas)
    // ---------------------------------------------------------
    
    // A função carregar_resultados (que está no leitura.c)
    Resultado* vetor_resultados = carregar_resultados("../data/results.csv", &qtd_resultados);
    
    // Me previnindo de um possível erro na leitura do arquivo, caso de erro eu vou saber mais facilmente e vou evitar que o código trave ou de merda
    if (vetor_resultados == NULL) {
        printf("Falha ao ler results.csv\n");
        return;
    }

    // ---------------------------------------------------------
    // PASSO 2: Carregar os dados do arquivo bios.csv(Atletas)
    // ---------------------------------------------------------
    Atleta* vetor_atletas = carregar_atletas("../data/bios.csv", &qtd_atletas);

    // Verificação de segurança para o segundo arquivo
    if (vetor_atletas == NULL) {

        // Me previnindo de um possível erro na leitura do arquivo, caso de erro eu vou saber mais facilmente e vou evitar que o código trave ou de merda
        printf("Falha ao ler bios.csv.\n"); 
        free(vetor_resultados); // Libera o que já tínhamos carregado antes de sair
        return;

    }

    // ---------------------------------------------------------
    // PASSO 3: Criar o Mapa para contagem
    // ---------------------------------------------------------

    // Usei calloc em vez de malloc
    // Diferença: O calloc já deixa tudo com zeros, enquanto o malloc deixa lixo de memória (valores aleatórios) que podem causar erros se não forem inicializados
    // A lógica aqui é: O índice do vetor será o ID do atleta
    // Ex: mapa_ouros[10] guarda quantas medalhas o atleta de ID 10 tem
    int* mapa_ouros = (int*) calloc(MAX_MAPA_ID, sizeof(int));

    if (mapa_ouros == NULL) {
        printf("Memoria RAM insuficiente para criar o mapa\n");
        free(vetor_resultados);
        free(vetor_atletas);
        return;
    }

    // ---------------------------------------------------------
    // PASSO 4: Contar Medalhas de Ouro
    // ---------------------------------------------------------
    printf("--> Processando medalhas de ouro...\n");
    int ouros_encontrados = 0;

    // Loop que passa por todas as linhas do arquivo de resultados
    for (int acumulado = 0; acumulado < qtd_resultados; acumulado++) {
        
        // Verifica se os campos NOC (País) e Medal não estão vazios ou corrompidos
        if (vetor_resultados[acumulado].noc[0] != '\0' && vetor_resultados[acumulado].medal[0] != '\0') {
            
            // Compara se o país é o que queremos (ex: BRA) e se a medalha é de ouro
            // strcmp retorna 0 se as strings forem iguais
            if (strcmp(vetor_resultados[acumulado].noc, pais_alvo) == 0 && 
                strcmp(vetor_resultados[acumulado].medal, "Gold") == 0) {
                
                // Pega o ID do atleta que ganhou essa medalha
                int id = vetor_resultados[acumulado].athlete_id;

                // Proteção: Só acessa o vetor se o ID for válido
                if (id >= 0 && id < MAX_MAPA_ID) {
                    mapa_ouros[id]++; // Adiciona +1 medalha para esse ID
                    ouros_encontrados++;
                }
            }
        }
    }
    printf("--> Total de medalhas de ouro do %s encontradas: %d\n", pais_alvo, ouros_encontrados);

    // ---------------------------------------------------------
    // PASSO 5: Verificar Sexo
    // ---------------------------------------------------------
    printf("--> Verificando generos dos multi-medalhistas...\n");
    
    int homens_multi = 0;   // Contador final de homens
    int mulheres_multi = 0; // Contador final de mulheres

    // Agora percorre a lista de atletas (bios.csv)
    for (int acumulador = 0; acumulador < qtd_atletas; acumulador++) {
        
        int id = vetor_atletas[acumulador].id; // Pega o ID do atleta atual

        // Checa o mapa, se esse ID acumulou MAIS DE 1 medalha de ouro
        if (id >= 0 && id < MAX_MAPA_ID && mapa_ouros[id] > 1) {
            
            // Se sim, pega o sexo dele
            char* sexo = vetor_atletas[acumulador].sex;

            // Verifica se é Homem 
            if (strcmp(sexo, "Male") == 0) {
                homens_multi++;
            } 
            // Verifica se é Mulher
            else if (strcmp(sexo, "Female") == 0) {
                mulheres_multi++;
            }

            // IMPORTANTE:
            // Zero o contador desse atleta no mapa
            // Motivo: Se o atleta aparecer duplicado no arquivo bios.csv (acontece KK),
            // ele não contara duas vezes, pois na segunda vez o valor sera 0.
            mapa_ouros[id] = 0; 
        }
    }

    // ---------------------------------------------------------
    // PASSO 6: Exibir Resultados e Limpar Memoria (menu q sera usado dps)
    // ---------------------------------------------------------
    printf("\n#######################################\n");
    printf("RESULTADO FINAL - %s\n", pais_alvo);
    printf("Homens com mais de 1 Ouro:   %d\n", homens_multi);
    printf("Mulheres com mais de 1 Ouro: %d\n", mulheres_multi);
    printf("#######################################\n");

    // Free libera a memória RAM que o calloc pegou
    // É boa prática sempre limpar a memoria antes de sair. (Evitar vazamentos de memória para não dar merda)
    free(vetor_resultados);
    free(vetor_atletas);
    free(mapa_ouros);
}

// Função principal que o sistema operacional chama para iniciar o programa 
int main() {

    // Você pode mudar para "USA", "FRA", "CHN" para testar outros países.
    resolver_questao_3("BRA");
    
    return 0; 
}