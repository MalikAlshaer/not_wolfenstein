#include <math.h>
#include "raylib.h"
#include "config.h"
#include "interface.h"

#define FPS 60

#define CELL_SIZE 50
#define PLAYER_FOV 80

#define RENDER_DISTANCE 500

#define MAP_HEIGHT 13
#define MAP_WIDTH 13

#define RAY_STEP PLAYER_FOV/SCREEN_WIDTH
#define RAY_STEP_SIZE 0.7

#define PLAYER_MOVEMENT_SPEED 1

// take any coordinate from 0..MAP_WIDTH or 0..MAP_HEIGHT and
// return the value converted to cell coordinates
#define COORD2CELL(i) (i * CELL_SIZE + CELL_SIZE/2.0)
// take any cell coordinate from 0..MAP_HEIGHT*CELL_SIZE or 0..MAP_WIDTH*CELL_SIZE and
// return the value converted to map coordinates
#define CELL2COORD(i) (i / CELL_SIZE)

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,4,4,4,4,4,4,4,4,4,4,0,1},
    {1,0,1,1,0,1,0,1,1,1,4,1,1},
    {1,0,1,0,0,0,0,0,0,0,4,4,1},
    {1,0,1,1,0,1,0,1,1,1,1,4,1},
    {1,0,0,0,0,1,0,0,0,0,0,4,1},
    {1,0,1,1,1,1,0,1,0,1,1,4,1},
    {1,0,1,0,1,0,0,0,0,0,1,4,1},
    {1,0,1,0,1,0,0,0,0,0,1,4,1},
    {1,1,1,0,1,0,1,1,0,0,1,4,1},
    {1,0,0,0,0,0,0,0,0,0,1,4,1},
    {1,0,0,1,0,0,0,0,0,0,1,4,1},
    {1,1,1,1,1,1,1,1,1,1,1,4,1}
    //in order to represent the game i created a simple map
    //4 refers to truth way
};

typedef struct {
    // x and y might be reversed idk
    int x;
    int y;
} Tile;

const Tile start_tile = {.x = 1, .y = 1};
const Tile exit_tile = {.x = 11, .y = 12};

typedef struct Player {
    double x;       // x coordinate of the player
    double y;       // y coordinate of the player
    double angle;   // direction the player is facing
    int hp;         // health points
    float counter;  // time left for the player to exit
} Player;

// Player player = {start_tile.x * CELL_SIZE + CELL_SIZE/2.0, start_tile.y * CELL_SIZE + CELL_SIZE/2.0, 90, 120, 60};
Player player = {COORD2CELL(start_tile.x), COORD2CELL(start_tile.y), 90, 100, 60};

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
        int cell_x = CELL2COORD(ray_x);
        int cell_y = CELL2COORD(ray_y);

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

    return -1;
}

void DrawHud(){
    // fps
    DrawFPS(SCREEN_WIDTH - 90, 15);

    //coords
    DrawText(TextFormat("x: %.2f y: %.2f", player.x, player.y), 10, 10, 20, BLUE);

    // hp
    DrawText(TextFormat("HP"), 10 ,35, 20, GREEN);
    DrawRectangle(45, 35, player.hp, 15, GREEN); // current_hp=120 initial

    // time left
    DrawText(TextFormat("remaining: %.1f s", player.counter), 10, 60, 20, RED);

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
}

//draws a vertical line centered around the horizontal center axis of window
void DrawVerticalLine(double height, double x, double distance){
    int wall_slice = (int)x % wall_texture.width + player.angle;
    
     //Rectangle vertical_slice = {wall_slice, 0, 1, 800};
    Rectangle vertical_rect = {x, SCREEN_HEIGHT/2.0 - height/2.0, 1, height};

    int intensity = 255 - ((distance / RENDER_DISTANCE) * 255);
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;

   //you can change {red, green, blue, transparency}
    Color wall_color = {120, 140, 140, intensity};

    DrawRectangleRec(vertical_rect, wall_color);
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
        if(map[map_y][map_x] != 1) {
            // commit changes
            player.x = temp_x;
            player.y = temp_y;

            if(player.hp <= 120) player.hp += 0.2;
        } 

        else {
            player.hp -= 0.4;
        }
    }
}

void CheckEnd() { // rename to check_win() or something like that
    player.counter -= 0.02;
    if(
            player.x > COORD2CELL(exit_tile.x) - 25 && player.x < COORD2CELL(exit_tile.x) + 25 &&
            player.y > COORD2CELL(exit_tile.y) - 25 && player.y < COORD2CELL(exit_tile.y) + 25
      ) {
        game_state = Win;    
        EnableCursor();
    }
    if (player.counter <= 0 || player.hp <= 0) {
        game_state = Lose;
        EnableCursor();
    }
}

void RunGame() {
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
        DrawFOV();
        if(game_state == Play || game_state == Pause){
            if (IsKeyPressed(KEY_P)) {
                PauseGame();
            }


            if (game_state == Play) {
                CheckEnd();
                move();
                DrawHud();
            }

            else if (game_state == Pause) {
                DrawPauseMenu();
            }
        }

        else if(game_state == Win || game_state == Lose) {
            StopMusicStream(game_music);
            DrawEndScreen();
        }

        EndDrawing();
    }

    UnloadButtons();

    UnloadMusic();

    UnloadTextures();

    CloseAudioDevice();
    CloseWindow();
}

int main(void) {
    RunGame();
    return 0;
}
