#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include "pause.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

#define CELL_SIZE 50

#define PLAYER_FOV 80

#define RENDER_DISTANCE 500

#define MAP_HEIGHT 10
#define MAP_WIDTH 12

#define RAY_STEP PLAYER_FOV/SCREEN_WIDTH
#define RAY_STEP_SIZE 0.7

#define PLAYER_ROTATION_SPEED 1
#define PLAYER_MOVEMENT_SPEED 1
#define MOUSE_SPEED 5


int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,1,0,1,0,1,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,1},
    {1,0,0,0,1,0,0,0,1,1,1,1},
    {1,0,1,1,1,0,0,0,1,0,1,1},
    {1,0,1,0,0,0,0,0,1,0,1,1},
    {1,0,1,1,1,1,1,0,0,0,1,1},
    {1,0,0,0,0,0,0,0,1,0,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
};
typedef struct Button{
    Rectangle rect;
    Color color;
}Button;
Button button_0 = {0};
void init_button(Button *button,Rectangle rect,Color color){
    button->rect = rect;
    button->color = color;
}
bool is_mouse_over_button(Button button){
    return CheckCollisionPointRec(GetMousePosition(),button.rect);
}
typedef struct {
    double x, y, angle;
}Player;

Player player = {100, 100, 45};

int paused = 0;
float current_hp=120;
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
void internal_interface(float current_hp){
    DrawText(TextFormat("HP"),10,35,20,GREEN);
    DrawRectangle(45,36,current_hp,15,GREEN); // current_hp=120
}
//draws a vertical line centered around the horizontal center axis of window
void DrawVerticalLine(double height, double x, double distance){
    Rectangle vertical_rect = {x, SCREEN_HEIGHT/2.0 - height/2.0, 1, height};

    int intensity = 255 - ((distance / RENDER_DISTANCE) * 255);
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;

   //you can change {red, green, blue, transparency}
    Color wallColor = {intensity, 1, 1, intensity};

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
    player.angle += sin(angleY * DEG2RAD) * MOUSE_SPEED;

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
        temp_x += c;
        temp_y += s;
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
            if(current_hp<=120) current_hp += 0.2;
        }else{
            current_hp -= 0.4;
        }
    }
    
}

void DrawHUD(){ //DrawRectangle(player.x, player.y, 10, 10, PLAYER_COLOR);
    //coords
    DrawText(TextFormat("x:%.2f y:%.2f", player.x, player.y), 10, 10, 20, BLUE);

    // crosshair
    DrawRectangle(SCREEN_WIDTH/2,       SCREEN_HEIGHT/2 + 15,   5,      10,     WHITE);//assagı
    DrawRectangle(SCREEN_WIDTH/2,       SCREEN_HEIGHT/2 - 20,   5,      10,     WHITE);// yukarı
    DrawRectangle(SCREEN_WIDTH/2 + 15,  SCREEN_HEIGHT/2,        10,     5,      WHITE);//sag
    DrawRectangle(SCREEN_WIDTH/2 - 20,  SCREEN_HEIGHT/2,        10,     5,      WHITE);//sol
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "not_wolfenstein.exe");
    InitAudioDevice();
    init_button(&button_0,(Rectangle){SCREEN_WIDTH/2 - 45,SCREEN_HEIGHT/2 - 250,90,40},RED);
    SetTargetFPS(90);
    DisableCursor();
    Sound sound_pause = LoadSound("amongsus.wav");
    Sound sound_game = LoadSound("beppo.wav");
    Texture2D pauseT = LoadTextureFromImage(LoadImage("textures/tester2.png"));
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        // pause game
        if (IsKeyPressed(KEY_E)) {
            paused = (paused + 1) % 2;
            if (!(paused % 2)) DisableCursor();
            if (paused % 2) EnableCursor();
        }
        
        if(is_mouse_over_button(button_0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            paused = (paused + 1) % 2;
            if (!(paused % 2)) DisableCursor();
            if (paused % 2) EnableCursor();
        }
        if (!paused) {
            
            move();
            DrawFOV();
            DrawHUD();
            internal_interface(current_hp);
            DrawFPS(SCREEN_WIDTH-90,15);
            DrawText("to pause press E",SCREEN_WIDTH/2-87,20,20,WHITE);
            PlaySound(sound_pause);
           
        }
        
        else if (paused) {
            DrawMenu(SCREEN_WIDTH, SCREEN_HEIGHT, pauseT);
            if(is_mouse_over_button(button_0)){
                button_0.color = BLUE;
            }else{
                button_0.color = RED;
            }
            DrawRectangleRec(button_0.rect,button_0.color);
            DrawText("Continue",button_0.rect.x + button_0.rect.width / 2 - MeasureText("Continue",20)/2,button_0.rect.y + button_0.rect.height / 2 -20/2,20,WHITE);          
            PlaySound(sound_game);
        }
        
        EndDrawing();
    }
    UnloadSound(sound_pause);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
