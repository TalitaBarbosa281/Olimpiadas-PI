//Para um determinado país, determine a quantidade de atletas do sexo masculino e feminino que já participaram das Olimpíadas

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/structs.h"
#include <ctype.h> 
#include "../include/leitura.h" 

// Código inserido para evitar ocorrência de erros caso um usuário digite em minusculo ou de forma "indefinida", com bRa... ou bRA...
void deixarMaiusculoQ2(char*str) {
   for (int i = 0; str[i] != '\0'; i++) {
    str[i] = toupper(str[i]);
   }
}

void participacaoPorGenero() {
    
    // Declarando as variáveis:
    
    int Feminino, Masculino;
    Masculino = 0;
    Feminino = 0;
    char pais[30]; // removendo os acentos da palavra país
    char linha[4096]; // aumentando os valores, depois de algumas sugestões sobre o problema do resultado
    char campo_atual[1024];
    char genero_atual[50]; // Criada para guardar a informação do sexo do atleta
    
// recebendo o país escolhido e testando a abertura do arquivo:
    
    printf("Digite o NOC do país: ");
    scanf("%s", pais);
    deixarMaiusculoQ2(pais); //chamando para fazer a função auxiliar funcionar

    FILE *data = fopen("bios.csv", "r");
    if (data == NULL) {
        printf("Erro ao abrir o arquivo!\n");   
        return; // removendo o 1
    }
    
// Mudando o filtro: pois, depois de alguns testes o resultado não é compatível com a resposta do site, continua não sendo...

        // Fazendo uma busca pela posição do NOC ->(interpretado como o país representante do atleta)

        fgets(linha, sizeof(linha), data); // Pulando a linha do cabeçalho
    
     // Fazendo uma busca por coluna, depois da limpeza de vírgulas dentro de aspas, graças a função acrescentada no #include leitura.h
     while (fgets(linha, sizeof(linha), data)) { 
      char *ptr = linha;

     // Pulando as colunas que antecedem a coluna que contém o NOC:
        ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual)); // pula 1º coluna
        ptr = csv_next_field(ptr, genero_atual, sizeof(campo_atual)); // coluna do sexo dos atletas (a informação que queremos)
        ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual)); // pula...
        ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual)); // pula...
        ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual));
        ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual));
        ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual)); // coluna que contém o NOC

     // Na posição da coluna desejada fazemos uma comparação com o dado do campo_atual com o país procurado
            // Evita a possiblidade de erro na leitura
                 campo_atual[strcspn(campo_atual, "\r\n")] = 0; 
                 genero_atual[strcspn(genero_atual, "\r\n")] = 0;

        if (strcmp(campo_atual, pais) == 0) {
         // Se o país procurado for encontrado, verifica o sexo do atleta
         if (genero_atual[0] == 'M') {   // Comparando com o primeiro caractere para otimizar o tempo  
          Masculino++;
         } else if (genero_atual[0] == 'F') {
                  Feminino++;
            }
            } 
        }
     
    printf("O/a %s teve %d atletas masculinos e %d atletas femininas que participaram das Olimpíadas de Inverno e Verão nos últimos anos.", pais, Masculino, Feminino);
    // Infelizmente o resultado ainda não está igual ao esperado, existem "limpezas" nescessárias ainda não percebidas, eu acho...
    fclose(data);
    
    return 0;
} 


