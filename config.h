#ifndef CONFIG_H_
#define CONFIG_H_

#include "raylib.h"

// #define SCREEN_WIDTH 1920
// #define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define FPS 60
#define FONT_SIZE 30

extern int game_paused;
extern int volume_paused;

extern float volume;
extern float pause_volume;
extern float temp_volume;

extern Music game_music;
extern Music pause_music;

extern Texture2D pause_bg;

void InitGameState();

void InitMusic();

void InitTextures();

void PauseGame();
#endif
