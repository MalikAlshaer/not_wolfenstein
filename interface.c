#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "config.h"
#include "interface.h"
#include "buttons.h"

#define VOLUME_STEP 0.1
#define TEXT_PADDING 10

Button *button_list = NULL;
int button_count = 0;

void DrawPauseMenu() {
    DrawTexture(pause_bg, SCREEN_WIDTH/2 - pause_bg.width/2, SCREEN_HEIGHT/2 - pause_bg.height/2, RAYWHITE);
    DrawText("GAME PAUSED", SCREEN_WIDTH/2 - 98, SCREEN_HEIGHT/2 - 190, 30, WHITE);
}

void InitButton(int pos_x, int pos_y, int width, int height, char *text, Color color, Function function) {
    Button button = {0};

    // DEFINE WIDTH AND HEIGHT FIRST
    button.width = (width) ? width : MeasureText(text, FONT_SIZE) + TEXT_PADDING * 2; // if the width is not 0 use it otherwise use text length
    button.height = (height) ? height : FONT_SIZE + TEXT_PADDING * 2; // same as last line
    // BECUASE YOU NEED TO USE THEM HERE
    button.pos_x = pos_x - button.width/2;
    button.pos_y = pos_y - button.height/2;
    button.text = strdup(text);
    button.color = color;
    button.function = function;

    Button *tmp = realloc(button_list, (button_count + 1) * sizeof(Button));
    if (tmp) {
        button_list = tmp;
        button_list[button_count] = button;
        button_count++;
        return;
    }
}

void DefineButtons() {
    InitButton(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 0, "RESUME", RED, ResumePause); // resume
    InitButton(SCREEN_WIDTH/2, 3 * (SCREEN_HEIGHT/4), 0, 0, "MUSIC", RED, VolumeOnOff);
}

void DrawButtons() {
    for (int i = 0; i < button_count; i++) {
        DrawRectangleRec((Rectangle) {
                button_list[i].pos_x,
                button_list[i].pos_y,
                button_list[i].width,
                button_list[i].height},
                button_list[i].color);
        DrawText(button_list[i].text, button_list[i].pos_x + TEXT_PADDING, button_list[i].pos_y + TEXT_PADDING, FONT_SIZE, WHITE);
    }
}

void PressButton() {
    Vector2 mouse = GetMousePosition();
    for (int j = 0; j < button_count; j++) {
        if (
                (int)mouse.x > button_list[j].pos_x &&
                (int)mouse.x < button_list[j].width + button_list[j].pos_x &&
                (int)mouse.y > button_list[j].pos_y &&
                (int)mouse.y < button_list[j].pos_y + button_list[j].height)
        {
            button_list[j].color = BLUE;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                button_list[j].function();
            }
        }

        else {
            button_list[j].color = RED;
        }
    }
}

void FreeButtons() {
    free(button_list);
}
