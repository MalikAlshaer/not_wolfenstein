#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600

#define CELL_SIZE 50
#define PLAYER_FOV 80
#define RENDER_DISTANCE 250

#define PLAYER_COLOR GetColor(0x00ff00FF)

#define MAP_HEIGHT 5
#define MAP_WIDTH 5

#define RAY_STEP_SIZE 0.05

#define SCENE_HEIGHT MAP_HEIGHT*CELL_SIZE
#define SCENE_WIDTH MAP_WIDTH*CELL_SIZE

//#define VERTICAL_SCALE HEIGHT*50
#define PLAYER_ROTATION_SPEED 1
#define PLAYER_MOVEMENT_SPEED 5

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0, 0, 0, 1, 0},
    {0, 0, 1, 1, 0},
    {1, 0, 0, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1}
};

typedef struct {
    double x, y, angle;
} Player;

Player player = {0, 0, 45};
void DrawPlayer(Player player){
    DrawRectangle(player.x, player.y, 10, 10, PLAYER_COLOR);
}
// returns the distance to next wall assuming the player looks in given direction
double GetDistance(Player player, double angle){
    //cast a ray
    bool wall_detected = false;

    double ray_x = player.x;
    double ray_y = player.y;
    double ray_distance=0;

    while(!wall_detected){
        // pythagorean for dist
        ray_distance=sqrt(pow(ray_x - player.x, 2) + pow(ray_y - player.y, 2));

        if(ray_distance > RENDER_DISTANCE){
            return -1;
        }

        // which cell is the ray inside now
        int cell_x = ray_x / CELL_SIZE;
        int cell_y = ray_y / CELL_SIZE;

        if(cell_x >= MAP_WIDTH || cell_y >= MAP_HEIGHT)
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
void DrawVerticalLine(double height, double x){
    Rectangle vertical_rect = {x, HEIGHT/2.0 - height/2,1,height};
    DrawRectangleRec(vertical_rect, WHITE);
}

void DrawFOV(Player player){
    // Determine how much angle per pixel on the screen width
    double ray_step = (double)PLAYER_FOV / WIDTH;

    for(int x = 0; x < WIDTH; x++){
        // Calculate the exact angle for this pixel vertical line
        double angle = (player.angle - PLAYER_FOV/2.0) + (x * ray_step);

        // check distance for every angle in field
        double distance = GetDistance(player, angle * DEG2RAD);

        if (distance > 0) {

            double corrected_distance = distance * cos((angle - player.angle) * DEG2RAD);

            // based on distance, draw a line of proportional height
            // (HEIGHT * CELL_SIZE) / corrected_distance works well to scale correctly
            double visual_height = (HEIGHT * CELL_SIZE) / corrected_distance;

            DrawVerticalLine(visual_height, x);
        }
    }
}

void move(){
    if(IsKeyDown(KEY_A)){
        player.angle -=PLAYER_ROTATION_SPEED;
    }

    if(IsKeyDown(KEY_D)){
        player.angle +=PLAYER_ROTATION_SPEED;
    }

    if(IsKeyDown(KEY_W)){
        player.x +=cos(player.angle*DEG2RAD)*PLAYER_MOVEMENT_SPEED;
        player.y +=sin(player.angle*DEG2RAD)*PLAYER_MOVEMENT_SPEED;
    }

    if(IsKeyDown(KEY_S)){
        player.x -=cos(player.angle*DEG2RAD)*PLAYER_MOVEMENT_SPEED;
        player.y -=sin(player.angle*DEG2RAD)*PLAYER_MOVEMENT_SPEED;
    }
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, "not_wolfenstein");
    SetTargetFPS(90);

    while (!WindowShouldClose()) {
        move();
        BeginDrawing();
            ClearBackground(BLACK);
            DrawPlayer(player);
            DrawFOV(player);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
