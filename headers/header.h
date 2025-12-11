#ifndef HEADER_H
#define HEADER_H

#include "raylib.h"

#define MAP_SIZE 750

#define FPS 120

#define DEFAULT_CAMERA_ZOOM 1
#define MAX_CAMERA_ZOOM 1.5

#define PLAYER_RADIUS 5
#define PLAYER_RESISTANCE_COE 8
#define PLAYER_ACCELERATION_COE 6000

#define MAX_ENTITIES 16

enum axis {
    X,
    Y
};

enum hurtbox_shape {
    LINE,
    CIRCLE,
    RECTANGLE,
    MAX_HURTBOX_SHAPE
};

// Hurtboxes are specifically for attacks
// Hitboxes only deal damage if the player is not dashing.

typedef struct {
    Vector2 speed;
    Vector2 position;
    int health;
} entity;

typedef struct {
    Vector2 start;
    Vector2 end;
} line;

typedef struct {
    Vector2 centre;
    double radius;
} circle;

typedef struct {
    bool is_active;
    enum hurtbox_shape shape;
    line line;
    circle circle;
    Rectangle rectangle;
    double duration;
} hurtbox;

typedef struct {
    bool is_active;
    bool is_solid;
    circle circle;
    int health;
    double invincible_duration;
} hitbox;

typedef struct {
    double dt;
    double total;
} time;

typedef struct {
    int width;
    int height;
} screen;

struct dash {
    bool is_dashing;
    double duration;
    double cooldown;
    Vector2 direction;
};

typedef struct {
    Vector2 speed;
    Vector2 position;
    Vector2 acceleration;
    int health;
    double invincible_duration;
    struct dash dash;
} player;

typedef enum {
    OBSTACLE,
    PROJECTILE,
} ObjectType;

typedef struct {
    int hitbox_index;
} enemy;

typedef struct {
    hurtbox hurtboxes[MAX_ENTITIES];
    int hurtbox_count;
    hitbox hitboxes[MAX_ENTITIES];
    int hitbox_count;
} world_objects;

typedef struct {
    world_objects objects;
    Camera2D camera;
    player player;
    screen screen;
} game_world;

void draw_debug(time time, game_world *world);

void update_draw_frame(game_world *world, time time);
void draw_map(void);

void update_player(game_world *world, time time);

void update_time(time *time, player *player);

#endif