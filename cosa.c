#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

typedef struct{
    double coeficiente;
    double exponente;
}Numero;

typedef struct{
    double ihat;
    double jhat;
}Vector;

typedef struct{
    Numero * terminos;
    int longitud;
}Expresion;

typedef struct{
    int filas;
    int columnas;
    char *** cadenas;
    int par;
}Buffer;

typedef struct{
    Vector v1;
    Vector v2;
}Matriz;

int verificaMat(Matriz m){
    if(m.v1.ihat == 0){
        if(m.v1.jhat == 0) return 0;
        return (m.v2.ihat != 0);
    }
    //Restamos la primera a la segunda
    m.v2.ihat /= m.v1.ihat;
    m.v2.jhat -= (m.v2.ihat * m.v1.jhat);

    return (m.v2.jhat != 0);
}

//Creo que por cosa de matematicas, los renglones de la matriz o las columnas deberian de estar invertidas
void rellenaMat(Matriz * m){
    while(1){
        printf("Por favor, ingrese el valor para la componente i de su primer vector : ");
        scanf("%lf", &(*m).v1.ihat);

        printf("Por favor, ingrese el valor para la componente j de su primer vector : ");
        scanf("%lf", &(*m).v1.jhat);
        
        printf("Por favor, ingrese el valor para la componente i de su segundo vector : ");
        scanf("%lf", &(*m).v2.ihat);

        printf("Por favor, ingrese el valor para la componente j de su segundo vector : ");
        scanf("%lf", &(*m).v2.jhat);

        if(verificaMat((*m))) return;
        system("cls");
        printf("La matriz ingresada no corresponde a una transformacion lineal, por favor, intentelo nuevamente\n\n");
    }

}

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

void auxiliaBuffer(Buffer * b, int y, int x){
    char cad[] = " .";
    char col[] = "087";
    if((x==(*b).columnas/2)||(y==(*b).filas/2-(*b).par)){
        col[0] = '2';
        col[1] = '5';
        col[2] = '5';
        if((x==(*b).columnas/2)&&(y==(*b).filas/2-(*b).par)){
            cad[0] = 'O';
            cad[1] = 'O';
        }
        else if((x==(*b).columnas/2)){
            cad[0] = '|';
            cad[1] = '|';
        }
        else{
            cad[0] = '=';
            cad[1] = '=';
        }
    }
    asignarRGB((*b).cadenas[y][x], col, col, col);
    asignarCar((*b).cadenas[y][x], cad);
    if(x > 0) x-=(*b).par;
    asignarRGB((*b).cadenas[(*b).columnas - (*b).par - y - 1][(*b).filas - x - 1], col, col, col);
    asignarCar((*b).cadenas[(*b).columnas - (*b).par - y - 1][(*b).filas - x - 1], cad);
}

//ahi va, falta que se afecten por la transformacion
void rellenaBuffer(Buffer * b){
    int i, j, repeticiones, res, largo, indice;
    largo = (*b).columnas/2;
    repeticiones = (*b).columnas / 10 + 1;

    Numero n = {0,0};
    
    for(i = 0; i < (*b).columnas/2; i++){
        for(j = 0; j < repeticiones; j++){
            n.coeficiente = (double)(j * 5);
            res = (int)evaluarFuncion(&n, i, (*b).columnas);
            if(res <= (largo - (*b).par)){
                indice = largo + res - (*b).par;
                auxiliaBuffer(b, indice, i);

                indice += (*b).par;;
                auxiliaBuffer(b, i, indice);

                indice = largo - res - (*b).par;
                auxiliaBuffer(b, indice, i);

                indice += (*b).par;
                auxiliaBuffer(b, i, indice);
            }
        }
    }
}

void iniciarBuffer(Buffer * b, int filas, int columnas){
    int i, j;

    (*b).filas = filas;
    (*b).columnas = columnas;
    (*b).par = (filas - 1) % 2;

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

void aplicarTra(Matriz * m, Vector * v){
    double num = (*v).ihat;
    (*v).ihat = (((*m).v1.ihat * (*v).ihat) + ((*m).v2.ihat * (*v).jhat));
    (*v).jhat = (((*m).v1.jhat * num) + ((*m).v2.jhat * (*v).jhat));
}

void dibujarFun(Expresion * e, Buffer * b, Matriz * m){
    //debemos de revisar la evaluacion de funciones y considerar hacer polinomios de la forma (x-1)(x+15)...etc
    Vector v;
    int anterior = -1;
    int i, j, indice;
    
    for(i = 0; i < (*b).columnas; i++){
        v.ihat = i-(*b).filas/2+(*b).par;
        v.jhat = 0;
        for(j = 0; j < (*e).longitud; j++){
            v.jhat += evaluarFuncion(&(*e).terminos[j], i, (*b).columnas);
        }
        aplicarTra(m, &v);

        //Aqui hay algo que no cuadra con la validacion, pero se supone que funciona
        if(((v.ihat <= ((*b).columnas / 2) - (*b).par) && (v.ihat >=((-(*b).columnas / 2))))&&
           ((v.jhat <= ((*b).filas / 2) - (*b).par) && (v.jhat >=((-(*b).filas / 2))))){
            //Esta mal la asignacion de coordenadas
            indice = (*b).filas - ((int)v.jhat + (*b).filas / 2) - 1;
            asignarRGB((*b).cadenas[(*b).filas - ((int)v.ihat + (*b).filas / 2) - 1][indice], "081", "217", "133");
            asignarCar((*b).cadenas[(*b).filas - ((int)v.ihat + (*b).filas / 2) - 1][indice], "OO");
            //Falta hacer que se rellene cuando no empiece desde los extremos
            //Falta que funcione con transformaciones
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
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleMode(consola, &modo);
    SetConsoleMode(consola, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    
    Matriz m ;
    Expresion e;
    Buffer b;
    int dummy;
    
    rellenaMat(&m);
    iniciarBuffer(&b, 75,75);
    iniciarFun(&e);
    dibujarFun(&e, &b, &m);
    imprimirBuffer(&b);
    scanf("%d", &dummy);

    
    return 0;
}