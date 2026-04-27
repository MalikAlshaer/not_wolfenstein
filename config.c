#include "raylib.h"
#include "config.h"


int volume_paused;
int game_paused;
float volume;
float pause_volume;
float temp_volume;

Music game_music;
Music pause_music;

Texture2D pause_bg;

void InitGameState() {
    game_paused = 0;
    volume_paused = 0;

    volume = 0.5f;
    pause_volume = 0.25f;
    temp_volume = 0.0f;
}

void InitMusic() {
    game_music =  LoadMusicStream("music/beppo.wav");
    pause_music = LoadMusicStream("music/amongsus.wav");

    PlayMusicStream(game_music);
    UpdateMusicStream(game_music);
}

void InitTextures() {
    pause_bg = LoadTextureFromImage(LoadImage("textures/space.png"));
}

void PauseGame() {
    game_paused = (game_paused + 1) % 2;

    // leave these if statements here so they only execute once
    if (game_paused) {
        EnableCursor();
        // lower volume on pause
        temp_volume = volume;
        pause_volume = volume/2.0f;
        volume = pause_volume;
        SetMusicVolume(game_music, volume);
    }

    else if (!game_paused) {
        DisableCursor();
        // bring volume back to original state after unpause
        volume = temp_volume;
        SetMusicVolume(game_music, volume);
    }
}
