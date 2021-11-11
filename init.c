
static void generate_map(map_t* map) {
    for_map(x, y) {
        tile_type_t tile_type = TILE_TYPE_ROCK;

        if (rand_i32(&rs, 0, 100) < 10 || v2_dist_sq(v2(x + 0.5, y + 0.5), v2(0.5 * MAP_SIZE, 0.5 * MAP_SIZE)) < 3 * 3) {
            tile_type = TILE_TYPE_COPPER;
        }

        if (rand_i32(&rs, 0, 100) < 15 || v2_dist_sq(v2(x + 0.5, y + 0.5), v2(0.5 * MAP_SIZE, 0.5 * MAP_SIZE)) < 3 * 3) {
            tile_type = TILE_TYPE_DIRT;
        }

        init_tile(&map->tiles[y][x], tile_type);
    }
}

static void init_game(game_state_t* gs) {
    gs->next_id         = 0;
    gs->entity_count    = 0;
    gs->particle_count  = 0;

    gs->cam.pos = v3(0.5 * MAP_SIZE, 0.5 * MAP_SIZE, 8);

    gs->order_tool = ORDER_TYPE_DESTROY_TILE;

    generate_map(&gs->map);

    for (u32 i = 0; i < 3; ++i) {
        new_entity(gs, &(entity_desc_t) {
            .type   = ENTITY_TYPE_WORKER,
            .pos    = v2(0.5 * MAP_SIZE + rand_f32(&rs, -3, 3), 0.5 * MAP_SIZE + rand_f32(&rs, -3, 3)),
        });
    }

    for (u32 i = 0; i < 2; ++i) {
        new_entity(gs, &(entity_desc_t) {
            .type   = ENTITY_TYPE_GUARD,
            .pos    = v2(0.5 * MAP_SIZE + rand_f32(&rs, -3, 3), 0.5 * MAP_SIZE + rand_f32(&rs, -3, 3)),
        });
    }

    for (u32 i = 0; i < 1024; ++i) {
        vec2i_t pos = { rand_i32(&rs, 0, MAP_SIZE - 1), rand_i32(&rs, 0, MAP_SIZE - 1) };

        if (map_is_traversable(&gs->map, pos.x, pos.y)) {
            new_entity(gs, &(entity_desc_t) {
                .type   = ENTITY_TYPE_ANT,
                .pos    = v2(pos.x + 0.5, pos.y + 0.5),
            });
        }
    }

    mouse_position = v3(.xy = gs->cam.pos.xy);
}

