#include <stdlib.h>
#include <string.h> // for setting up the text in the buttons
#include "raylib.h"
#include "config.h"
#include "interface.h"
#include "buttons.h"

#define FONT_SIZE SCREEN_WIDTH/20
#define BUTTON_FONT_SIZE SCREEN_WIDTH/70
#define TEXT_PADDING SCREEN_WIDTH/192

#define DEFAULT_BUTTON_COLOR GREEN
#define HOVER_BUTTON_COLOR GRAY

Button *button_list = NULL;
int button_count = 0;

void DrawPauseMenu() {
    DrawRectangleRec((Rectangle){0, 0, SCREEN_WIDTH, SCREEN_WIDTH}, (Color){0, 0, 0, 90}); // Dim screen
    DrawText("GAME PAUSED", SCREEN_WIDTH/2 - MeasureText("GAME PAUSED", BUTTON_FONT_SIZE)/2, 3*SCREEN_HEIGHT/8, BUTTON_FONT_SIZE, WHITE); // pls don't change this :)
    DrawButtons();
    PressButton();
}

void DrawEndScreen() {
    if (game_state == Win) {
        DrawText("You Win", SCREEN_WIDTH/2 - MeasureText("You Win", FONT_SIZE), SCREEN_HEIGHT/2 + FONT_SIZE/2, FONT_SIZE, GREEN);
    }
    else if (game_state == Lose) {
        DrawText("Game Over", SCREEN_WIDTH/2 - MeasureText("Game Over", FONT_SIZE)/2, SCREEN_HEIGHT/2 - FONT_SIZE/2, FONT_SIZE, RED);
    }
}

void InitButton(int pos_x, int pos_y, int width, int height, char *text, Color color, Function function) {
    Button button = {0};

    // define width and height first -
    button.width = (width) ? width : MeasureText(text, BUTTON_FONT_SIZE) + TEXT_PADDING * 2; // if the width is not 0 use it otherwise use text length
    button.height = (height) ? height : BUTTON_FONT_SIZE + TEXT_PADDING * 2; // same as last line
    // - because you need to use them here
    button.pos_x = pos_x - button.width/2; // use center of button as base for location
    button.pos_y = pos_y - button.height/2; // (instead of the top right corner)
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
    InitButton(20 * SCREEN_WIDTH/40,    10 * SCREEN_HEIGHT/20,  0,  0, "RESUME",  DEFAULT_BUTTON_COLOR, PauseGame); // resume
    InitButton(20 * SCREEN_WIDTH/41,    12 * SCREEN_HEIGHT/20,  0,  0, "-",       DEFAULT_BUTTON_COLOR, DecMouseSens);
    InitButton(21 * SCREEN_WIDTH/41,    12 * SCREEN_HEIGHT/20,  0,  0, "+",       DEFAULT_BUTTON_COLOR, IncMouseSens);
    InitButton(20 * SCREEN_WIDTH/40,    14 * SCREEN_HEIGHT/20,  0,  0, "MUSIC",   DEFAULT_BUTTON_COLOR, MusicOnOff); // toggle music
    InitButton(18 * SCREEN_WIDTH/40,    14 * SCREEN_HEIGHT/20,  0,  0, "-",       DEFAULT_BUTTON_COLOR, DecMusicVolume); // dec music
    InitButton(22 * SCREEN_WIDTH/40,    14 * SCREEN_HEIGHT/20,  0,  0, "+",       DEFAULT_BUTTON_COLOR, IncMusicVolume); // inc music
}

void DrawButtons() {
    for (int i = 0; i < button_count; i++) {
        DrawRectangleRec((Rectangle) {
                button_list[i].pos_x,
                button_list[i].pos_y,
                button_list[i].width,
                button_list[i].height},
                button_list[i].color);
        DrawText(button_list[i].text, button_list[i].pos_x + TEXT_PADDING, button_list[i].pos_y + TEXT_PADDING, BUTTON_FONT_SIZE, WHITE);
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
            button_list[j].color = HOVER_BUTTON_COLOR;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                button_list[j].function();
            }
        }

        else {
            button_list[j].color = DEFAULT_BUTTON_COLOR;
        }
    }
}

void UnloadButtons() {
    free(button_list);
}
