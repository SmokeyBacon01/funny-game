// gcc -Iheaders src\main.c src\entity.c -o build\game.exe -lraylib -lopengl32 -lgdi32 -lwinmm
 
 
// TODO
// Add collision logic
    // - hurtboxes
    // - try messing with active/inactive hurtboxes
 
#include "raylib.h"
#include "raymath.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
 
#include "header.h"
#include "world.h"
#include "entity.h"
 
int main(int argc, char *argv[]) {
 
    if (argc != 1) {
        InitWindow(800, 500, "debug");  
    } else {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
        InitWindow(0, 0, "raylib");
    }
 
    HideCursor();
 
    // Initialisations
 
    game_world *world = malloc(sizeof(game_world));
    initialise_world(world);
 
    time time;
    initialise_time(&time);

    initialise_hurtboxes(&world->objects);
    // initialise_hitboxes(&world->objects);
 
    SetTargetFPS(FPS);  
    
    create_test_hurtboxes(&world->objects);
    create_test_hitboxes(&world->objects);
 
    // Main gameplay loop.
    while (!WindowShouldClose()) {
        update_time(&time, &world->player);
 
        update_player(world, time);
        update_hurtboxes(&world->objects, time);
        update_hitboxes(&world->objects, time);
        world->camera.target = world->player.position;
 
        update_draw_frame(world, time);
    }
    CloseWindow();

    free(world);
    return 0;
}