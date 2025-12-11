#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "header.h"

#define DASH_DURATION 0.2
#define DASH_COOLDOWN 1
#define DASH_RESISTANCE_COE 8
#define DASH_ACCELERATION_COE 15
#define DASH_BOUNCINESS 1.0
#define BOUNCE_EXTENSION_COE 2
#define NOT_DASHING_BOUNCE_COE 0.5
#define DASH_ZOOM_COE 3
#define DASH_ZOOM_DECAY 2

#define HITBOX_COLLISION_LAUNCH_COE 500000

#define SLOW_DURATION 1

void collide_player_world(player *player);
void bounce_player_world(player *player, enum axis direction);
void dash_player(player *player, time time);
void extend_dash (player *player);
void dash_zoom(player *player, time time, game_world *world);
bool should_player_dash (player player);
void move_player(player *player, time time, world_objects *objects);

void update_player_hitbox_collision(world_objects *objects, player *player);
void update_hitboxes(world_objects *objects, time time);
void remove_hitbox(world_objects *objects, int i);

void initialise_hurtboxes(world_objects *objects);
void update_hurtboxes(world_objects *objects, time time);
void remove_hurtbox(world_objects *objects, int i);
bool check_player_hurtbox_collision(world_objects objects, player player);

Vector2 get_projection(Vector2 v, Vector2 u);
Vector2 get_solid_hitbox_normal_acceleration(world_objects objects, player *player, time time, Vector2 acceleration);

void create_line_hurtbox(world_objects *objects, Vector2 start, Vector2 target, double duration);
void create_circle_hurtbox(world_objects *objects, Vector2 centre, double radius, double duration);
void create_rectangle_hurtbox(world_objects *objects, Rectangle rectangle, double duration);

void initialise_hitboxes(world_objects *objects);
void create_circle_hitbox(world_objects *objects, Vector2 centre, double radius, int health);

void update_hitboxes(world_objects *objects, time time);

void update_player_health(world_objects *objects, player *player);
void update_player_invincibility(player *player, time time);

Vector2 get_input_acceleration(void);
Vector2 normalise_vector(Vector2 vector);

#endif