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

typedef struct{
    Matriz expansiones;
    Matriz cortes;
}Transformaciones;

void aplicarTra(Matriz * m, Vector * v){
    double num = (*v).ihat;
    (*v).ihat = (((*m).v1.ihat * (*v).ihat) + ((*m).v2.ihat * (*v).jhat));
    (*v).jhat = (((*m).v1.jhat * num) + ((*m).v2.jhat * (*v).jhat));
}

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

void transfElem(Matriz m, Transformaciones * t){
    double dummy;
    if((m.v1.ihat) && (m.v1.jhat) && (m.v2.ihat)&& (m.v2.jhat)){
        dummy = ((m.v1.ihat * m.v2.jhat) - (m.v1.jhat * m.v2.ihat));
        (*t).expansiones.v2.jhat = m.v2.jhat;
        (*t).expansiones.v1.ihat = dummy/m.v2.jhat;
        (*t).expansiones.v1.jhat = 0;
        (*t).expansiones.v2.ihat = 0;

        (*t).cortes.v1.jhat = m.v1.jhat/m.v2.jhat;
        (*t).cortes.v2.ihat = (m.v2.ihat * m.v2.jhat)/dummy;
        (*t).cortes.v2.jhat = 1;
        (*t).cortes.v1.ihat = 1 + ((*t).cortes.v1.jhat * (*t).cortes.v2.ihat);
        return;
    }

    (*t).expansiones = m;
    (*t).cortes.v1.ihat = 1;
    (*t).cortes.v1.jhat = 0;
    (*t).cortes.v2.ihat = 0;
    (*t).cortes.v2.jhat = 1;
    
    if(((!m.v1.jhat) && (!m.v2.ihat)) || ((!m.v1.ihat) && (!m.v2.jhat))) return;
    
    if(!m.v1.ihat){
        (*t).expansiones.v1.ihat = (*t).expansiones.v2.ihat;
        (*t).expansiones.v2.jhat = (*t).expansiones.v1.jhat;
        (*t).expansiones.v1.jhat = 0;
        (*t).expansiones.v2.ihat = 0;
        (*t).cortes.v1.ihat = 0;
        (*t).cortes.v1.jhat = 1;
        (*t).cortes.v2.ihat = 1;
        (*t).cortes.v2.jhat = m.v2.jhat / m.v1.jhat;
        return;
    }
    
    if(!m.v2.ihat){
        (*t).expansiones.v1.jhat = 0;
        (*t).cortes.v1.jhat = m.v1.jhat/m.v2.jhat;
        return;
    }
    
    if(!m.v1.jhat){
        (*t).expansiones.v2.ihat = 0;
        (*t).cortes.v2.ihat = m.v2.ihat/m.v1.ihat;
        return;
    }
    
    (*t).expansiones.v1.ihat = 0;
    (*t).cortes.v1.jhat = m.v1.ihat/m.v2.ihat;
}

void rellenaMat(Matriz * m, Transformaciones * t){
    while(1){
        printf("Por favor, ingrese el valor para la componente i de su primer vector : ");
        scanf("%lf", &(*m).v1.ihat);

        printf("Por favor, ingrese el valor para la componente j de su primer vector : ");
        scanf("%lf", &(*m).v1.jhat);
        
        printf("Por favor, ingrese el valor para la componente i de su segundo vector : ");
        scanf("%lf", &(*m).v2.ihat);

        printf("Por favor, ingrese el valor para la componente j de su segundo vector : ");
        scanf("%lf", &(*m).v2.jhat);

        if(verificaMat((*m))){
            transfElem((*m), t);
            return;
        } 
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


double evaluFun(Numero * n, int num, int col){
    return (pow(num - col /2, (*n).exponente) * (*n).coeficiente);
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

void auxiliaFun(Buffer * b, Vector v, Matriz * m){
    int x, y;
    aplicarTra(m, &v);
    y = ((*b).filas / 2 - (*b).par) - (int)round(v.jhat);
    x = (int)round(v.ihat) + (*b).columnas / 2;
    if((x < 0)||(x >= (*b).columnas)||(y < 0)||(y >= (*b).filas)) return;
    asignarRGB((*b).cadenas[y][x], "081", "217", "133");
    asignarCar((*b).cadenas[y][x], "OO");
}

void auxiliaRel(Buffer * b, Vector v,Matriz* m, int ini, int fin, int modo){
    for(ini; ini > fin; ini--){
        Vector dummy = v;
        if(modo) dummy.jhat = (double)ini;
        else dummy.ihat = (double)ini;
        auxiliaFun(b,dummy,m);
    }
}

void rellenaFun(Buffer * b, Vector anterior, Vector punto, Transformaciones * t){
    int i, pasos;
    double dx = punto.ihat - anterior.ihat;
    double dy = punto.jhat - anterior.jhat;
    
    pasos = (int)ceil(fmax(fabs(dx), fabs(dy)));
    
    if(pasos <= 1) return;

    for(i= 1; i < pasos; i++){
        double factor = (double)i / pasos;
        Vector dummy;
        dummy.ihat = anterior.ihat + dx * factor;
        dummy.jhat = anterior.jhat + dy * factor;
        auxiliaFun(b, dummy, &(*t).cortes);
    }
}

void dibujarFun(Expresion * e, Buffer * b, Transformaciones * t){
    Vector v;
    Vector anterior = {(*b).columnas,(*b).filas};
    int i, j;
    
    for(i = 0; i < (*b).columnas; i++){
        v.ihat = i - (*b).columnas / 2;
        v.jhat = 0;
        for(j = 0; j < (*e).longitud; j++){
            v.jhat += evaluFun(&(*e).terminos[j], i, (*b).columnas);
        }
        aplicarTra(&(*t).expansiones, &v);

        if((v.jhat < -(*b).columnas/2)||(v.jhat > (*b).columnas/2)||
           (v.ihat < -(*b).filas   /2)||(v.ihat > (*b).filas   /2)) continue;
        
        if((anterior.jhat != (*b).columnas) && (i != 0)){
            if((anterior.jhat > v.jhat + 1) || (anterior.jhat < v.jhat - 1)||
               (anterior.ihat > v.ihat + 1) || (anterior.ihat < v.ihat - 1)){
                rellenaFun(b, anterior, v, t);
            }
        }
        
        anterior = v;
        auxiliaFun(b,v,&(*t).cortes);
    }
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
            res = (int)evaluFun(&n, i, (*b).columnas);
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

void imprimeBuffer(Buffer * b){
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
    
    Matriz m ;
    Transformaciones t;
    Expresion e;
    Buffer b;
    int dummy;
    
    rellenaMat(&m, &t);
    printf("Expansiones\n");
    printf("%lf   %lf\n", t.expansiones.v1.ihat, t.expansiones.v2.ihat);
    printf("%lf   %lf\n", t.expansiones.v1.jhat, t.expansiones.v2.jhat);
    printf("Cortes\n");
    printf("%lf   %lf\n", t.cortes.v1.ihat, t.cortes.v2.ihat);
    printf("%lf   %lf\n", t.cortes.v1.jhat, t.cortes.v2.jhat);
    iniciarBuffer(&b, 45,45);
    iniciarFun(&e);
    dibujarFun(&e, &b, &t);
    imprimeBuffer(&b);
    scanf("%d", &dummy);
    
    return 0;
}