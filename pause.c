#include "raylib.h"

void DrawMenu(int width, int height, Texture2D texture) {
    DrawTexture(texture, width/2 - texture.width/2, height/2 - texture.height/2, RAYWHITE);
}
