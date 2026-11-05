#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
typedef struct{
    int filas;
    int columnas;
    char *** cadenas;
}Buffer;

void iniciarBuffer(Buffer * b, int filas, int columnas){
    int i, j;
    
    (*b).filas = filas;
    (*b).columnas = columnas;
    (*b).cadenas = calloc(filas, sizeof(char**));
    for(i = 0; i < filas; i++){
        (*b).cadenas[i] = calloc(columnas, sizeof(char*));
        for(j = 0; j < columnas; j++){
            (*b).cadenas[i][j] = calloc(27, sizeof(char));
            (*b).cadenas[i][j][0] ='\033';
            (*b).cadenas[i][j][1] ='[';
            (*b).cadenas[i][j][2] ='3';
            (*b).cadenas[i][j][3] ='8';
            (*b).cadenas[i][j][4] =';';
            (*b).cadenas[i][j][5] ='2';
            (*b).cadenas[i][j][6] =';';
            (*b).cadenas[i][j][10] =';';
            (*b).cadenas[i][j][14] =';';
            (*b).cadenas[i][j][18] ='m';

            if(((i%5==0)||(j%5==0))&&(j!=columnas/2)&&(i!=filas/2)){
                (*b).cadenas[i][j][7] ='0';
                (*b).cadenas[i][j][8] ='8';
                (*b).cadenas[i][j][9] ='7';
                (*b).cadenas[i][j][11] ='0';
                (*b).cadenas[i][j][12] ='8';
                (*b).cadenas[i][j][13] ='7';
                (*b).cadenas[i][j][15] ='0';
                (*b).cadenas[i][j][16] ='8';
                (*b).cadenas[i][j][17] ='7';
                if(i%5==0){
                    (*b).cadenas[i][j][19] ='-';
                    (*b).cadenas[i][j][20] ='-';    
                }
                else{
                    (*b).cadenas[i][j][19] =' ';
                    (*b).cadenas[i][j][20] ='|';
                }
            }
            else{
                (*b).cadenas[i][j][7] ='2';
                (*b).cadenas[i][j][8] ='5';
                (*b).cadenas[i][j][9] ='5';
                (*b).cadenas[i][j][11] ='2';
                (*b).cadenas[i][j][12] ='5';
                (*b).cadenas[i][j][13] ='5';
                (*b).cadenas[i][j][15] ='2';
                (*b).cadenas[i][j][16] ='5';
                (*b).cadenas[i][j][17] ='5';
                if(i==filas/2){
                    (*b).cadenas[i][j][19] = '-';
                    if(j==columnas/2) (*b).cadenas[i][j][20] = 'O';
                    else (*b).cadenas[i][j][20] = '-';    
                }
                else{
                    (*b).cadenas[i][j][19] = ' ';    
                    if(j == columnas/2) (*b).cadenas[i][j][20] = '|';
                    else (*b).cadenas[i][j][20] = ' ';
                }
            } 
            
            (*b).cadenas[i][j][21] = '\033';
            (*b).cadenas[i][j][22] = '[';
            (*b).cadenas[i][j][23] = '0';
            (*b).cadenas[i][j][24] = 'm';
            (*b).cadenas[i][j][25] = '\0';
        }
    }
}

void hacerCuadrado(Buffer * b){
    int i, j;
    for(i = (*b).filas/3; i < (*b).filas/3*2; i++){
        for(j = (*b).columnas/3; j < (*b).columnas/3*2; j++){
            (*b).cadenas[i][j][19] = 'O';
            (*b).cadenas[i][j][20] = 'O';
        }
    }
}

void imprimirBuffer(Buffer * b){
    int i, j;
    for(i = 0; i < (*b).filas; i++){
        for(j = 0; j < (*b).columnas; j++) printf("%s", (*b).cadenas[i][j]);
        printf("\n");
    }
}

int main(){
    HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo;
    GetConsoleMode(consola, &modo);
    SetConsoleMode(consola, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    Buffer b;
    int dummy;
    iniciarBuffer(&b, 50, 50);
    //hacerCuadrado(&b);
    imprimirBuffer(&b);
    scanf("%d", &dummy);
    return 0;
}