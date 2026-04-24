#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "interface.h"
#include "config.h"

#define TEXT_PADDING 10

Button *button_list = NULL;
int button_count = 0;

void DrawPauseMenu (Texture2D pause_bg) {
    DrawTexture(pause_bg, SCREEN_WIDTH/2 - pause_bg.width/2, SCREEN_HEIGHT/2 - pause_bg.height/2, RAYWHITE);
    DrawText("GAME PAUSED", SCREEN_WIDTH/2 - 98, SCREEN_HEIGHT/2 - 190, 30, WHITE);
}

void init(Button *button, int pos_x, int pos_y, int width, int height, char *text, Color color, Function function) {
    button->pos_x = pos_x;
    button->pos_y = pos_y;
    // button->width = (width) ? width : strlen(text); // if the width is not 0 use it otherwise use text length
    button->width = (width) ? width : MeasureText(text, FONT_SIZE) + TEXT_PADDING * 2; // if the width is not 0 use it otherwise use text length
    button->height = height;
    button->text = strdup(text);
    button->color = color;
    button->function = function;

    Button *tmp = realloc(button_list, (button_count + 1) * sizeof(Button));
    if (tmp) {
        button_list = tmp;
        button_list[button_count] = *button;
        button_count++;
    }
}

int paused = 0;

void ResumePause(void) {
    paused = (paused + 1) % 2;
}

Button resume;
Button pause_music;
void InitButtons() {
    init(&resume, 100, 100, 0, 100, "message", RED, ResumePause);
}

void DrawButtons() {
    for (int i = 0; i < button_count; i++) {
        DrawRectangleRec((Rectangle) {
                button_list[i].pos_x,
                button_list[i].pos_y,
                button_list[i].width,
                button_list[i].height},
                button_list[i].color);
        DrawText(button_list[i].text, button_list[i].pos_x + TEXT_PADDING, button_list[i].pos_y, FONT_SIZE, WHITE);
    }
}

// Button volume_inc = {0}; // sesi arttırma
// Button volume_dec = {0}; // sesi azaltma
// Button sens = {0};
// Button sens_inc = {0};
// Button sens_dec = {0};

float volume;
float pause_volume;
float temp_volume;

void InitAudio() {
    volume = 0.5f;
    pause_volume = 0.25f;
    temp_volume = 0.0f;
}
