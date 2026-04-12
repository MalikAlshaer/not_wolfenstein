#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600

#define CELL_SIZE 50

#define PLAYER_FOV 80

#define RENDER_DISTANCE 500

#define MAP_HEIGHT 10
#define MAP_WIDTH 10

#define RAY_STEP PLAYER_FOV/WIDTH
#define RAY_STEP_SIZE 0.7

#define PLAYER_ROTATION_SPEED 1
#define PLAYER_MOVEMENT_SPEED 1
#define MOUSE_SPEED 0.1

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,1,1,1,1,1,1,0},
    {0,0,0,1,0,0,0,0,1,0},
    {1,0,0,1,1,0,1,0,1,0},
    {1,0,0,0,1,0,0,0,1,0},
    {1,0,0,0,1,0,0,0,1,0},
    {1,0,1,1,1,0,0,0,1,0},
    {1,0,1,0,0,0,0,0,1,0},
    {1,0,1,1,1,1,1,0,1,0},
    {1,0,0,0,0,0,0,0,1,0},
    {1,1,1,1,1,1,1,1,1,0}
};

typedef struct {
    double x, y, angle;
}Player;

Player player = {100, 100, 45};

int paused = 0;

void DrawHUD(){ //DrawRectangle(player.x, player.y, 10, 10, PLAYER_COLOR);
    //coords
    DrawText(TextFormat("x:%.2f y:%.2f", player.x, player.y), 10, 10, 20, BLUE);

    // crosshair
    DrawRectangle(WIDTH/2, HEIGHT/2 + 15, 4, 10, RED);//assagı
    DrawRectangle(WIDTH/2, HEIGHT/2 - 19, 4, 10, RED);// yukarı
    DrawRectangle(WIDTH/2 + 15, HEIGHT/2, 10, 4, RED);//sag
    DrawRectangle(WIDTH/2 - 20, HEIGHT/2, 10, 4, RED);//sol

}

// returns the distance to next wall assuming the player looks in given direction
double GetDistance(double angle) {
    bool wall_detected = false;

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

//draws a vertical line centered around the horizontal center axis of window
void DrawVerticalLine(double height, double x, double distance){
    Rectangle vertical_rect = {x, HEIGHT/2.0 - height/2.0, 1, height};

    int intensity = 255 - ((distance / RENDER_DISTANCE) * 255);
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;

   //you can change {red, green, blue, transparency}
    Color wallColor = {100, 100, 100, intensity};

    DrawRectangleRec(vertical_rect, wallColor);
}

void DrawFOV(){
    for(int x = 0; x < WIDTH; x++){
        // Calculate the exact angle for this pixel vertical line from left to right
        double ray_angle = ((player.angle - PLAYER_FOV/2.0) + ((float)x * RAY_STEP)) * DEG2RAD;

        // check distance for every angle in field
        double distance = GetDistance(ray_angle);

        if (distance > 0) {
            distance = distance * cos(ray_angle - player.angle * DEG2RAD);

            double visual_height = (HEIGHT * CELL_SIZE) / distance;
            DrawVerticalLine(visual_height, x, distance);
        }
    }
}

void move(){
    double temp_x = player.x;
    double temp_y = player.y;
    float angleX = 0, angleY = 0;

    Vector2 mouseDelta = GetMouseDelta();
    angleX -= mouseDelta.y * MOUSE_SPEED;
    angleY += mouseDelta.x * MOUSE_SPEED;
    //player.angle += cos(angleY) * 0.1;
    player.angle += sin(angleY * DEG2RAD) * 50;

    if(IsKeyDown(KEY_D)) {
        temp_y += cos(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
        temp_x -= sin(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
    }

    if(IsKeyDown(KEY_A)) {
        temp_y -= cos(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
        temp_x += sin(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
    }

    if(IsKeyDown(KEY_W)){
        temp_x += cos(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
        temp_y += sin(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
    }

    if(IsKeyDown(KEY_S)){
        temp_x -=cos(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
        temp_y -=sin(player.angle * DEG2RAD) * PLAYER_MOVEMENT_SPEED;
    }

    int map_x = temp_x / CELL_SIZE;
    int map_y = temp_y / CELL_SIZE;

    // if within map bounds
    if(map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT) {
        // if there is not wall
        if(map[map_y][map_x] == 0) {
            // commit changes
            player.x = temp_x;
            player.y = temp_y;
        }
    }
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, "not_wolfenstein.exe");
    SetTargetFPS(90);
    DisableCursor();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        move();
        DrawFOV();
        DrawHUD();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
