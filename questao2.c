//Para um determinado país, determine a quantidade de atletas do sexo masculino e feminino que já participaram das Olimpíadas
// (a energia caiu, bateria pouca...)
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


int main() {
    
    // Declarando as variáveis:
    
    //int Masculino, Feminino; // não nescessárias por enquanto
    //Masculino = 0;
    //Feminino = 0;
    int contador = 0;
    int coluna = 0;
    char país[30];
    char linha[3000];
    char *token;
    
// recebendo o país escolhido e testando a abertura do arquivo:
    
    printf("Digite o NOC do país: ");
    scanf("%s", país);
    FILE*output = fopen("output/bios.csv", "r");
    if (output == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    
    // Fazendo a leitura por linhas:
    // Mudando o filtro: depois de alguns testes o resultado não é compatível com a resposta do site, ao que parece...
    // O problema é que nem sempre o NOC se encontra na sé
    while (fgets(linha, sizeof(linha), output)) {
      //for (i = 0; i == '\0'; i++) {  // 

      char *token = strtok(linha, ",");

        // Fazendo uma busca pela posição do NOC ->(interpretado como o país representante do atleta);
        //Testando somente a busca pelo país por enquanto;

        while (token != NULL) {
            if (coluna == 6) {
                if (strstr(token, país) != NULL) {
                    printf ("País encontrado: %s\n", token);
                    contador++;
                }
            }   
            token = strtok(NULL, ",");
            coluna++;
        }  
     }
    printf("Número de vezes que o %s aparece: %d\n", país, contador);
    fclose(output);
    
    return 0;
}
