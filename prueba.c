#include <stdio.h>

int main(){
    char cadena [] = "\033[38;2;87;87;87mSalida de consola\033[0m";
    char cad [] = "\033[38;2;255;255;255mLL\033[0m";
    char cad [] = "\033[38;2;256;256;256m";
    char cad [] = "jej";
    char * cosa = "\033[38;2;255;255;255mLL\033[0m";
    printf("%s", cosa);
    return 0;
}