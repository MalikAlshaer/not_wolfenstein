#include "raylib.h"
//fonkisyon çalıştırma

void DrawMenu(int width, int height, Texture2D texture) {
    
    DrawTexture(texture, width/2 - texture.width/2, height/2 - texture.height/2, RAYWHITE);
    DrawText("GAME PAUSED",width/2 - 98,height/2 - 190,30,WHITE);
    //buraya yaz
}
