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
    int mx;
    int my;
    int par;
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

double evaluarFuncion(Numero * n, int num, int col){
    return (pow(num - col /2, (*n).exponente) * (*n).coeficiente);
}
//Esta terriblemente mal, pero ahhi va
void rellenaBuffer(Buffer * b){
    int i, j, repeticiones, res, largo, indice;
    largo = (*b).columnas/2;
    repeticiones = (*b).columnas / 10 + 1;

    Numero n = {0,0};
    
    //De normal esperaba que este for, el primero , solo se hiciera la mitad de la longitud de
    //columnas, por ejemplo si midiera 50, que lo hiciera 25 veces
    //Como guia, si tuvieras el punto (1, 0), se esperaba tener (0,1) (-1,0) y (0, -1)
    //Estoy 99 por ciento seguro de que aqui suceden repeticiones de mas
    //Tambien , esto solo funciona cuando la funcion es constante, deberia de cambiar con otras rectas
    for(i = 0; i < (*b).columnas; i++){
        for(j = 0; j < repeticiones; j++){
            n.coeficiente = (double)(j * 5);
            res = (int)evaluarFuncion(&n, i, (*b).columnas);
            if(res <= (largo - (*b).par)){
                indice = largo + res - (*b).par;
                
                asignarRGB((*b).cadenas[indice][i], "087", "087", "087");
                asignarCar((*b).cadenas[indice][i], " .");
                
                indice += (*b).par;;
                
                asignarRGB((*b).cadenas[i][indice], "087", "087", "087");
                asignarCar((*b).cadenas[i][indice], " .");

                indice = largo - res - (*b).par;
                
                asignarRGB((*b).cadenas[indice][i], "087", "087", "087");
                asignarCar((*b).cadenas[indice][i], " .");
                
                indice += (*b).par;
                
                asignarRGB((*b).cadenas[i][indice], "087", "087", "087");
                asignarCar((*b).cadenas[i][indice], " .");
            }
        }
    }
}

void iniciarBuffer(Buffer * b, int filas, int columnas){
    int i, j;

    Numero n = {0, 1};

    (*b).filas = filas;
    (*b).columnas = columnas;
    (*b).par = (filas - 1) % 2;

    //No se porque jeje, pero asi cuadra siempre
    (*b).my = 5 - (((columnas - 1) % 10) / 2);
    (*b).mx = (*b).my - (*b).par;

    (*b).cadenas = calloc(filas, sizeof(char**));
    for(i = 0; i < filas; i++){
        (*b).cadenas[i] = calloc(columnas, sizeof(char*));
        for(j = 0; j < columnas; j++){
            (*b).cadenas[i][j] = calloc(26, sizeof(char));
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
            asignarRGB((*b).cadenas[i][j], "255", "255", "255");
            asignarCar((*b).cadenas[i][j], "  ");
            (*b).cadenas[i][j][21] = '\033';
            (*b).cadenas[i][j][22] = '[';
            (*b).cadenas[i][j][23] = '0';
            (*b).cadenas[i][j][24] = 'm';
            (*b).cadenas[i][j][25] = '\0';
        }
    }
    rellenaBuffer(b);
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

//Ocupamos hacer que esto sea mas inteligente para que las cosas se vean mas naturales
void rellenaFun(Buffer * b, int inicio, int fin, int x){
    for(fin ; fin < inicio; fin++){
        asignarCar((*b).cadenas[fin][x-1], "OO");
        asignarRGB((*b).cadenas[fin][x-1], "255", "255", "255");
    }
}

void dibujarFun(Expresion * e, Buffer * b){
    //debemos de revisar la evaluacion de funciones y considerar hacer polinomios de la forma (x-1)(x+15)...etc
    double suma;
    int anterior = -1;
    int i, j, indice;
    
    //Comente esta linea y el programa, literalmente, paso de n^2 a n xd
    //for(i = 0; i < (*b).filas; i++){
    for(i = 0; i < (*b).columnas; i++){
        suma = 0;
        for(j = 0; j < (*e).longitud; j++){
            suma+= evaluarFuncion(&(*e).terminos[j], i, (*b).columnas);
        }
        if((suma <= ((*b).filas / 2) - (*b).par) && (suma >=((-(*b).filas / 2)))){
            indice = (*b).filas - ((int)suma + (*b).filas / 2) - 1;
            asignarRGB((*b).cadenas[indice][i], "255", "255", "255");
            asignarCar((*b).cadenas[indice][i], "OO");
            //Falta hacer que se rellene cuando no empiece desde los extremos
            if((anterior != -1)&&(i != 0)){
                if((anterior != indice - 1) && (anterior != indice) && (anterior != indice + 1)){
                    if(anterior < indice) rellenaFun(b, indice, anterior, i);
                    else rellenaFun(b, anterior, indice, i+1);
                }
            }   
            anterior = indice;
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

    iniciarBuffer(&b, 50,50);
    iniciarFun(&e);
    dibujarFun(&e, &b);
    imprimirBuffer(&b);
    scanf("%d", &dummy);
    return 0;
}