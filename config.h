#ifndef CONFIG_H_
#define CONFIG_H_

#include "raylib.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// #define SCREEN_WIDTH 1000
// #define SCREEN_HEIGHT 560

#define FPS 60

extern int mouse_sens;

extern int game_paused;
extern int volume_paused;

extern float music_volume;
extern float pause_volume;
extern float temp_volume;

extern Music game_music;
// extern Music pause_music; // dont need this

extern Texture2D pause_bg;

void InitGameState();

void InitMusic();

void InitTextures();

void PauseGame();
#endif
