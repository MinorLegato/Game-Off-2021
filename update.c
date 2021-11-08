
static void update_player(game_state_t* gs, f32 dt) {
    camera_t* cam = &gs->cam;

    if (platform.keyboard.down[KEY_W]) { cam->pos.y += 8 * dt; }
    if (platform.keyboard.down[KEY_S]) { cam->pos.y -= 8 * dt; }
    if (platform.keyboard.down[KEY_A]) { cam->pos.x -= 8 * dt; }
    if (platform.keyboard.down[KEY_D]) { cam->pos.x += 8 * dt; }

    if (platform.keyboard.down[KEY_KP_ADD])         { cam->pos.z -= 8 * dt; }
    if (platform.keyboard.down[KEY_KP_SUBTRACT])    { cam->pos.z += 8 * dt; }

    if (platform.mouse.down[MOUSE_BUTTON_LEFT]) {
        tile_t* tile = NULL;

        if (tile = map_get_tile(&gs->map, floorf(mouse_position.x), floorf(mouse_position.y))) {
            tile->order = gs->order_tool;
        }
    }
    
    if (platform.mouse.down[MOUSE_BUTTON_RIGHT]) {
        tile_t* tile = NULL;

        if (tile = map_get_tile(&gs->map, floorf(mouse_position.x), floorf(mouse_position.y))) {
            tile->order     = ORDER_TYPE_NONE;
            tile->worker_id = 0;
        }
    }

    if (platform.mouse.scroll.y < 0) {
        gs->order_tool++;
    }

    if (platform.mouse.scroll.y > 0) {
        gs->order_tool--;
    }

    gs->order_tool = CLAMP(gs->order_tool, ORDER_TYPE_NONE + 1, ORDER_TYPE_COUNT - 1);
}

static void update_entity_physics(game_state_t* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t* e = &gs->entity_array[i];

        e->pos.x += e->vel.x * dt;
        e->pos.y += e->vel.y * dt;

        e->vel.x -= 4 * e->vel.x * dt;
        e->vel.y -= 4 * e->vel.y * dt;
    }
}

static void find_work(entity_t* e, game_state_t* gs) {
    path_init(v2_cast(vec2i_t, e->pos), &gs->map);

    while (!path_empty()) {
        vec2i_t pos = path_pop();
        
        for (int i = 0; i < ARRAY_COUNT(path_dirs); ++i) {
            vec2i_t     next    = v2i_add(pos, path_dirs[i]);
            tile_t*     tile    = map_get_tile(&gs->map, next.x, next.y);
            entity_t*   worker  = NULL;

            if (tile) {
                if (tile->order) {
                    if (!tile->worker_id) {
                        tile->worker_id = e->id;

                        e->ai           = AI_WORKER_EXECUTE_ORDER;
                        e->target_pos   = next;

                        return;
                    } if (worker = get_entity(gs, tile->worker_id)) {
                        // transfrer order to entity 'e', if 'e' is closer:
                        if (e != worker && v2_dist(e->pos, v2(next.x + 0.5, next.y + 0.5)) < v2_dist(worker->pos, v2(next.x + 0.5, next.y + 0.5))) {
                            worker->ai          = AI_WORKER_IDLE;
                            worker->target_pos  = v2i(0); 

                            tile->worker_id     = e->id;

                            e->ai               = AI_WORKER_EXECUTE_ORDER;
                            e->target_pos       = next;

                            return;
                        }
                    } else {
                        // current worker has disappeared, transfrer order to entity 'e':
                        tile->worker_id = e->id;

                        e->ai           = AI_WORKER_EXECUTE_ORDER;
                        e->target_pos   = next;

                        return;
                    }
                }

                path_push(next);
            }
        }
    }
}

static void move_entity_towards_target(entity_t* e, game_state_t* gs, f32 dt) {
    path_init(e->target_pos, &gs->map);

    while (!path_empty()) {
        vec2i_t pos = path_pop();
        
        for (int i = 0; i < ARRAY_COUNT(path_dirs); ++i) {
            vec2i_t next = v2i_add(pos, path_dirs[i]);

            if (next.x == (i32)e->pos.x && next.y == (i32)e->pos.y) {
                vec2_t dir = v2_norm(v2_sub(v2(pos.x + 0.5, pos.y + 0.5), e->pos));

                e->vel.x += 6 * dir.x * dt;
                e->vel.y += 6 * dir.y * dt;
                return;
            }

            path_push(next);
        }
    }
}

static void update_entity_ai(game_state_t* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t* e = &gs->entity_array[i];

        switch (e->ai) {
            // Worker AI:
            case AI_WORKER_IDLE: {
                find_work(e, gs);
            } break;
            case AI_WORKER_EXECUTE_ORDER: {
                move_entity_towards_target(e, gs, dt);

                tile_t* tile = NULL;
                if (tile = map_get_tile(&gs->map, e->target_pos.x, e->target_pos.y)) {
                    if (tile->order) {
                        if (v2_dist_sq(e->pos, v2(e->target_pos.x + 0.5, e->target_pos.y + 0.5)) <= (0.6 + entity_info_table[e->type].rad)) {
                            switch (tile->order) {
                                case ORDER_TYPE_DESTROY_TILE: {
                                    destroy_tile(tile);
                                } break;
                                case ORDER_TYPE_BUILD_ROCK_WALL: {
                                    init_tile(tile, TILE_TYPE_ROCK_WALL);
                                } break;
                            }

                            e->target_pos   = v2i(0);
                            e->ai           = AI_WORKER_IDLE;
                            tile->worker_id = 0;
                        }
                    } else {
                        e->target_pos   = v2i(0);
                        e->ai           = AI_WORKER_IDLE;
                        tile->worker_id = 0;
                    }
                }
            } break;
            // Guard AI:
            case AI_GUARD_IDLE: {
                vec2_t ru = rand_unit_v2(&rs);

                e->vel.x += 6 * ru.x * dt;
                e->vel.y += 6 * ru.y * dt;
            } break;
        }
    }
}

static c2Circle get_entity_circle(const entity_t* e) {
    return (c2Circle) {
        .p = { e->pos.x, e->pos.y },
        .r = entity_info_table[e->type].rad,
    };
}

static void handle_entity_collisions(game_state_t* gs, f32 dt) {
    // @TODO: broad-phase this shit!
    c2Manifold m;
    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t* a          = &gs->entity_array[i];
        c2Circle  a_circle   = get_entity_circle(a);

        for (u32 j = 0; j < gs->entity_count; ++j) {
            if (i == j) continue;
            const entity_t* b = &gs->entity_array[j];

            c2CircletoCircleManifold(a_circle, get_entity_circle(b), &m);
            for (int k = 0; k < m.count; ++k) {
                a->pos.x -= m.depths[k] * m.n.x;
                a->pos.y -= m.depths[k] * m.n.y;
                a->vel.x -= m.depths[k] * m.n.x * dt;
                a->vel.y -= m.depths[k] * m.n.y * dt;
            }
        }

        rect2i_t map_rect = {
            (i32)(a->pos.x - entity_get_info(a)->rad),
            (i32)(a->pos.y - entity_get_info(a)->rad),
            (i32)(a->pos.x + entity_get_info(a)->rad),
            (i32)(a->pos.y + entity_get_info(a)->rad),
        };

        for_rect2(map_rect, x, y) {
            if (!map_is_traversable(&gs->map, x, y)) {
                c2AABB tile_aabb = { { (f32)x, (f32)y }, { x + 1.0f, y + 1.0f } };

                c2CircletoAABBManifold(a_circle, tile_aabb, &m);
                for (int k = 0; k < m.count; ++k) {
                    a->pos.x -= m.depths[k] * m.n.x;
                    a->pos.y -= m.depths[k] * m.n.y;
                    a->vel.x -= m.depths[k] * m.n.x * dt;
                    a->vel.y -= m.depths[k] * m.n.y * dt;
                }
            }
        }
    }
}

static void update_entities(game_state_t* gs, f32 dt) {
    update_entity_ai(gs, dt);
    update_entity_physics(gs, dt);
    handle_entity_collisions(gs, dt);
}

static void update_map(game_state_t* gs, f32 dt) {
    for_map(x, y) {
        tile_t* tile = &gs->map.tiles[y][x];

        if (tile_get_info(tile)->is_wall == order_info_table[tile->type].on_ground) {
            tile->order = ORDER_TYPE_NONE;
        }
    }
}

static void update_game(game_state_t* gs, f32 dt) {
    update_player(gs, dt);
    update_map(gs, dt);
    update_entities(gs, dt);
}

