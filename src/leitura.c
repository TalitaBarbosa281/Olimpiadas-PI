#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "leitura.h"

/* Isso aqui é uma função auxiliar que usaremos para realizar a "leitura" dos arquivos .CSV, de forma a não precisar repetir a leitura desnecessariamente
O objetivo de csv_next_field é ler cada campo até a próxima vírgula, respeitando as aspas. Antes de implementar essa tática, tinhamos o problema da leitura simplesmente cortar de forma errada por causa de vírgulas inseridas entre aspas, como no caso de "Swimming, Men's 100m", em que o leitor começava a entender "Swimming" como um campo só e isso prejudicava toca a leitura sequencial da linha
Os parâmetros são:
line: Ponteiro para onde estamos lendo na linha gigante.
dest: Onde vamos salvar a palavra limpa.
dest_size: Tamanho máximo do buffer de destino para evitar overflow.
*/
char* csv_next_field(char* line, char* dest, int dest_size) {
    char *ptr = line; // Ponteiro que vai "caminhar" pela linha original
    int i = 0; // Índice para escrever no destino
    dest[0] = '\0'; // Limpa o destino inicial

    //Primeiro if evando em consideração campos que começam com aspas
    if (*ptr == '"') {
        ptr++; //pula essa aspa
        // Loop até encontrar a aspa de fechamento ou o fim da linha
        while (*ptr && i < dest_size - 1) {
            //Se encontra uma aspa dentro do texto, faz uma nova verificação para ver se se é uma aspa dupla, que significa uma aspa literal. Não sendo aspa dupla, quer dizer que é fechamento do campo e sai do loop
            if (*ptr == '"') {
                if (*(ptr+1) == '"') {
                    dest[i++] = '"'; ptr += 2;
                } else {
                    ptr++; break; 
                }
            } else {
                dest[i++] = *ptr++;
            }
        }
        //se houver uma vírgula logo após fechar as aspas, pulamos ela
        if (*ptr == ',') ptr++; 

    } else {
        // Nesse caso simples sem aspas, ele vai ler até encontrar virgula, quebra de linha ou fim da string
        while (*ptr && *ptr != ',' && *ptr != '\n' && *ptr != '\r' && i < dest_size - 1) {
            dest[i++] = *ptr++;
        }
        //pula a virgula para preparar o ponteiro para o próximo campo
        if (*ptr == ',') ptr++; 
    }
    dest[i] = '\0'; 
    return ptr; //faz o retorna para a posição onde se para
}

// Implementação da função de carregar resultados (RESULTS.CSV)
Resultado* carregar_resultados(const char* filename, int* qtd) {
    printf("--- [LEITURA] Abrindo arquivo: %s ---\n", filename);
    //Coloquei uma verificação caso ocorra erro do arquivo não estar inserido corretamente
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("ERRO: Arquivo '%s' nao encontrado.\n", filename);
        return NULL;
    }

    /*
    Fazemos uma alocação dinâmica da memória com malloc porque com um arquivo muito grande poderia haver problema para o código funcionar
    */
    int capacidade = 400000; 
    Resultado *lista = (Resultado*) malloc(sizeof(Resultado) * capacidade);
    
    if (lista == NULL) {
        printf("ERRO: Falta de memoria RAM.\n");
        fclose(f);
        return NULL;
    }
    //Os buffers são espaços muito grandes porque, ao longo de testes, ficou evidente que buffers "pequenos" não consumiam o arquivo inteiro
    char linha[4096]; 
    char temp[1024];  
    int count = 0;

    // Pula cabeçalho
    fgets(linha, sizeof(linha), f); 

    /*
    Loop de leitura. Serve para mapear as colunas de results.csv.
    Por exemplo, na coluna 0 converte-se para o int os anos. Na coluna 1 copia-se o texto dos eventos para a struct. Nas colunas 2 e 3, se lê, mas ignora (não vai ser útil para responder as questões aqui). Na coluna 4, se lê as medalhas. Na coluna 5, se lê os nomes. Na coluna 6 se lê o ID dos atletas e na coluna 7 se lê o NOC (país)
    */
    while (fgets(linha, sizeof(linha), f) && count < capacidade) {
        char *ptr = linha;
        
        ptr = csv_next_field(ptr, temp, sizeof(temp));
        lista[count].year = atoi(temp);

        ptr = csv_next_field(ptr, lista[count].event, 300);
        ptr = csv_next_field(ptr, temp, sizeof(temp)); // Team
        ptr = csv_next_field(ptr, temp, sizeof(temp)); // Pos
        ptr = csv_next_field(ptr, lista[count].medal, 15);
        ptr = csv_next_field(ptr, lista[count].athlete_name, 150);
        
        ptr = csv_next_field(ptr, temp, sizeof(temp));
        lista[count].athlete_id = atoi(temp);
        
        ptr = csv_next_field(ptr, lista[count].noc, 10);

        count++; //contador
    }

    fclose(f); //fecha o arquivo
    *qtd = count; //atualiza a varíavel total na main
    printf("--- [LEITURA] Sucesso! %d linhas carregadas. ---\n", count);
    return lista; // retorna o ponteiro do vetor preenchido
}
/*
Aqui é feito para carregar os atletas (em bios.csv). Seguie uma lógica parecida da anterior, mapeando as colunas, que aqui são diferentes, mas a estrutura é a mesma
*/
Atleta* carregar_atletas(const char* filename, int* qtd) {
    printf("--- [LEITURA] Abrindo arquivo de Atletas: %s ---\n", filename);
    
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("ERRO: Arquivo '%s' nao encontrado.\n", filename);
        return NULL;
    }


    int capacidade = 200000; 
    Atleta *lista = (Atleta*) malloc(sizeof(Atleta) * capacidade);
    
    char linha[4096]; 
    char temp[1024];  
    int count = 0;

    // Pula cabeçalho
    fgets(linha, sizeof(linha), f); 

    while (fgets(linha, sizeof(linha), f) && count < capacidade) {
        char *ptr = linha;
        
        // Mapeamento do BIOS.CSV:
        // Roles
        ptr = csv_next_field(ptr, temp, sizeof(temp));

        // Sex, essa parte conta
        ptr = csv_next_field(ptr, lista[count].sex, 10);

        // Full name
        ptr = csv_next_field(ptr, temp, sizeof(temp));

        // Used name, também é considerado
        ptr = csv_next_field(ptr, lista[count].name, 150);

        ptr = csv_next_field(ptr, temp, sizeof(temp)); // Born
        ptr = csv_next_field(ptr, temp, sizeof(temp)); // Died
        ptr = csv_next_field(ptr, temp, sizeof(temp)); // NOC 
        
        // Athlete ID. Também se considera
        ptr = csv_next_field(ptr, temp, sizeof(temp));
        lista[count].id = atoi(temp);
        count++;
    }

    fclose(f);
    *qtd = count;
    printf("--- [LEITURA] Sucesso! %d atletas carregados. ---\n", count);
    return lista;
}