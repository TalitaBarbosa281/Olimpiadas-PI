//Para um determinado país, determine a quantidade de atletas do sexo masculino e feminino que já participaram das Olimpíadas

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/structs.h"
#include <ctype.h> 
#include "../include/leitura.h" 

//Código inserido para evitar ocorrência de erros caso um usuário digite em minusculo ou de forma "indefinida", com bRa... ou bRA...
void deixarMaiusculoQ2(char*str) {
    for (int i = 0; str[i] != '\0'; i++){
        str[i] = toupper(str[i]);
    }
}

void participacaoPorGenero() {
    
    // Declarando as variáveis:
    int masculino = 0;
    int feminino = 0;
    char pais[30]; //removendo os acentos da palavra país
    char linha[4096]; //aumentando os valores, depois de algumas sugestões sobre o problema do resultado
    char campo_atual[1024];
    char genero_atual[50]; //Criada para guardar a informação do sexo do atleta
    
    /*
    Durante o desenvolvimento do código descobrimos o problema de alguns dados não baterem e certos países não serem lidos corretamente. Isso finalmente foi identificado pelo fato de, em bios.csv, alguns atletas não terem NOC identificado corretamente ou nomes dos países estarem escritos, a exemplo da Russia, de forma não usual (Russian Federation), o que atrapalha a utilização para o usuário. Com isso, inserimos results.csv no código para corrigir o problema de leitura
    Criamos uma lista gigante (até o ID 200.000). 
    Se a posição 10 tiver o valor '1', significa que o atleta de ID 10 é do país escolhido.
    Usamos 'static' para limpar a memória automaticamente (iniciar com zeros).
    */
    static char ids_do_pais[200000]; 
    
    // Garante limpeza caso a função seja reexecutada no mesmo ciclo do programa
    memset(ids_do_pais, 0, sizeof(ids_do_pais));
    // recebendo o país escolhido e testando a abertura do arquivo:
    printf("\n>>> Q2: Participacao por Genero <<<\n");
    printf("Digite o NOC do pais (Ex: BRA): ");
    scanf("%s", pais);
    deixarMaiusculoQ2(pais); 

    // --- ETAPA 1: INDEXAÇÃO (Leitura do results.csv) ---
    // Objetivo: Identificar quais IDs de atletas representam a sigla digitada (Ex: BRA)
    FILE *f_res = fopen("results.csv", "r");
    if (!f_res) {
        printf("[ERRO CRITICO] Arquivo 'results.csv' nao encontrado.\n");
        return;
    }
    // Pula cabeçalho
    fgets(linha, sizeof(linha), f_res); 

    while (fgets(linha, sizeof(linha), f_res)) {
        char *ptr = linha;
        char noc_lido[10];
        char id_str[20];
        
        // Pula as 6 primeiras colunas (Games, Event, etc) para chegar no ID
        for(int i=0; i<6; i++) ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual));
        
        ptr = csv_next_field(ptr, id_str, sizeof(id_str));     // Coluna: athlete_id
        ptr = csv_next_field(ptr, noc_lido, sizeof(noc_lido)); // Coluna: NOC

        // Se o registro corresponde ao país buscado, marcamos na tabela
        if (strcmp(noc_lido, pais) == 0) {
            int id = atoi(id_str);
            if(id > 0 && id < 200000) ids_do_pais[id] = 1; 
        }
    }
    fclose(f_res);

    FILE *data = fopen("bios.csv", "r");
    if (data == NULL) {
        printf("Erro ao abrir o arquivo!\n");   
        return; // removendo o 1
    }
    
    
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
        ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual)); // coluna que contém o país

        // Lê o ID do atleta (está na última coluna)
        ptr = csv_next_field(ptr, campo_atual, sizeof(campo_atual)); // athlete_id
        int id_atleta = atoi(campo_atual);

        // Limpeza de caracteres de controle
        genero_atual[strcspn(genero_atual, "\r\n")] = 0;

        // Aqui realizamos a atividade de filtragem onde vamos descobrir o id do atleta e assim realizar a contagem do numero de atletas por país corretamente
        if (id_atleta > 0 && id_atleta < 200000 && ids_do_pais[id_atleta] == 1) {
            
            if (genero_atual[0] == 'M') masculino++;
            else if (genero_atual[0] == 'F') feminino++;
            
            // Zera a marcação para não contar o mesmo atleta duas vezes (evita duplicatas)
            ids_do_pais[id_atleta] = 0; 
        } 
    }
     
    printf("O/a %s teve %d atletas masculinos e %d atletas femininas que participaram das Olimpiadas de Inverno e Verao nos ultimos anos.\n", pais, masculino, feminino);
    printf("No total, sao %d atletas", masculino+feminino);

    fclose(data);

} 
