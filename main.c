#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "config.h"
#include "interface.h"

#define CELL_SIZE 50

#define PLAYER_FOV 80

#define RENDER_DISTANCE 500

#define MAP_HEIGHT 10
#define MAP_WIDTH 12

#define RAY_STEP PLAYER_FOV/SCREEN_WIDTH
#define RAY_STEP_SIZE 0.7

#define PLAYER_MOVEMENT_SPEED 1

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,1,0,1,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,1},
    {1,0,0,0,1,0,0,0,1,1,0,1},
    {1,0,1,1,1,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,0,0,1,0,0,1},
    {1,0,1,1,1,1,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
};

typedef struct Player {
    double x;
    double y;
    double angle;
    int hp;
} Player;

//               x    y    deg hp
Player player = {100, 100, 45, 120};

// returns the distance to next wall assuming the player looks in given direction
double GetDistance(double angle) {
    int wall_detected = 0;

    double ray_x = player.x;
    double ray_y = player.y;
    double ray_distance=0;

    //cast a ray
    while(!wall_detected){
        ray_distance += RAY_STEP_SIZE;

        if(ray_distance > RENDER_DISTANCE){
            return -1;
        }

        // which cell is the ray inside now
        int cell_x = ray_x / CELL_SIZE;
        int cell_y = ray_y / CELL_SIZE;

        if(cell_x < 0 || cell_x >= MAP_WIDTH || cell_y < 0 || cell_y >= MAP_HEIGHT)
            return -1;

        // is the ray inside the wall
        if(map[cell_y][cell_x]== 1){
            wall_detected=1;
            return ray_distance;

        } else {
            ray_x += cos(angle)*RAY_STEP_SIZE;
            ray_y += sin(angle)*RAY_STEP_SIZE;
        }
    }

    printf("WHAT THE HELL HAPPENED?\n");
    //exit(-1);
    return -1;
}

void DrawHud(){
    // fps
    DrawFPS(SCREEN_WIDTH - 90, 15);

    // hp
    DrawText(TextFormat("HP"),10,35,20,GREEN);
    DrawRectangle(45, 36, player.hp, 15, GREEN); // current_hp=120 initial

    // DrawText("to pause press E",SCREEN_WIDTH/2-87,20,20,WHITE);
    // DrawText("to sprint keep ctrl",SCREEN_WIDTH/2-95,49,20,WHITE);

    //coords
    DrawText(TextFormat("x:%.2f y:%.2f", player.x, player.y), 10, 10, 20, BLUE);



    // crosshair
    int crosshair_length = 10, crosshair_width = 3, crosshair_separation = 10;
    // top
    DrawRectangle(
            SCREEN_WIDTH/2 - crosshair_width/2,
            SCREEN_HEIGHT/2 - crosshair_length - crosshair_separation,
            crosshair_width,
            crosshair_length,
            WHITE);

    // bottom
    DrawRectangle(
            SCREEN_WIDTH/2 - crosshair_width/2,
            SCREEN_HEIGHT/2 + crosshair_separation,
            crosshair_width,
            crosshair_length,
            WHITE);

    // right
    DrawRectangle(
            SCREEN_WIDTH/2 + crosshair_separation,
            SCREEN_HEIGHT/2 - crosshair_width/2,
            crosshair_length,
            crosshair_width,
            WHITE);

    // left
    DrawRectangle(
            SCREEN_WIDTH/2 - crosshair_length - crosshair_separation,
            SCREEN_HEIGHT/2 - crosshair_width/2,
            crosshair_length,
            crosshair_width,
            WHITE);

    //minimap
    float mini_point_x = player.x/50 * 12 + SCREEN_WIDTH - 152;
    float mini_point_y = player.y/50 * 11;
    DrawRectangle(SCREEN_WIDTH - 140,50,130,95,WHITE); //map background
    DrawRectangle(mini_point_x,mini_point_y + 38,10,10,BLUE);
    DrawText("mini map",SCREEN_WIDTH - 122,145,25,WHITE);
}

//draws a vertical line centered around the horizontal center axis of window
void DrawVerticalLine(double height, double x, double distance){
    Rectangle vertical_rect = {x, SCREEN_HEIGHT/2.0 - height/2.0, 1, height};

    int intensity = 255 - ((distance / RENDER_DISTANCE) * 255);
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;

   //you can change {red, green, blue, transparency}
    Color wallColor = {0, 255, 0, intensity};

    DrawRectangleRec(vertical_rect, wallColor);
}

void DrawFOV(){
    double ray_angle, distance, visual_height;
    for(int x = 0; x < SCREEN_WIDTH; x++){
        // Calculate the exact angle for this pixel vertical line from left to right
        ray_angle = ((player.angle - PLAYER_FOV/2.0) + ((float)x * RAY_STEP)) * DEG2RAD;

        // check distance for every angle in field
        distance = GetDistance(ray_angle);

        if (distance > 0) {
            distance = distance * cos(ray_angle - player.angle * DEG2RAD);

            // depth of field basically
            visual_height = (SCREEN_HEIGHT * CELL_SIZE) / distance;
            DrawVerticalLine(visual_height, x, distance);
        }
    }
}

void move(){
    // change player location
    double temp_x = player.x;
    double temp_y = player.y;
    // change camera angle
    float angleX = 0, angleY = 0;

    Vector2 mouseDelta = GetMouseDelta();
    angleX -= mouseDelta.y;
    angleY += mouseDelta.x;
    // player.angle += cos(angleY) * 0.1;
    player.angle += sin(angleY * DEG2RAD) * mouse_sens;

    // trig functions are now variables instead of typing them four times each
    double c = cos(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
    double s = sin(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;

    if(IsKeyDown(KEY_D)) {
        temp_y += c;
        temp_x -= s;
    }

    if(IsKeyDown(KEY_A)) {
        temp_y -= c;
        temp_x += s;
    }

    if(IsKeyDown(KEY_W)){
        if(IsKeyDown(KEY_LEFT_CONTROL)){
            temp_x += c*1.8;
            temp_y += s*1.8;
        }else{
            temp_x += c;
            temp_y += s;
        }
    }

    if(IsKeyDown(KEY_S)){
        temp_x -= c;
        temp_y -= s;
    }

    // convert player coords to map coords
    int map_x = temp_x / CELL_SIZE;
    int map_y = temp_y / CELL_SIZE;

    // if within map bounds
    if(map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT) {
        // if there is not wall
        if(map[map_y][map_x] == 0) {
            // commit changes
            player.x = temp_x;
            player.y = temp_y;
            if(player.hp <= 120) player.hp += 0.2;
        } else {
            player.hp -= 0.4;
        }
    }
}

void ToggleMusic(int music_bool, Music *music){
    if (music_bool) { ResumeMusicStream(*music); }
    else { PauseMusicStream(*music); }
}

int main(void) {
    SetTargetFPS(FPS);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "not_wolfenstein.exe");
    DefineButtons(); // visual stuff from our interface library
    InitAudioDevice();
    DisableCursor();

    InitGameState(); // set up all variables to be used throughout files (eg. paused, volume, etc.)

    InitMusic();

    InitTextures();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        UpdateMusicStream(game_music);

        // pause game
        if (IsKeyPressed(KEY_E)) {
            PauseGame();
        }

        DrawFOV();

        if (!game_paused) {
            move();
            DrawHud();
        }

        else if (game_paused) {
            DrawPauseMenu();
            DrawButtons();
            PressButton();
        }

        EndDrawing();
    }

    FreeButtons();

    UnloadTexture(pause_bg);
    UnloadTexture(wall);

    // UnloadMusicStream(pause_music);
    UnloadMusicStream(game_music);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
