 //Para um determinado país, determine a quantidade de atletas do sexo masculino e feminino que já participaram das Olimpíadas

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/structs.h"

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


int main() {
    
    // Declarando as variáveis:
    
    //int Masculino, Feminino; // não nescessárias por enquanto
    //Masculino = 0;
    //Feminino = 0;
    int contador = 0;
    //int coluna = 0;
    char país[30];
    char linha[3000];
    char campo_atual[300];
    
// recebendo o país escolhido e testando a abertura do arquivo:
    
    printf("Digite o NOC do país: ");
    scanf("%s", país);
    FILE*data = fopen("data/bios.csv", "r");
    if (data == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    
// Mudando o filtro: pois, depois de alguns testes o resultado não é compatível com a resposta do site


        // Fazendo uma busca pela posição do NOC ->(interpretado como o país representante do atleta);
        //Testando somente a busca pelo país por enquanto

        fgets(linha, sizeof(linha), data); // Pulando a linha do cabeçalho
    
     // Fazendo uma busca por coluna, depois da limpeza de vírgulas dentro de aspas,  // graças a função acrescentada antes da main   
     while (fgets(linha, sizeof(linha), data)) { 
      char *ptr = linha;

     // Pulando as colunas que antecedem a coluna que contém o NOC:
        ptr = csv_next_field(ptr, campo_atual, 300); 
        ptr = csv_next_field(ptr, campo_atual, 300);
        ptr = csv_next_field(ptr, campo_atual, 300);
        ptr = csv_next_field(ptr, campo_atual, 300);
        ptr = csv_next_field(ptr, campo_atual, 300);
        ptr = csv_next_field(ptr, campo_atual, 300);
        ptr = csv_next_field(ptr, campo_atual, 300);
        
        // Na posição da coluna desejada fazemos uma comparação com o dado do campo_atual com o país procurado
        campo_atual[strcspn(campo_atual, "\r\n")] = 0;
        if (strcmp(campo_atual, país) == 0) {
                 contador++;
            } 
     
    }
    printf("Numero de vezes que o %s aparece: %d\n", país, contador);
    // Infelizmente o resultado ainda não está igual ao esperado, existem "limpezas" nescessárias ainda não percebidas
    fclose(data);
    
    return 0;
}

