#include "raylib.h"
#include "raymath.h"

#include "header.h"
#include "world.h"
#include "entity.h"
#include "enemy.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void update_enemy(player *player, time time, game_world *world) {
    enemy_path(player, time, world);
}

void enemy_path(player *player, time time, game_world *world)
{
    Vector2 center = player->position;
    Vector2 target_position = (Vector2)
    {
    center.x + ENEMY_PLAYER_RADIUS*cos(time.total*ENEMY_SPEED),
    center.y + ENEMY_PLAYER_RADIUS*sin(time.total*ENEMY_SPEED)
    };
    double Player_Enemy_Distance =  Vector2Length(Vector2Subtract(world->player.position, world->enemy.position));
    world->enemy.distance = Player_Enemy_Distance;

    Vector2 Direction_Vector = Vector2Scale(normalise_vector(Vector2Subtract(target_position, world->enemy.position)), ENEMY_SPEED);
    if (Player_Enemy_Distance > ENEMY_PLAYER_RADIUS) 
    {
        world -> enemy.position = Vector2Add(
            world -> enemy.position, Direction_Vector
        );
    }
    else
    {
        Direction_Vector = Vector2Scale(
            Vector2Normalize(
                Vector2Subtract(
                    target_position, world->enemy.position
                )
            ), 
        ENEMY_SPEED);

        world -> enemy.position = Vector2Add(
            world -> enemy.position, Direction_Vector
        );
    }

}

// initialise_enemy_example(&world -> enemy);
// DrawCircleV(world -> enemy.position, PLAYER_RADIUS, RED);
// #define ENEMY_PLAYER_RADIUS 100
// #define ENEMY_SPEED 3

// typedef struct {
//     Vector2 speed;
//     Vector2 position;
//     int health;
//     double distance;
// } entity;