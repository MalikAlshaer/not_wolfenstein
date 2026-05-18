#ifndef PAUSE_H_
#define PAUSE_H_
#include "common.h"
#include "raylib.h"

// understand this
typedef void (*Function)(void);

typedef struct Button{
    int pos_x;          // x position of the center of the button
    int pos_y;          // y position of the center of the button
    int width;          // width of the button
    int height;         // height of the button
    char *text;         // text that goes inside the button
    Color color;        // color of the button
    Function function;  // what the button does once pressed
} Button;

void DrawPauseMenu();

void DrawEndScreen();

void InitButton(int pos_x, int pos_y, int width, int height, char *text, Color color, Function function);

void DefineButtons();

void DrawButtons();

void PressButton();

void UnloadButtons();

#endif
