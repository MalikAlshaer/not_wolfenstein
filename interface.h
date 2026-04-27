#ifndef PAUSE_H_
#define PAUSE_H_
#include "config.h"
#include "raylib.h"

typedef void (*Function)(void);

typedef struct Button{
    int pos_x;
    int pos_y;
    int width;
    int height;
    char *text;
    Color color;
    Function function;
} Button;

void DrawPauseMenu();

void InitButton(int pos_x, int pos_y, int width, int height, char *text, Color color, Function function);

void DefineButtons();

void DrawButtons();

void PressButton();

void FreeButtons();

#endif
