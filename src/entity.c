#include "raylib.h"
#include "raymath.h"

#include "header.h"
#include "entity.h"
#include "world.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Updates player values
void update_player(game_world *world, time time) {
    dash_player(&world->player, time);
    move_player(&world->player, time, &world->objects);
    collide_player_world(&world->player);
    dash_zoom(&world->player, time, world);

    update_player_health(&world->objects, &world->player);
    update_player_invincibility(&world->player, time);
}

// Updates player's health
void update_player_health(world_objects *objects, player *player) {
    if (player->invincible_duration != 0) {
        return;
    }

    if (check_player_hurtbox_collision(*objects, *player)) {
        player->health--;
        player->invincible_duration = INVINCIBILITY_DURATION;
        return;
    }
}

void initialise_hitboxes(world_objects *objects) {
    objects->hitbox_count = 0;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        objects->hitboxes[i].is_active = false;
    }
}

void create_circle_hitbox(world_objects *objects, Vector2 centre, double radius, int health) {
    hitbox hitbox;
    hitbox.is_active = true;
    hitbox.is_solid = true;
    hitbox.circle.centre = centre;
    hitbox.circle.radius = radius;
    hitbox.health = health;
    objects->hitboxes[objects->hitbox_count++] = hitbox;
}

// NOTE: Parameters are uninitialised, only the active flag is initialised.
// Change this flag as necessary.
void initialise_hurtboxes(world_objects *objects) {
    objects->hurtbox_count = 0;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        objects->hurtboxes[i].is_active = false;
    }
}

void update_hitboxes(world_objects *objects, time time) {
    for (int i = 0; i < objects->hitbox_count; i++) {
        if (!objects->hitboxes[i].is_active) {
            break;
        }

        if (objects->hitboxes[i].health == 0) {
            remove_hitbox(objects, i);
        }
    }
}

void remove_hitbox(world_objects *objects, int i) {
    objects->hitboxes[i].is_active = false;
    objects->hitboxes[i] = objects->hitboxes[objects->hitbox_count - 1];
    objects->hitboxes[objects->hitbox_count].is_active = false;
    objects->hitbox_count--;
}

// If a collision with the player occurs, deal damage
// Decrement all timers. If it becomes negative or 0, set it to 0, make inactive, and move last active hurtbox into that slot.
void update_hurtboxes(world_objects *objects, time time) {
    for (int i = 0; i < objects->hurtbox_count; i++) {
        if (!objects->hurtboxes[i].is_active) {
            break;
        }
        objects->hurtboxes[i].duration -= time.dt;

        if (objects->hurtboxes[i].duration < 0) {
            remove_hurtbox(objects, i);
        }
    }
}

void remove_hurtbox(world_objects *objects, int i) {
    objects->hurtboxes[i].duration = 0;
    objects->hurtboxes[i].is_active = false;
    objects->hurtboxes[i] = objects->hurtboxes[objects->hurtbox_count - 1];
    objects->hurtboxes[objects->hurtbox_count].is_active = false;
    objects->hurtbox_count--;
}

// ret true if any hurtbox collision
bool check_player_hurtbox_collision(world_objects objects, player player) {
    for (int i = 0; i < objects.hurtbox_count; i++) {
        hurtbox hurtbox = objects.hurtboxes[i];
        if (!hurtbox.is_active) {
            break;
        }

        // guard statement to prevent uninitialised errors
        if (!hurtbox.is_active) {
            continue;
        }

        if (hurtbox.shape == LINE) {
            if (CheckCollisionCircleLine(player.position, PLAYER_RADIUS, hurtbox.line.start, hurtbox.line.end)) {
                return true;
            }
        } else if (hurtbox.shape == CIRCLE) {
            if (CheckCollisionCircles(player.position, PLAYER_RADIUS, hurtbox.circle.centre, hurtbox.circle.radius)) {
                return true;
            }
        } else if (hurtbox.shape == RECTANGLE) {
            if (CheckCollisionCircleRec(player.position, PLAYER_RADIUS, hurtbox.rectangle)) {
                return true;
            }
        }
    }
    return false;
}

void create_line_hurtbox(world_objects *objects, Vector2 start, Vector2 target, double duration) {
    hurtbox hurtbox;
    hurtbox.is_active = true;
    hurtbox.shape = LINE;
    hurtbox.duration = duration;
    hurtbox.line.start = start;
    hurtbox.line.end = target;
    objects->hurtboxes[objects->hurtbox_count++] = hurtbox;
}

void create_circle_hurtbox(world_objects *objects, Vector2 centre, double radius, double duration) {
    hurtbox hurtbox;
    hurtbox.is_active = true;
    hurtbox.shape = CIRCLE;
    hurtbox.duration = duration;
    hurtbox.circle.centre = centre;
    hurtbox.circle.radius = radius;
    objects->hurtboxes[objects->hurtbox_count++] = hurtbox;
}

void create_rectangle_hurtbox(world_objects *objects, Rectangle rectangle, double duration) {
    hurtbox hurtbox;
    hurtbox.is_active = true;
    hurtbox.shape = RECTANGLE;
    hurtbox.duration = duration;
    hurtbox.rectangle = rectangle;
    objects->hurtboxes[objects->hurtbox_count++] = hurtbox;
}

// Updates player's invincibility
void update_player_invincibility(player *player, time time) {
    if (player->invincible_duration > 0) {
        player->invincible_duration -= time.dt;
    } else if (player->invincible_duration < 0.0) {
        player->invincible_duration = 0.0;
    }
} 

// Deals with collisions, currently prevents player from leaving bounds.
void collide_player_world(player *player) {
    if (player->position.x > MAP_SIZE - PLAYER_RADIUS) {
        player->position.x = MAP_SIZE - PLAYER_RADIUS;
        bounce_player_world(player, Y);
    }
    
    if (player->position.x < -MAP_SIZE + PLAYER_RADIUS) {
        player->position.x = -MAP_SIZE + PLAYER_RADIUS;
        bounce_player_world(player, Y);
    }
    
    if (player->position.y > MAP_SIZE - PLAYER_RADIUS) {
        player->position.y = MAP_SIZE - PLAYER_RADIUS;
        bounce_player_world(player, X);
    }
    
    if (player->position.y < -MAP_SIZE + PLAYER_RADIUS) {
        player->position.y = -MAP_SIZE + PLAYER_RADIUS;
        bounce_player_world(player, X);
    }
}

// Bounces player.
void bounce_player_world(player *player, enum axis direction) {
    float bounciness = DASH_BOUNCINESS;

    if (!player->dash.is_dashing) {
        bounciness = NOT_DASHING_BOUNCE_COE * DASH_BOUNCINESS;
     }
    
    
    if (direction == X) {
        player->speed.y *= -bounciness;
        player->dash.direction.y *= -1;
    } 
    else {
        player->speed.x *= -bounciness;
        player->dash.direction.x *= -1;
    }
    
    extend_dash(player);
}

void extend_dash (player *player) {
    player->dash.duration *= BOUNCE_EXTENSION_COE;

    if (player->dash.duration > DASH_DURATION) 
    {
        player->dash.duration = DASH_DURATION;
    }   
}

void dash_zoom(player *player, time time, game_world *world) { //implement accelration for camera zoom

    if (player->dash.is_dashing) {
        double t = DASH_DURATION - player->dash.duration;
        world->camera.zoom = MAX_CAMERA_ZOOM + (DEFAULT_CAMERA_ZOOM - MAX_CAMERA_ZOOM) * exp(-20*t);
        printf("%lf\n", world->camera.zoom);
    }
    else {
        double t = DASH_COOLDOWN - player->dash.cooldown;
        printf("%lf\n", world->camera.zoom);
        world->camera.zoom = DEFAULT_CAMERA_ZOOM + (MAX_CAMERA_ZOOM - DEFAULT_CAMERA_ZOOM) * exp(-20*t);
    }
}   

// Dashes by dropping resistance and increases acceleration temporarily
void dash_player(player *player, time time) {

    if (should_player_dash(*player)) {
        player->dash.duration = DASH_DURATION;
        player->dash.direction = get_input_acceleration();
        player->dash.is_dashing = true;
    }

    if (player->dash.duration < 0) {
        player->dash.duration = 0;
        player->dash.cooldown = DASH_COOLDOWN;
        player->dash.is_dashing = false;
        player->dash.direction = Vector2Zero();
    } else if (player->dash.is_dashing) {
        player->dash.duration -= time.dt;
    }

    if (player->dash.cooldown <= 0) {
        player->dash.cooldown = 0;
    } 
    else {
        player->dash.cooldown -= time.dt;
    }
}

// Dashes if space is hit, cooldown is 0, a direction is held, and is not currently dashing
bool should_player_dash (player player) {
    if (!IsKeyPressed(KEY_SPACE)) {
        return false;
    } else if (player.dash.cooldown != 0) {
        return false;
    } else if (Vector2Length(get_input_acceleration()) == 0) {
        return false;
    } else if (player.dash.is_dashing) {
        return false;
    } else {
        return true;
    }
}

// Player movement, based on acceleration
void move_player(player *player, time time, world_objects *objects) {

    Vector2 input_acceleration;

    if (!player->dash.is_dashing) {
        input_acceleration = get_input_acceleration();
    } else {
        input_acceleration = player->dash.direction;
    }

    Vector2 resistance = Vector2Scale(player->speed, -1);

    resistance = Vector2Scale(resistance, PLAYER_RESISTANCE_COE);
    input_acceleration = Vector2Scale(input_acceleration, PLAYER_ACCELERATION_COE);

    if (player->dash.is_dashing) {
        resistance = Vector2Scale(resistance, DASH_RESISTANCE_COE);
        input_acceleration = Vector2Scale(input_acceleration, DASH_ACCELERATION_COE);
    }

    player->acceleration = Vector2Add(resistance, input_acceleration);

    update_player_hitbox_collision(objects, player);

    player->speed = Vector2Add(player->speed, Vector2Scale(player->acceleration, time.dt));

    player->position = Vector2Add(player->position, Vector2Scale(player->speed, time.dt));
}


void update_player_hitbox_collision(world_objects *objects, player *player) {

    for (int i = 0; i < objects->hitbox_count; i++) {
        hitbox hitbox = objects->hitboxes[i];
        if (!hitbox.is_active) {
            break;
        }

        if (CheckCollisionCircles(player->position, PLAYER_RADIUS, hitbox.circle.centre, hitbox.circle.radius)) {

            if (player->dash.is_dashing) {
                objects->hitboxes[i].health--;
                extend_dash(player);

                if (objects->hitboxes[i].health == 0) {
                    // slowmo
                    return;
                }
            }

            Vector2 launch_vector = Vector2Subtract(player->position, hitbox.circle.centre);
            launch_vector = normalise_vector(launch_vector);
            player->dash.direction = launch_vector;
            launch_vector = Vector2Scale(launch_vector, HITBOX_COLLISION_LAUNCH_COE);
            player->acceleration = Vector2Add(player->acceleration, launch_vector);

            if (player->invincible_duration == 0 && !player->dash.is_dashing) {
                player->health--;
                player->invincible_duration = INVINCIBILITY_DURATION;
            }

            player->dash.is_dashing = true;
            player->dash.duration = DASH_DURATION;
            player->dash.cooldown = DASH_COOLDOWN;
        }
    }
}

// On kill, slow down and immediately refresh dash.


// Returns a unit vector in direction of wasd input
Vector2 get_input_acceleration(void) {
    Vector2 movement;
    movement.x = 0;
    movement.y = 0;
    if (IsKeyDown(KEY_W)) {
        movement.y -= 1;
    }
    
    if (IsKeyDown(KEY_A)) {
        movement.x -= 1;
    }
    
    if (IsKeyDown(KEY_S)) {
        movement.y += 1;
    }
    
    if (IsKeyDown(KEY_D)) {
        movement.x += 1;
    }

    movement = normalise_vector(movement);
    return movement;
}

// take a wild guess.
Vector2 normalise_vector(Vector2 vector) {
    double length = Vector2Length(vector);
    if (length == 0) {
        return vector;
    }
    vector.x /= length;
    vector.y /= length;
    return vector;
}

