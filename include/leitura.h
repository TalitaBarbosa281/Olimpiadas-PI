#ifndef LEITURA_H
#define LEITURA_H

#include "structs.h"
/*Este arquivo atua como uma "Interface Pública", ele avisa ao compilador a existência dessas funções e dos parâmetros que ocorrem, sendo "chamado" por #include "leitura.c"
Sem isso, ocorre o erro de "Implicit Declaration", que não "quebra" o código, mas poderia resultar em erros caso não houvesse isso aqui
*/
// Disponibiliza a função de carregar results para qualquer arquivo que der #include "leitura.h"
Resultado* carregar_resultados(const char *caminho, int *total);
// Disponibiliza a função de carregar bios
Atleta* carregar_atletas(const char *filename, int *qtd);
//Disponibiliza a ferramenta de leitura de CSV
char* csv_next_field(char* line, char* dest, int dest_size);

#endif
