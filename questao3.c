//Para um determinado país, determine o número de atletas do sexo masculino e feminino que conquistaram mais de uma medalha de ouro

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

#define MAX_LINE 1024

// metodo para pegar o campo de uma linha do csv

char* get_field(char* linha, int num_campo) {

    char* copia_linha = strdup(linha); // Cria uma cópia para não estragar a original
    char* pedaco = strtok(copia_linha, ","); // Divide a linha em pedaços separados por vírgula
    char* resultado = NULL; // vai guardar o dado
    int contador = 0; // vai definir qual a coluna o codigo esta lendo

    while (pedaco != NULL) {

        if (contador == num_campo) {
            resultado = strdup(pedaco); // Salva o valor para retornar
            break;
        }
        pedaco = strtok(NULL, ","); // faz o codigo continuar lendo a linha de onde parou
        contador++;

    }

    free(copia_linha); // Limpa a memória
    return resultado; // Retorna o valor
}

// metodo para ler o bios.csv buscando o genero dos atletas
FILE *funcao_bios = fopen("output/bios.csv", "r");
    if (!funcao_bios) {

        printf("Erro: Nao foi possivel abrir o arquivo bios.csv\n");
        return;

    }

    fgets(buffer, MAX_LINE, funcao_bios); // Pula cabeçalho, esta função previne o erro de leitura do cabeçalho como dados no while


