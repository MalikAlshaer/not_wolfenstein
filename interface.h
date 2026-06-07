#ifndef PAUSE_H_
#define PAUSE_H_
#include "common.h"
#include "raylib.h"

typedef struct Button{
    int pos_x;          // x position of the center of the button
    int pos_y;          // y position of the center of the button
    int width;          // width of the button
    int height;         // height of the button
    char *text;         // text that goes inside the button
    Color color;        // color of the button
    void (*function)(void);  // what the button does once pressed
} Button;

void DrawPauseMenu();

void DrawEndScreen();

void InitButton(int pos_x, int pos_y, int width, int height, char *text, Color color, void (*function)(void));

void DefineButtons();

void DrawButtons();

void PressButton();

void UnloadButtons();

#endif
