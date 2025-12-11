#ifndef WORLD_H
#define WORLD_H

#include "stdio.h"
#include "header.h"

#define INVINCIBILITY_DURATION 1

void initialise_world(game_world *world);
void initialise_player(player *player);
void initialise_time(time *time);

void draw_hitboxes(world_objects *objects);
void create_test_hitboxes(world_objects *objects);

void draw_hurtboxes(world_objects *objects);
void create_test_hurtboxes(world_objects *objects);
#endif