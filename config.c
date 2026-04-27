#include "raylib.h"
#include "config.h"

int mouse_sens;

int volume_paused;
int game_paused;

float music_volume;
float pause_volume;
float temp_volume;

Music game_music;
// Music pause_music;

Texture2D pause_bg;

void InitGameState() {
    mouse_sens = 5;
    game_paused = 0;
    volume_paused = 0;

    music_volume = 0.5f;
    pause_volume = 0.25f;
    temp_volume = 0.0f;
}

void InitMusic() {
    game_music =  LoadMusicStream("music/beppo.wav");
    // pause_music = LoadMusicStream("music/amongsus.wav");

    PlayMusicStream(game_music);
    UpdateMusicStream(game_music);
}

void InitTextures() {
    wall = LoadTextureFromImage(LoadImage("textures/wall.png"));

    pause_bg = LoadTextureFromImage(LoadImage("textures/space.png"));
    pause_bg.width = SCREEN_WIDTH;
    pause_bg.height = SCREEN_HEIGHT;
}

// this is the pause button's function as well as the E key
// so it is defined here instead of buttons.h
void PauseGame() {
    game_paused = (game_paused + 1) % 2;

    // leave these if statements here so they only execute once
    if (game_paused) {
        EnableCursor();
        // lower music_volume on pause
        temp_volume = music_volume;
        pause_volume = music_volume/2.0f;
        music_volume = pause_volume;
        SetMusicVolume(game_music, music_volume);
    }

    else if (!game_paused) {
        DisableCursor();
        // bring music_volume back to original state after unpause
        music_volume = temp_volume;
        SetMusicVolume(game_music, music_volume);
    }
}
