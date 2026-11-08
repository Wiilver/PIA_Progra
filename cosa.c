#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

typedef struct{
    double coeficiente;
    double exponente;
}Numero;

typedef struct{
    Numero * terminos;
    int longitud;
}Expresion;

typedef struct{
    int filas;
    int columnas;
    char *** cadenas;
}Buffer;

void asignarRGB(char cadena[], char R[], char G[], char B[]){
    cadena[7]  = R[0];
    cadena[8]  = R[1];
    cadena[9]  = R[2];
    cadena[11] = G[0];
    cadena[12] = G[1];
    cadena[13] = G[2];
    cadena[15] = B[0];
    cadena[16] = B[1];
    cadena[17] = B[2];
}

void asignarCar(char cadena[], char car[]){
    cadena[19] = car[0];
    cadena[20] = car[1];
}

void iniciarBuffer(Buffer * b, int filas, int columnas){
    int i, j;
    char dummy [] = "  ";

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

            //La verdad no sabria explicar porque lo ocupo mover solo con la i y porque no con la j
            if(((i % 5 == 0) || (j % 5 == 0)) && (j != columnas / 2) && (i != filas / 2)){
                asignarRGB((*b).cadenas[i][j], "087", "087", "087");
                //La logica de este modulo no sirve si cambiamos el tamanio
                if(i % 5 == 0) asignarCar((*b).cadenas[i][j], "--");
                else asignarCar((*b).cadenas[i][j], " |");
            }
            else{
                //Esta parte lo del dummy debemos de cambiarlo con asignarCar
                asignarRGB((*b).cadenas[i][j], "255", "255", "255");
                if(i == filas / 2){
                    if((j == (columnas / 2))){
                        dummy [0] = 'O';
                        dummy [1] = 'O';
                    }
                    else{
                        dummy[0] = '=';
                        dummy[1] = '=';
                    }
                }
                else{
                    if(j == (columnas / 2)){
                        dummy [0] = '|';
                        dummy [1] = '|';
                    }
                    else{
                        dummy[0] = ' ';
                        dummy[1] = ' ';
                    }
                }
                asignarCar((*b).cadenas[i][j], dummy);
            } 
            
            (*b).cadenas[i][j][21] = '\033';
            (*b).cadenas[i][j][22] = '[';
            (*b).cadenas[i][j][23] = '0';
            (*b).cadenas[i][j][24] = 'm';
            (*b).cadenas[i][j][25] = '\0';
        }
    }
}

void iniciarFun(Expresion * e){
    int terminos, i;
    double coeficiente ,exponente;
    while(1){
        printf("Por favor, indique el numero de terminos de su funcion, se le recuerda que no se permiten constantes : ");
        scanf("%d", &terminos);
        if(terminos > 0) break;
        system("cls");
        printf("El numero de terminos debe de ser mayor a 0, por favor, intentelo nuevamente\n\n");
    }
    system("cls");
    (*e).longitud = terminos;
    (*e).terminos = calloc(terminos, sizeof(Numero));
    for(i = 0; i < terminos; i++){
        while(1){
            printf("Por favor, ingrese el coeficiente del termino numero %d : ", i+1);
            scanf("%lf", &coeficiente);
            if(coeficiente != 0) break;
            system("cls");
            printf("El coeficiente no puede ser igual a 0, por favor, intentalo nuevamente\n\n");
        }
        system("cls");
        
        while(1){
            printf("Por favor, ingrese el exponente del termino numero %d : ", i+1);
            scanf("%lf", &exponente);
            if(exponente != 0) break;
            system("cls");
            printf("El exponente no puede ser igual a 0, por favor, intentalo nuevamente\n\n");
        }
        system("cls");
        (*e).terminos[i].coeficiente = coeficiente;
        (*e).terminos[i].exponente   = exponente;
    }
}

void dibujarFun(Expresion * e, Buffer * b){
    //No solo eso, debemos de hacer algo respecto a dibujar seguido
    //Tambin debemos de revisar la evaluacion de funciones y considerar hacer polinomios de la forma (x-1)(x+15)...etc
    //La logica de esto no va a servir si cambiamos el tamanio
    double suma;
    int i, j, k;
    for(i = 0; i < (*b).filas; i++){
        for(j = 0; j < (*b).columnas; j++){
            suma = 0;
            for(k = 0; k < (*e).longitud; k++){
                //Esta linea basicamente calcula todo 3x^6 + 8x^3 + 2x^2, siendo la x sacada con x = j - (*b).columnas / 2
                suma+= pow(j -(*b).columnas / 2,(*e).terminos[k].exponente) * (*e).terminos[k].coeficiente;
            }
            if((suma <= ((*b).filas / 2)) && (suma >=((-(*b).filas / 2)))){

                asignarCar((*b).cadenas[(*b).filas - ((int)suma + (*b).filas / 2) - 1][j], "OO");
            }
        }
    }
}

//Se va a borrar
void hacerCuadrado(Buffer * b){
    int i, j;
    for(i = (*b).filas/3; i < (*b).filas/3*2; i++){
        for(j = (*b).columnas/3; j < (*b).columnas/3*2; j++){
            asignarCar((*b).cadenas[i][j], "OO");
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

    Expresion e;
    Buffer b;
    int dummy;
    //Resulta ser que necesitamos uno para cuando la dimension sea par y otra para cuando la dimension sea impar
    iniciarBuffer(&b, 51, 51);
    //hacerCuadrado(&b);
    iniciarFun(&e);
    dibujarFun(&e, &b);
    imprimirBuffer(&b);
    scanf("%d", &dummy);
    return 0;
}