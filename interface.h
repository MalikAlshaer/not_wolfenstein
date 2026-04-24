#ifndef PAUSE_H_
#define PAUSE_H_
#include "config.h"
#include <raylib.h>

// volume variables
#define VOLUME_STEP 0.1

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

void DrawPauseMenu(Texture2D texture);

void init(Button *button, int pos_x, int pos_y, int width, int height, char *text, Color color, Function function);

bool MouseOverButton(Button button);

void InitButtons();

void DrawButtons();

// void InitButtons();

// extern Button resume;
// extern Button *button_list;
extern int button_count;

extern float volume;
extern float pause_volume;
extern float temp_volume;


void InitAudio();

#endif
