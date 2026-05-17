#include "config.h"
#include "raylib.h"

#define VOLUME_STEP 0.1f

// ignore this
// void PauseGame(void) {
//     // not the best solution
//     // just to keep button functions together
//     // pause game is defined in interface.c
//     PauseGame();
// }

void MusicOnOff(void) {
    volume_paused = (volume_paused + 1) % 2;
    if (!volume_paused) {
        ResumeMusicStream(game_music);
    }
    else if (volume_paused) {
        PauseMusicStream(game_music);
    }
}

void DecMusicVolume() {
    if (temp_volume - VOLUME_STEP >= 0) {
        temp_volume -= VOLUME_STEP;
        pause_volume = temp_volume/2.0f;
        SetMusicVolume(game_music, pause_volume);
    }
}

void IncMusicVolume() {
    if (temp_volume + VOLUME_STEP < 1.0f) {
        temp_volume += VOLUME_STEP;
        pause_volume = temp_volume/2.0f;
        SetMusicVolume(game_music, pause_volume);
    }
}

void DecMouseSens() {
    // can't go lower than one
    mouse_sens = (mouse_sens > 1) ? mouse_sens - 1 : 1;
}

void IncMouseSens() {
    mouse_sens += 1;
}

// placeholder function for testing
void DoNothing() {}
