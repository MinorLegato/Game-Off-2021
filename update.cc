
static void update_player(game_state_t* gs, f32 dt) {
    auto cam = &gs->cam;

    if (platform.keyboard.down[KEY_W]) { cam->pos.y += 8 * dt; }
    if (platform.keyboard.down[KEY_S]) { cam->pos.y -= 8 * dt; }
    if (platform.keyboard.down[KEY_A]) { cam->pos.x -= 8 * dt; }
    if (platform.keyboard.down[KEY_D]) { cam->pos.x += 8 * dt; }

    if (platform.keyboard.down[KEY_KP_ADD])         { cam->pos.z -= 8 * dt; }
    if (platform.keyboard.down[KEY_KP_SUBTRACT])    { cam->pos.z += 8 * dt; }

    if (platform.mouse.down[MOUSE_BUTTON_LEFT]) {
        if (auto tile = gs->map.get_tile(floorf(mouse_position.x), floorf(mouse_position.y))) {
            tile->order = gs->order_tool;
        }
    }
    
    if (platform.mouse.down[MOUSE_BUTTON_RIGHT]) {
        if (auto tile = gs->map.get_tile(floorf(mouse_position.x), floorf(mouse_position.y))) {
            tile->order     = ORDER_TYPE_NONE;
            tile->worker_id = 0;
        }
    }

    if (platform.mouse.scroll.y < 0) {
        gs->order_tool = (order_type_t)((u32)gs->order_tool + 1);
    }

    if (platform.mouse.scroll.y > 0) {
        gs->order_tool = (order_type_t)((u32)gs->order_tool - 1);
    }

    gs->order_tool = (order_type_t)CLAMP(gs->order_tool, (u32)ORDER_TYPE_NONE + 1, (u32)ORDER_TYPE_COUNT - 1);
}

static void update_entity_physics(game_state_t* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        auto e = &gs->entity_array[i];

        e->pos += e->vel * dt;
        e->vel -= 4 * e->vel * dt;
    }
}

static void find_work(entity_t* e, game_state_t* gs) {
    path_finder.init(v2i(e->pos), &gs->map);

    while (!path_finder.empty()) {
        auto pos = path_finder.pop();
        
        for (int i = 0; i < ARRAY_COUNT(path_dirs); ++i) {
            auto next = pos + path_dirs[i];

            if (auto tile = gs->map.get_tile(next.x, next.y)) {
                if (tile->order) {
                    if (!tile->worker_id) {
                        tile->worker_id = e->id;

                        e->ai           = AI_WORKER_EXECUTE_ORDER;
                        e->target_pos   = next;

                        return;
                    } if (auto worker = gs->get_entity(tile->worker_id)) {
                        // transfrer order to entity 'e', if 'e' is closer:
                        if (e != worker && v2_dist(e->pos, v2(next) + v2(0.5)) < v2_dist(worker->pos, v2(next) + v2(0.5))) {
                            worker->ai          = AI_WORKER_IDLE;
                            worker->target_pos  = {}; 

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

                path_finder.push(next);
            }
        }
    }
}

static void move_entity_towards_target(entity_t* e, game_state_t* gs, f32 dt) {
    path_finder.init(e->target_pos, &gs->map);

    while (!path_finder.empty()) {
        auto pos = path_finder.pop();
        
        for (int i = 0; i < ARRAY_COUNT(path_dirs); ++i) {
            auto next = pos + path_dirs[i];

            if (next == v2i(e->pos)) {
                e->vel += 4 * v2_norm(v2(pos) + v2(0.5) - e->pos) * dt;
                return;
            }

            path_finder.push(next);
        }
    }
}

static void update_entity_ai(game_state_t* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        auto e = &gs->entity_array[i];

        switch (e->ai) {
            // Worker AI:
            case AI_WORKER_IDLE: {
                find_work(e, gs);
            } break;
            case AI_WORKER_EXECUTE_ORDER: {
                move_entity_towards_target(e, gs, dt);

                if (auto tile = gs->map.get_tile(e->target_pos.x, e->target_pos.y)) {
                    if (tile->order) {
                        if (v2_dist_sq(e->pos, v2(e->target_pos) + v2(0.5)) <= (0.6 + entity_info_table[e->type].rad)) {
                            switch (tile->order) {
                                case ORDER_TYPE_DESTROY_TILE: {
                                    destroy_tile(tile);
                                } break;
                                case ORDER_TYPE_BUILD_ROCK_WALL: {
                                    init_tile(tile, TILE_TYPE_ROCK_WALL);
                                } break;
                            }

                            e->target_pos = {};
                            e->ai = AI_WORKER_IDLE;
                            tile->worker_id = 0;
                        }
                    } else {
                        e->target_pos = {};
                        e->ai = AI_WORKER_IDLE;
                        tile->worker_id = 0;
                    }
                }
            } break;
            // Guard AI:
            case AI_GUARD_IDLE: {
                e->vel += 6 * rand_unit_v2(&rs) * dt;
            } break;
        }
    }
}

static c2Circle get_entity_circle(const entity_t* e) {
    c2Circle c;
    c.p = { e->pos.x, e->pos.y };
    c.r = entity_info_table[e->type].rad;
    return c;
}

static void handle_entity_collisions(game_state_t* gs, f32 dt) {
    // @TODO: broad-phase this shit!
    c2Manifold m;
    for (u32 i = 0; i < gs->entity_count; ++i) {
        auto a          = &gs->entity_array[i];
        auto a_circle   = get_entity_circle(a);

        for (u32 j = 0; j < gs->entity_count; ++j) {
            if (i == j) continue;
            auto b = &gs->entity_array[j];

            c2CircletoCircleManifold(a_circle, get_entity_circle(b), &m);
            for (int k = 0; k < m.count; ++k) {
                a->pos.x -= m.depths[k] * m.n.x;
                a->pos.y -= m.depths[k] * m.n.y;
                a->vel.x -= m.depths[k] * m.n.x * dt;
                a->vel.y -= m.depths[k] * m.n.y * dt;
            }
        }

        rect2i_t map_rect = { v2i(a->pos) - v2i(a->get_info()->rad + 1), v2i(a->pos) + v2i(a->get_info()->rad + 1) };
        for_rect2(map_rect, x, y) {
            if (!gs->map.is_traversable(x, y)) {
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
        auto tile = &gs->map.tiles[y][x];
        
        switch (tile->order) {
            case ORDER_TYPE_DESTROY_TILE: {
                if (tile->type == TILE_TYPE_DIRT) {
                    tile->order = ORDER_TYPE_NONE;
                }
            } break;
        }
    }
}

static void update_game(game_state_t* gs, f32 dt) {
    update_player(gs, dt);
    update_map(gs, dt);
    update_entities(gs, dt);
}

