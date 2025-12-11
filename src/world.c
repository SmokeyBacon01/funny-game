#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "header.h"
#include "world.h"
#include "entity.h"

void draw_debug(time time, game_world *world) {
    int n = 10;
    DrawFPS(10, n);
    n += 40;
    DrawText(TextFormat("position: %lf, %lf", world->player.position.x, world->player.position.y), 10, n, 20, BLACK);
    n += 40;
    DrawText(TextFormat("speed: %lf", Vector2Length(world->player.speed) / 100), 10, n, 20, BLACK);
    n += 40;
    DrawText(TextFormat("acceleration: %lf", Vector2Length(world->player.acceleration) / 100), 10, n, 20, BLACK);
    n += 40;
    if (world->player.dash.is_dashing) {
        DrawText(TextFormat("Dashing: %.1lf", world->player.dash.duration), 10, n, 20, BLACK);
    } else if (world->player.dash.cooldown != 0) {
        DrawText(TextFormat("Cooldown: %.1lf", world->player.dash.cooldown), 10, n, 20, BLACK);
    }
    n += 40;
    DrawText(TextFormat("HP: %d", world->player.health), 10, n, 20, BLACK);
    n += 40;
    DrawText(TextFormat("Invincibility: %lf", world->player.invincible_duration), 10, n, 20, BLACK);
    n += 40;
    if (check_player_hurtbox_collision(world->objects, world->player)) { 
        DrawText(TextFormat("Collision!", world->player.health), 10, n, 20, BLACK);
    }
    n += 40;
}

// Updates delta time.
void update_time(time *time, player *player) {
    time->dt = GetFrameTime();
    time->total += time->dt;
}

// Draws what the map looks like
void draw_map(void) {
    ClearBackground(DARKGRAY);

    DrawRectangle(-MAP_SIZE, -MAP_SIZE, 2 * MAP_SIZE, 2 * MAP_SIZE, WHITE);

    for (int x = -MAP_SIZE; x <= MAP_SIZE; x += MAP_SIZE / 15) {
        DrawLine(x, -MAP_SIZE, x, MAP_SIZE, LIGHTGRAY);
    }

    for (int y = -MAP_SIZE; y <= MAP_SIZE; y += MAP_SIZE / 15) {
        DrawLine(-MAP_SIZE, y, MAP_SIZE, y, LIGHTGRAY);
    }
    


}

// Updates what the camera sees
void update_draw_frame(game_world *world, time time) {
    BeginDrawing();
        BeginMode2D(world->camera);
        draw_map();
        
        // PLAYER
        DrawCircleV(world->player.position, PLAYER_RADIUS, BLACK);

        //OBJECTS
        draw_hitboxes(&world->objects);
        draw_hurtboxes(&world->objects);

        EndMode2D();
        
        draw_debug(time, world);

    EndDrawing();
}

// Initialises values
void initialise_world(game_world *world) {
    world->screen.width = GetScreenWidth();
    world->screen.height = GetScreenHeight();

    world->camera.target = world->player.position;
    world->camera.offset = (Vector2){world->screen.width / 2, world->screen.height / 2};
    world->camera.zoom = DEFAULT_CAMERA_ZOOM;
    world->camera.rotation = 0.0f;

    initialise_player(&world->player);
}

void initialise_player(player *player) {
    player->health = 100;
    player->position = Vector2Zero();
    player->speed = Vector2Zero();
    player->acceleration = Vector2Zero();
    player->invincible_duration = 0;
    player->dash.is_dashing = false;
    player->dash.cooldown = 0;
    player->dash.duration = 0;
    player->dash.direction = Vector2Zero();
}

void initialise_time(time *time) {
    time->total = 0;
}

//////////////////////////////////////////////////////////////

void draw_hurtboxes(world_objects *objects) {
    for (int i = 0; i < objects->hurtbox_count; i++) {
        hurtbox hurtbox = objects->hurtboxes[i];
        if (!hurtbox.is_active) {
            continue;
        }

        if (hurtbox.shape == LINE) {
            DrawLineV(hurtbox.line.start, hurtbox.line.end, RED);
        } else if (hurtbox.shape == CIRCLE) {
            DrawCircleV(hurtbox.circle.centre, hurtbox.circle.radius, RED);
        } else if (hurtbox.shape == RECTANGLE) {
            DrawRectangleRec(hurtbox.rectangle, RED);
        }
    }    
}

void draw_hitboxes(world_objects *objects) {
    for (int i = 0; i < objects->hitbox_count; i++) {
        hitbox hitbox = objects->hitboxes[i];

        if (!hitbox.is_active) {
            break;
        }

        DrawCircleV(hitbox.circle.centre, hitbox.circle.radius, BLUE);
    }    
}

//////////////////////////////////////////////////////////////

void create_test_hurtboxes(world_objects *objects) {
    Vector2 start = {-100, -100};
    Vector2 end = {-100, -200};
    create_line_hurtbox(objects, start, end, 5);

    Vector2 centre = {100, 100};
    create_circle_hurtbox(objects, centre, 15, 10);

    Rectangle rec = {200, 200, 100, 100};
    create_rectangle_hurtbox(objects, rec, 15);
}

void create_test_hitboxes(world_objects *objects) {

    Vector2 centre = {500, 500};
    create_circle_hitbox(objects, centre, 30, 1);
    
    Vector2 centre2 = {300, 500};
    create_circle_hitbox(objects, centre2, 30, 1);
}