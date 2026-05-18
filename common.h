#ifndef COMMON_H_
#define COMMON_H_

#include "raylib.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

// #define SCREEN_WIDTH 1000
// #define SCREEN_HEIGHT 560

typedef enum {
    Play = 0,
    Pause,
    Win,
    Lose,
} GameState;

extern GameState game_state;

extern int mouse_sens;

// extern int game_paused;
extern int volume_paused;

extern float music_volume;
extern float pause_volume;
extern float temp_volume;

extern Music game_music;
// extern Music pause_music; // dont need this

extern Texture2D pause_bg;
extern Texture2D wall_texture;

void InitGameState();

void InitMusic();

void UnloadMusic();

void InitTextures();

void UnloadTextures();

void PauseGame();
#endif
