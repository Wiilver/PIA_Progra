#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>

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

int limpiarBufferNum(){
    char basura;
    int rep = 0;
    while(1){
        scanf("%c", &basura);
        if(basura == '\n') break;
        rep++;
    }
    if(rep > 0){
        system("cls");
        printf("Solo se admiten numeros como respuestas, por favor, intentelo nuevamente\n\n");
        return 0;
    }
    return 1;
}

int limpiarBufferStr(char * str){
    int i;
    char basura;
    for(i = 0; i < 20; i++){
        if(str[i] == '\n'){
            str[i] = '\0';
            return 1;
        }
    }
    while(1){
        scanf("%c", &basura);
        if(basura == '\n'){
            system("cls");
            printf("La cadena ingresada supero la longitud maxima, por favor, intentelo nuevamente\n\n");
            return 0;
        } 
    }
}

int verificarStr(char * str){
    int i = 0;
    while(1){
        if(i==20) return 0;
        if(str[i]=='\0'){
            return (i > 0);
        }
        if(
            ((str[i] < 65) || (str[i] > 90))&&
            ((str[i] < 97) || (str[i] > 122))&&
            ((str[i] < 48) || (str[i] > 57))
        )return 0;
        i++;
    }
}


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
        while(1){
            printf("Por favor, ingrese el valor para la componente i de su primer vector : ");
            scanf("%lf", &(*m).v1.ihat);
            if(limpiarBufferNum()) break;
        }

        while(1){
            printf("Por favor, ingrese el valor para la componente j de su primer vector : ");
            scanf("%lf", &(*m).v1.jhat);
            if(limpiarBufferNum()) break;
        }

        while(1){
            printf("Por favor, ingrese el valor para la componente i de su segundo vector : ");
            scanf("%lf", &(*m).v2.ihat);
            if(limpiarBufferNum()) break;
        }

        while(1){
            printf("Por favor, ingrese el valor para la componente j de su segundo vector : ");
            scanf("%lf", &(*m).v2.jhat);
            if(limpiarBufferNum()) break;
        }
        system("cls");
        if(verificaMat((*m))){
            transfElem((*m), t);
            return;
        } 
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
        if(!limpiarBufferNum()) continue;
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
            if(!limpiarBufferNum()) continue;
            if(coeficiente != 0) break;
            system("cls");
            printf("El coeficiente no puede ser igual a 0, por favor, intentalo nuevamente\n\n");
        }
        system("cls");
        
        while(1){
            printf("Por favor, ingrese el exponente del termino numero %d : ", i+1);
            scanf("%lf", &exponente);
            if(!limpiarBufferNum()) continue;
            if(exponente != 0) break;
            system("cls");
            printf("El exponente no puede ser igual a 0, por favor, intentalo nuevamente\n\n");
        }
        system("cls");
        (*e).terminos[i].coeficiente = coeficiente;
        (*e).terminos[i].exponente   = exponente;
    }
}

void auxiliaFun(Buffer * b, Vector v, char * cad, char* R, char* G, char *B){
    int x, y;
    y = ((*b).filas / 2 - (*b).par) - (int)round(v.jhat);
    x = (int)round(v.ihat) + (*b).columnas / 2;
    if((x < 0)||(x >= (*b).columnas)||(y < 0)||(y >= (*b).filas)) return;
    asignarRGB((*b).cadenas[y][x], R, G, B);
    asignarCar((*b).cadenas[y][x], cad);
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
        aplicarTra(&(*t).cortes, &dummy);
        auxiliaFun(b, dummy,"OO", "081", "217", "133");
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
        aplicarTra(&(*t).cortes, &v);
        auxiliaFun(b, v,"OO", "081", "217", "133");
    }
}


void auxiliaBuffer(Buffer * b, Vector v, Transformaciones * t, char * cad, char * R, char * G, char * B){
    Vector dummy = v;

    aplicarTra(&(*t).expansiones, &dummy);
    aplicarTra(&(*t).cortes, &dummy);

    auxiliaFun(b, dummy, cad, R, G, B);
}

void rellLinTransf(Buffer * b, Vector ini, Vector fin, Transformaciones * t, char * cad, char * R, char * G, char * B){
    int i, pasos;
    double factor, dx, dy;
    Vector inter;

    aplicarTra(&(*t).expansiones, &ini);
    aplicarTra(&(*t).cortes, &ini);

    aplicarTra(&(*t).expansiones, &fin);
    aplicarTra(&(*t).cortes, &fin);

    dx = fin.ihat - ini.ihat;
    dy = fin.jhat - ini.jhat;
    pasos = (int)ceil(fmax(fabs(dx), fabs(dy)));

    auxiliaFun(b, ini, cad, R, G, B);
    auxiliaFun(b, fin, cad, R, G, B);

    if(pasos <= 1) return;

    for(i = 1; i < pasos; i++){
        factor = (double)i / pasos;
        inter.ihat = ini.ihat + dx * factor;
        inter.jhat = ini.jhat + dy * factor;
        auxiliaFun(b, inter, cad, R, G, B);
    }
}

void rellenaBuffer(Buffer * b, Transformaciones * t){
    int i;
    int mitadX = (*b).columnas / 2;
    int mitadY = (*b).filas / 2 - (*b).par;
    int espacio = 5;
    Vector ini, fin;

    ini.jhat = (double)(-mitadY);
    fin.jhat = -ini.jhat;
    for(i = espacio; i <= mitadX; i += espacio){
        ini.ihat = (double)i;
        fin.ihat = ini.ihat;
        rellLinTransf(b, ini, fin, t, " .", "087", "087", "087");

        ini.ihat *= -1;
        fin.ihat  = ini.ihat;
        rellLinTransf(b, ini, fin, t, " .", "087", "087", "087");
    }

    ini.ihat = (double)(-mitadX);
    fin.ihat = -ini.ihat;
    for(i = espacio; i <= mitadY; i += espacio){
        ini.jhat = (double)i;
        fin.jhat = ini.jhat;
        rellLinTransf(b, ini, fin, t, " .", "087", "087", "087");

        ini.jhat *= -1;
        fin.jhat = ini.jhat;
        rellLinTransf(b, ini, fin, t, " .", "087", "087", "087");
    }

    ini.ihat = 0;
    ini.jhat = (double)(-mitadY);
    fin.ihat = 0;
    fin.jhat = -ini.jhat;
    rellLinTransf(b, ini, fin, t, "||", "255", "255", "255");

    ini.ihat = (double)(-mitadX);
    ini.jhat = 0;
    fin.ihat = -ini.ihat;
    fin.jhat = 0;
    rellLinTransf(b, ini, fin, t, "==", "255", "255", "255");
}

void limpiarBuffer(Buffer * b){
    int i, j;
    for(i = 0; i < (*b).filas; i++){
        for(j = 0; j < (*b).columnas; j++){
            asignarRGB((*b).cadenas[i][j], "255", "255", "255");
            asignarCar((*b).cadenas[i][j], "  ");
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
}

void imprimeBuffer(Buffer * b){
    int i, j;
    for(i = 0; i < (*b).filas; i++){
        for(j = 0; j < (*b).columnas; j++) printf("%s", (*b).cadenas[i][j]);
        printf("\n");
    }
}


void liberarExpresion(Expresion * e){
    if((*e).terminos != NULL){
        free((*e).terminos);
        (*e).terminos = NULL;
    }
    (*e).longitud = 0;
}

void liberarBuffer(Buffer * b){
    int i, j;
    if((*b).cadenas != NULL){
        for(i = 0; i < (*b).filas; i++){
            if((*b).cadenas[i] != NULL){
                for(j = 0; j < (*b).columnas; j++){
                    free((*b).cadenas[i][j]);
                }
                free((*b).cadenas[i]);
            }
        }
        free((*b).cadenas);
        (*b).cadenas = NULL;
    }
    (*b).filas = 0;
    (*b).columnas = 0;
    (*b).par = 0;
}

void guardarImpresion(Buffer * b, Matriz * m, Expresion * e, Transformaciones * t){
    int i, j;
    char nombre[20];
    char ruta[25];
    ruta[24] = '\0';
    ruta[23] = 't';
    ruta[22] = 'x';
    ruta[21] = 't';
    ruta[20] = '.';
    while(1){
        printf("Por favor, ingrese el nombre en el que quiera guardar su impresion, solo se admiten letras y numeros\n");
        printf("Nombre : ");
        fgets(nombre, 20, stdin);
        if(!limpiarBufferStr(nombre)) continue;
        if(verificarStr(nombre)) break;
        system("cls");
        printf("El nombre dado del archivo no es valido, por favor, intentelo nuevamente\n\n");
    }

    for(i = 0; i < 20; i++){
        if(nombre[i]!='\0'){
            ruta[i] = nombre[i];
            continue;
        } 
        ruta[i] = '.';
        ruta[i+1] = 't';
        ruta[i+2] = 'x';
        ruta[i+3] = 't';
        ruta[i+4] = '\0';
        break;
    }

    FILE * salida = fopen(ruta, "w");
    if(salida == NULL){
        printf("\nParece que hubo un error al abrir el archivo, se terminara al programa");
        return;
    }
    fprintf(salida, "La funcion graficada es y = ");
    for(i = 0; i < (*e).longitud; i++){
        fprintf(salida, "%.2lfx^%.2lf", (*e).terminos[i].coeficiente, (*e).terminos[i].exponente);
        if(i<(*e).longitud-1) fprintf(salida, " + ");
    }

    fprintf(salida, "\n\n");

    for(i = 0; i < (*b).filas; i ++){
        for(j = 0; j < (*b).columnas; j++) fprintf(salida, "%c%c", (*b).cadenas[i][j][19], (*b).cadenas[i][j][20]);
        fprintf(salida,"\n");
    }

    if(((*m).v1.ihat == 1) && ((*m).v1.jhat == 0) && ((*m).v2.ihat == 0) && ((*m).v2.jhat == 1)){
        fprintf(salida, "\nComo la matriz dada fue la base canonica, no se sufrio ninguna transformacion");
        fclose(salida);
        return;
    }

    fprintf(salida, "Asi es como se ve la funcion al verse afectada por transformacion \n\n");
    fprintf(salida, "%.2lf    %.2lf\n", (*m).v1.ihat, (*m).v2.ihat);
    fprintf(salida, "%.2lf    %.2lf\n\n", (*m).v1.jhat, (*m).v2.jhat);
    fprintf(salida, "La funcion en la base canonica se ve de la siguiente manera\n\n");

    limpiarBuffer(b);
    Transformaciones dummy;
    Vector v = {1, 0};
    dummy.cortes.v1 = v;
    dummy.expansiones.v1 = v;
    v.ihat = 0;
    v.jhat = 1;
    dummy.cortes.v2 = v;
    dummy.expansiones.v2 = v;
    
    rellenaBuffer(b, &dummy);
    dibujarFun(e, b, &dummy);
    
    for(i = 0; i < (*b).filas; i ++){
        for(j = 0; j < (*b).columnas; j++) fprintf(salida, "%c%c", (*b).cadenas[i][j][19], (*b).cadenas[i][j][20]);
        fprintf(salida,"\n");
    }

    fclose(salida);
}

void guardarBinario(Buffer * b, Matriz * m, Expresion * e, Transformaciones * t){
    int i, j;
    char nombre[20];
    char ruta[25];

    while(1){
        printf("Por favor, ingrese el nombre para guardar el archivo binario (solo letras y numeros)\n");
        printf("Nombre : ");
        fgets(nombre, 20, stdin);
        if(!limpiarBufferStr(nombre)) continue;
        if(verificarStr(nombre)) break;
        system("cls");
        printf("El nombre dado del archivo no es valido, por favor, intentelo nuevamente\n\n");
    }

    for(i = 0; i < 20; i++){
        if(nombre[i] != '\0'){
            ruta[i] = nombre[i];
            continue;
        }
        ruta[i]   = '.';
        ruta[i+1] = 'b';
        ruta[i+2] = 'i';
        ruta[i+3] = 'n';
        ruta[i+4] = '\0';
        break;
    }

    FILE * salida = fopen(ruta, "wb");
    if(salida == NULL){
        printf("\nParece que hubo un error al abrir el archivo binario\n");
        return;
    }

    fwrite(m, sizeof(Matriz), 1, salida);
    fwrite(t, sizeof(Transformaciones), 1, salida);

    fwrite(&(*e).longitud, sizeof(int), 1, salida);
    fwrite((*e).terminos, sizeof(Numero), (*e).longitud, salida);

    fwrite(&(*b).filas, sizeof(int), 1, salida);
    fwrite(&(*b).columnas, sizeof(int), 1, salida);
    fwrite(&(*b).par, sizeof(int), 1, salida);
    for(i = 0; i < (*b).filas; i++){
        for(j = 0; j < (*b).columnas; j++){
            fwrite((*b).cadenas[i][j], sizeof(char), 26, salida);
        }
    }

    fclose(salida);
}

int cargarBinario(Buffer * b, Matriz * m, Expresion * e, Transformaciones * t){
    int i, j;
    int nuevaLongitud, nuevasFilas, nuevasColumnas, nuevoPar;
    char nombre[20];
    char ruta[25];

    while(1){
        printf("Por favor, ingrese el nombre del archivo binario a cargar (sin extension)\n");
        printf("Nombre : ");
        fgets(nombre, 20, stdin);
        if(!limpiarBufferStr(nombre)) continue;
        if(verificarStr(nombre)) break;
        system("cls");
        printf("El nombre dado del archivo no es valido, por favor, intentelo nuevamente\n\n");
    }

    for(i = 0; i < 20; i++){
        if(nombre[i] != '\0'){
            ruta[i] = nombre[i];
            continue;
        }
        ruta[i]   = '.';
        ruta[i+1] = 'b';
        ruta[i+2] = 'i';
        ruta[i+3] = 'n';
        ruta[i+4] = '\0';
        break;
    }

    FILE * entrada = fopen(ruta, "rb");
    if(entrada == NULL){
        printf("\nNo se pudo abrir el archivo binario\n");
        return 0;
    }

    if(fread(m, sizeof(Matriz), 1, entrada) != 1 ||
       fread(t, sizeof(Transformaciones), 1, entrada) != 1 ||
       fread(&nuevaLongitud, sizeof(int), 1, entrada) != 1 ||
       nuevaLongitud <= 0){
        printf("\nError al leer los datos base del archivo binario\n");
        fclose(entrada);
        return 0;
    }

    Numero * nuevosTerminos = calloc(nuevaLongitud, sizeof(Numero));
    if(nuevosTerminos == NULL ||
       fread(nuevosTerminos, sizeof(Numero), nuevaLongitud, entrada) != (size_t)nuevaLongitud){
        printf("\nError al leer los terminos de la expresion\n");
        free(nuevosTerminos);
        fclose(entrada);
        return 0;
    }

    if(fread(&nuevasFilas, sizeof(int), 1, entrada) != 1 ||
       fread(&nuevasColumnas, sizeof(int), 1, entrada) != 1 ||
       fread(&nuevoPar, sizeof(int), 1, entrada) != 1 ||
       nuevasFilas <= 0 || nuevasColumnas <= 0){
        printf("\nError al leer las dimensiones del buffer\n");
        free(nuevosTerminos);
        fclose(entrada);
        return 0;
    }

    char *** nuevasCadenas = calloc(nuevasFilas, sizeof(char**));
    if(nuevasCadenas == NULL){
        free(nuevosTerminos);
        fclose(entrada);
        return 0;
    }

    for(i = 0; i < nuevasFilas; i++){
        nuevasCadenas[i] = calloc(nuevasColumnas, sizeof(char*));
        if(nuevasCadenas[i] == NULL){
            for(int k = 0; k < i; k++){
                for(j = 0; j < nuevasColumnas; j++) free(nuevasCadenas[k][j]);
                free(nuevasCadenas[k]);
            }
            free(nuevasCadenas);
            free(nuevosTerminos);
            fclose(entrada);
            return 0;
        }
        for(j = 0; j < nuevasColumnas; j++){
            nuevasCadenas[i][j] = calloc(26, sizeof(char));
            if(nuevasCadenas[i][j] == NULL ||
               fread(nuevasCadenas[i][j], sizeof(char), 26, entrada) != 26){
                for(int colPrev = 0; colPrev <= j; colPrev++) free(nuevasCadenas[i][colPrev]);
                free(nuevasCadenas[i]);
                for(int k = 0; k < i; k++){
                    for(int c = 0; c < nuevasColumnas; c++) free(nuevasCadenas[k][c]);
                    free(nuevasCadenas[k]);
                }
                free(nuevasCadenas);
                free(nuevosTerminos);
                fclose(entrada);
                return 0;
            }
        }
    }

    fclose(entrada);

    // Liberar recursos anteriores solo cuando todo se leyó correctamente
    liberarExpresion(e);
    liberarBuffer(b);

    (*e).longitud = nuevaLongitud;
    (*e).terminos = nuevosTerminos;

    (*b).filas = nuevasFilas;
    (*b).columnas = nuevasColumnas;
    (*b).par = nuevoPar;
    (*b).cadenas = nuevasCadenas;

    return 1;
}


void animar(Buffer * b, Transformaciones * t, Expresion * e){
    double i;
    Transformaciones dummy={{{1,0},{0,1}},{{1,0},{0,1}}};
    limpiarBuffer(b);
    rellenaBuffer(b, &dummy);
    dibujarFun(e, b, &dummy);
    imprimeBuffer(b);
    if((((*t).cortes.v1.ihat==1)&&((*t).cortes.v2.ihat==0)&&
        ((*t).cortes.v1.jhat==0)&&((*t).cortes.v2.jhat==1))&&
       (((*t).expansiones.v1.ihat==1)&&((*t).expansiones.v2.ihat==0)&&
        ((*t).expansiones.v1.jhat==0)&&((*t).expansiones.v2.jhat==1))){
        printf("No se ha declarado ninguna transformacion lineal\n");
        printf("Ingrese un numero para regresar al menu : ");
        scanf("%lf", &i);
        limpiarBufferNum();
        system("cls");
        return;
    }
    Transformaciones cambios = {
        {{((*t).expansiones.v1.ihat-1)/20,(*t).expansiones.v1.jhat/20},
          {(*t).expansiones.v2.ihat/20,((*t).expansiones.v2.jhat-1)/20}}, 
        {{((*t).cortes.v1.ihat-1)/20,(*t).cortes.v1.jhat/20},
          {(*t).cortes.v2.ihat/20,((*t).cortes.v2.jhat-1)/20}}};
    
    Sleep(75);
    printf("\033[H");
    for(i = 0.0; i <= 19.0; i+=1.0){
        dummy.cortes.v1.ihat +=  cambios.cortes.v1.ihat; 
        dummy.cortes.v1.jhat +=  cambios.cortes.v1.jhat; 
        dummy.cortes.v2.ihat +=  cambios.cortes.v2.ihat; 
        dummy.cortes.v2.jhat +=  cambios.cortes.v2.jhat; 
        dummy.expansiones.v1.ihat +=  cambios.expansiones.v1.ihat; 
        dummy.expansiones.v1.jhat +=  cambios.expansiones.v1.jhat; 
        dummy.expansiones.v2.ihat +=  cambios.expansiones.v2.ihat; 
        dummy.expansiones.v2.jhat +=  cambios.expansiones.v2.jhat; 
        rellenaBuffer(b, &dummy);
        dibujarFun(e, b, &dummy);
        imprimeBuffer(b);
        limpiarBuffer(b);
        Sleep(75);
        if(i != 19.0)printf("\033[H");
    }
    printf("Ingrese un numero para regresar al menu : ");
    scanf("%lf", &i);
    limpiarBufferNum();
    system("cls");
}

void menu(){
    Transformaciones t = {{{1, 0}, {0, 1}}, {{1, 0}, {0, 1}}};
    Expresion e = {NULL, 0};
    Matriz m = {{1, 0}, {0, 1}};
    Buffer b;
    int resp, fun = 0;
    iniciarBuffer(&b, 53, 53);
    while(1){
        printf("Por favor, seleccione una de las siguientes opciones :\n");
        printf("1.- Asignar una funcion para graficar\n");
        printf("2.- Mostrar transformacion\n");
        printf("3.- Asignar una transformacion lineal\n");
        printf("4.- Cargar una transformacion lineal\n");
        printf("5.- Guardar la impresion\n");
        printf("6.- Guardar la transformacion\n");
        printf("7.- Salir\n");
        printf("Respuesta : ");
        scanf("%d", &resp);
        if(!limpiarBufferNum())continue;
        if((resp < 1) || (resp > 7)){
            printf("La respuesta dada esta fuera de los rangos permitidos, por favor, intentelo nuevamente\n\n");
            continue;
        }
        system("cls");
        switch(resp){
            case 1:
                iniciarFun(&e);
                fun = 1;
                break;
            case 2:
                if(!fun){
                    printf("Solo puedes hacer esto despues de asignar una funcion\n\n");
                    continue;
                }
                animar(&b, &t, &e);
                break;
            case 3:
                rellenaMat(&m,&t);
                break;
            case 4:
                if(cargarBinario(&b, &m, &e, &t)) fun = 1;
                else fun = 0;
                break;
            case 5:
                if(!fun){
                    printf("Solo puedes hacer esto despues de asignar una funcion\n\n");
                    continue;
                }
                limpiarBuffer(&b);
                rellenaBuffer(&b, &t);
                dibujarFun(&e, &b, &t);
                guardarImpresion(&b, &m, &e, &t);
                break;
            case 6:
                if(!fun){
                    printf("Solo puedes hacer esto despues de asignar una funcion\n\n");
                    continue;
                }
                limpiarBuffer(&b);
                rellenaBuffer(&b, &t);
                dibujarFun(&e, &b, &t);
                guardarBinario(&b, &m, &e, &t);
                break;
            case 7:
                return;
        }
    }
}

int main(){
    HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo;
    GetConsoleMode(consola, &modo);
    SetConsoleMode(consola, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    
    menu();
    
    return 0;
}