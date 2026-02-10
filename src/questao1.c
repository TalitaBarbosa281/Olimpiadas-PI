//Considere 10 países a sua escolha. Crie um ranking dos países com base no número total de medalhas conquistadas. Ordene os países pelo total de medalhas

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/structs.h"
#include "../include/leitura.h"

// -------------------------------- Inicio do gráfico --------------------------------

void gerar_grafico_rankingQ1(PaisStats placar[]) {
    FILE *gnuplotPipe = popen("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persistent", "w");

    if (gnuplotPipe == NULL) {
        printf("Gnuplot nao encontrado (verifique o caminho).\n");
        return;
    }

    printf("\nGerando grafico...\n");

    // Configurações visuais
    fprintf(gnuplotPipe, "set terminal pngcairo size 1000,600 enhanced font 'Verdana,10'\n");
    fprintf(gnuplotPipe, "set output 'ranking_medalhas.png'\n");
    
    fprintf(gnuplotPipe, "set title 'Top 10 Paises Escolhidos - Total de Medalhas'\n");
    fprintf(gnuplotPipe, "set ylabel 'Total de Medalhas'\n");
    fprintf(gnuplotPipe, "set xlabel 'Paises'\n");
    
    // Estilo das barras
    fprintf(gnuplotPipe, "set style fill solid 1.0 border -1\n");
    fprintf(gnuplotPipe, "set boxwidth 0.5\n");
    fprintf(gnuplotPipe, "set grid y\n");
    
    // Define o limite do eixo Y com uma margem de segurança
    int max_medalhas = placar[0].total_medalhas; // O primeiro é o maior pq está ordenado
    
    int teto_grafico = max_medalhas + (max_medalhas * 0.1); // se o pais campeão tem 50 medalhas, o grafico vai até 55
    if (teto_grafico == 0) {
        teto_grafico = 10; // Se ninguém tem medalha, define o teto como 10 só para exibir o gráfico vazio (so para evitar uma msg de erro)
    }
    
    fprintf(gnuplotPipe, "set yrange [0:%d]\n", teto_grafico); // envia o comando

    // Plotagem
    fprintf(gnuplotPipe, "plot '-' using 2:xtic(1) with boxes notitle linecolor rgb '#2E8B57'\n");

    // Enviar dados (NOC e Total)
    for (int i = 0; i < 10; i++) {
        fprintf(gnuplotPipe, "\"%s\" %d\n", placar[i].noc, placar[i].total_medalhas);
    }
    
    fprintf(gnuplotPipe, "e\n");

    fflush(gnuplotPipe);
    pclose(gnuplotPipe);

    printf("Grafico gerado: 'ranking_medalhas.png'\n");
}

// -------------------------------- Fim do gráfico --------------------------------

// Essa função percorre a string recebida e transforma cada caractere em maiúsculo. Isso evita erro quando o usuário digita siglas em minúsculo.
void deixarMaiusculoQ1(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}


// Essa função verifica se o código NOC digitado existe dentro dos dados carregados. Ela percorre todos os registros comparando as siglas.
int validarNOCQ1(char *noc, Resultado *dados, int totalDeDados) {
    for (int i = 0; i < totalDeDados; i++) {
        if (strcmp(dados[i].noc, noc) == 0) {
            return 1;  // encontrado
        }
    }
    return 0; // não encontrado
}

// Função principal responsável por montar o ranking
void ordenarRanking(Resultado *dados, int totalDeDados){

    printf("\nRanking total\n");

    // Vetor que guarda as estatísticas dos países escolhidos
    PaisStats placar[10];
    int count = 0;

    // Loop que permite o usuário escolher os 10 países
    while(count < 10){

        char entrada[100];

        printf("Digite o NOC do %d.o pais: ", count+1);
        scanf("%s", entrada);

        // Padroniza entrada para evitar erro de comparação
        deixarMaiusculoQ1(entrada);

        // Verifica tamanho da sigla
        if(strlen(entrada) != 3){
            printf("Sigla deve ter 3 letras\n");
            continue;
        }

        // Verifica se existe nos dados
        if(validarNOCQ1(entrada, dados, totalDeDados)){

            int repetido = 0;

            // Evita inserir país duplicado
            for(int j=0;j<count;j++){
                if(strcmp(placar[j].noc, entrada)==0){
                    repetido = 1;
                    break;
                }
            }

            if(repetido){
                printf("Pais repetido, escolha outro\n");
            }
            else{
                strcpy(placar[count].noc, entrada);
                placar[count].total_medalhas = 0;
                printf(" -> %s adicionado\n", entrada);
                count++;
            }
        }
        else{
            printf("Codigo nao encontrado\n");
        }
    }


    // Percorre todos os dados carregados
    // Aqui acontece a contagem de medalhas e atletas
    for(int i=0;i<totalDeDados;i++){
        // Se não tem medalha ("NA" ou vazio), pula para o próximo
        if (strcmp(dados[i].medal, "NA") == 0 || strlen(dados[i].medal) < 2) {
            continue;
        }
            // Verifica se o registro pertence a um dos países escolhidos
            for (int k = 0; k < 10; k++) {
            if (strcmp(dados[i].noc, placar[k].noc) == 0) {
                placar[k].total_medalhas++;
                break; // Achou o país, não precisa verificar os outros 9
            }
        }
    }

   //Ordenando por meio de um bubbleSort, um outro método de ordenação diferente do quicksort, sendo mais claro em questão de construção lógica, ao mesmo tempo que é mais "robusto" visualmente
   for (int i = 0; i < 10 - 1; i++) { //o primeiro for serve para definir quantas vezes vai analisar a lista
        for (int j = 0; j < 10 - i - 1; j++) { // o segundo for serve para fazer as comparações. O -1 está presente porque não é necessário observar o último, uma vez que já está ordenado
            // Compara o atual com o próximo
            if (placar[j].total_medalhas < placar[j+1].total_medalhas) {
                // TROCA (SWAP)
                PaisStats temporario = placar[j];
                placar[j] = placar[j+1];
                placar[j+1] = temporario;
            }
        }
    }

    // Impressão do ranking final
    printf("\n%-10s | %s\n", "PAIS", "TOTAL MEDALHAS");
    printf("---------------------------\n");
        for (int i = 0; i < 10; i++) {
            printf("%-10s | %d\n", placar[i].noc, placar[i].total_medalhas);
    }
    printf("\n");
    gerar_grafico_rankingQ1(placar);
    }

