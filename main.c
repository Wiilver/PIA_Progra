#include "raylib.h"

void moverPutosLinealmente(Vector2 * sujeto,Vector2 * objetivo ){
    Vector2 diferencia = {objetivo->x - sujeto->x, objetivo->y - sujeto->y};
    if(diferencia.x*diferencia.x>.5) sujeto->x+=(diferencia.x/100);
    if(diferencia.y*diferencia.y>.5) sujeto->y+=(diferencia.y/100);
}

int main(){
    InitWindow(500, 500, "hOLAMUNDO");
    SetTargetFPS(60);

    Vector2 v1   = {100,10};
    Vector2 f1   = {10,10};
    Vector2 v2  = {200,100};
    Vector2 f2   = {10,100};
    Vector2 v3  = {300,200};
    Vector2 f3   = {10,200};
    Vector2 v4  = {400,300};
    Vector2 f4   = {10,300};
    
    while(!WindowShouldClose()){
        moverPutosLinealmente(&v1, &f1);
        moverPutosLinealmente(&v2, &f2);
        moverPutosLinealmente(&v3, &f3);
        moverPutosLinealmente(&v4, &f4);
        BeginDrawing();
        DrawCircle(v4.x, v4.y, 5, BLACK);
        DrawCircle(v1.x, v1.y, 5, BLACK);
        DrawCircle(v2.x, v2.y, 5, BLACK);
        DrawCircle(v3.x, v3.y, 5, BLACK);
        ClearBackground(BLUE);
        EndDrawing();
    }
    return 0;
}