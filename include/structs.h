#ifndef STRUCTS_H
#define STRUCTS_H


// esse arquivo de struct serve de auxiliar para as questões, diminuindo as linhas de código que seriam repetidas de forma desnecessária no código principal

typedef struct {
    int id;             // Coluna 'athlete_id'
    char name[150];     // Coluna 'Used name'
    char sex[10];       // Coluna 'Sex' 
    char noc[10];       // Coluna 'NOC' 
} Atleta;


typedef struct {
    int year;               // Coluna 'Games'
    char event[300];        // Coluna 'Event'
    char athlete_name[150]; // Coluna 'As' (Nome do atleta no momento da prova)
    int athlete_id;         // Coluna 'athlete_id'
    char noc[10];           // Coluna 'NOC' (País que ele representou nesta prova)
    char medal[15];         // Coluna 'Medal' (Gold, Silver, Bronze, NA)
} Resultado;

typedef struct {
    char noc[10];  
    int qtd_ouros;          
    int qtd_pratas;
    int qtd_bronzes;
    int total_medalhas;     // Soma simples
    int pontuacao_total;    // Soma ponderada
} PaisStats;

typedef struct {
    int athlete_id;
    char sex[10];           
    int count_gold;         // Quantos ouros ganhou
    int participated;       // Flag para saber se já contamos ele
} AtletaStats;

#endif