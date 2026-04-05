#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

#define CELL_SIZE 50

#define PLAYER_FOV 80

#define RENDER_DISTANCE 500

#define PLAYER_COLOR GetColor(0x00ff00FF)

#define MAP_HEIGHT 10
#define MAP_WIDTH 10

#define RAY_STEP PLAYER_FOV/SCREEN_WIDTH
#define RAY_STEP_SIZE 0.5

#define SCENE_HEIGHT MAP_HEIGHT*CELL_SIZE
#define SCENE_WIDTH MAP_WIDTH*CELL_SIZE

#define PLAYER_ROTATION_SPEED 1
#define PLAYER_MOVEMENT_SPEED 2

typedef struct {
    double x, y, angle;
}Player;

int map[MAP_HEIGHT][MAP_WIDTH]={
    {0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,1,0},
    {1,0,0,1,1,0,0,0,1,0},
    {1,0,0,0,1,0,0,0,1,0},
    {1,0,0,0,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,0,1,0},
    {1,1,1,1,1,0,0,1,1,0}
};

Player player = {0, 0, 45};

void DrawPlayer(Player player){ //DrawRectangle(player.x, player.y, 10, 10, PLAYER_COLOR);
    DrawText(TextFormat("x:%.2f y:%.2f",player.x,player.y),10,10,20,RED);
}

// returns the distance to next wall assuming the player looks in given direction
double GetDistance(Player player, double angle){
    //cast a ray
    bool wall_detected = false;
    double ray_x = player.x;
    double ray_y = player.y;
    double ray_distance=0;
    while(!wall_detected){
        double ray_distance=sqrt(pow(ray_x - player.x,2)+pow(ray_y-player.y,2)); // d^2 = x^2 + y^2
        if(ray_distance > RENDER_DISTANCE){
            return -1;
        }
        // which cell is the ray inside now
        int cell_x = ray_x / CELL_SIZE;
        int cell_y = ray_y / CELL_SIZE;

        if(cell_x > MAP_WIDTH || cell_x < 0 || cell_y >= MAP_HEIGHT || cell_y < 0)
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
    Rectangle vertical_rect = {x, SCREEN_HEIGHT/2.0 - height/2, 1, height};
    double y = SCREEN_HEIGHT/2.0 - height/2;
    if(vertical_rect.x > 250 && y > 250){
        DrawRectangleRec(vertical_rect, WHITE);
    } else {
        DrawRectangleRec(vertical_rect, BLUE);
    }
}
void DrawFOV(Player player){
    for(int x = 0; x < SCREEN_WIDTH; x++){
        // Calculate the exact angle for this pixel vertical line
        double angle = (player.angle - PLAYER_FOV/2.0) + ((float)x * RAY_STEP);

        // check distance for every angle in field
        double distance = GetDistance(player, angle*DEG2RAD);

        if (distance > 0) {

            double corrected_distance = distance * cos((angle - player.angle) * DEG2RAD);

            // based on distance, draw a line of proportional height
            // (HEIGHT * CELL_SIZE) / corrected_distance works well to scale correctly
            double visual_height = (SCREEN_HEIGHT * CELL_SIZE) / corrected_distance;

            DrawVerticalLine(visual_height, x);
        }
    }
}
void move(Player *p){

    double temp_angle = p->angle;
    double temp_x = p->x;
    double temp_y = p->y;

    if(IsKeyDown(KEY_A)){
        temp_angle -=PLAYER_ROTATION_SPEED;
    }
    if(IsKeyDown(KEY_D)){
        temp_angle +=PLAYER_ROTATION_SPEED;
    }
    if(IsKeyDown(KEY_W)){
        temp_x +=cos(player.angle*DEG2RAD)*PLAYER_MOVEMENT_SPEED;
        temp_y +=sin(player.angle*DEG2RAD)*PLAYER_MOVEMENT_SPEED;
    }
    if(IsKeyDown(KEY_S)){
        temp_x -=cos(player.angle*DEG2RAD)*PLAYER_MOVEMENT_SPEED;
        temp_y -=sin(player.angle*DEG2RAD)*PLAYER_MOVEMENT_SPEED;
    }
    int map_x = temp_x / CELL_SIZE;
    int map_y = temp_y / CELL_SIZE;


    if(map_x >= 0 && map_x < MAP_WIDTH &&
       map_y >= 0 && map_y < MAP_HEIGHT)
    {

        if(map[map_y][map_x] == 0) {
            p->x = temp_x;
            p->y = temp_y;
        }

    }

    p->angle = temp_angle;
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "not doom");
    SetTargetFPS(90);


    while (!WindowShouldClose())
    {
        move(&player);
        BeginDrawing();
            ClearBackground(BLACK);
            DrawPlayer(player);
            DrawFOV(player);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
