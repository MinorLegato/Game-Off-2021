
static void updatePlayer(GameState* gs, f32 dt) {
    Camera* cam = &gs->cam;

    if (platform.keyboard.down[KEY_W]) { cam->pos.y += 8 * dt; }
    if (platform.keyboard.down[KEY_S]) { cam->pos.y -= 8 * dt; }
    if (platform.keyboard.down[KEY_A]) { cam->pos.x -= 8 * dt; }
    if (platform.keyboard.down[KEY_D]) { cam->pos.x += 8 * dt; }

    if (platform.keyboard.down[KEY_KP_ADD])         { cam->pos.z -= 8 * dt; }
    if (platform.keyboard.down[KEY_KP_SUBTRACT])    { cam->pos.z += 8 * dt; }

    if (platform.mouse.down[MOUSE_BUTTON_LEFT]) {
        if (Tile* tile = gs->map.getTile(floorf(mouse_position.x), floorf(mouse_position.y))) {
            tile->order = OrderType_DestroyTile;
        }
    }
    
    if (platform.mouse.down[MOUSE_BUTTON_RIGHT]) {
        if (Tile* tile = gs->map.getTile(floorf(mouse_position.x), floorf(mouse_position.y))) {
            tile->order     = OrderType_None;
            tile->worker_id = 0;
        }
    }
}

static void updateEntityPhysics(GameState* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        Entity* e = &gs->entity_array[i];

        e->pos += e->vel * dt;
        e->vel -= 4 * e->vel * dt;
    }
}

static b32 findClosestWorkOrder(Entity* e, GameState* gs) {
    path_finder.init(v2i(e->pos), &gs->map);

    while (!path_finder.empty()) {
        Vec2i pos = path_finder.pop();
        
        for (int i = 0; i < ARRAY_COUNT(path_dirs); ++i) {
            Vec2i next = pos + path_dirs[i];

            if (Tile* tile = gs->map.getTile(next.x, next.y)) {
                if (tile->order && !tile->worker_id) {
                    tile->worker_id = e->id;
                    e->target_pos = next;

                    return true;
                }

                path_finder.push(next);
            }
        }
    }

    return false;
}

static void moveEntityTowardTarget(Entity* e, GameState* gs, f32 dt) {
    static Vec2i from_map[MAP_SIZE][MAP_SIZE];

    path_finder.init(e->target_pos, &gs->map);

    while (!path_finder.empty()) {
        Vec2i pos = path_finder.pop();
        
        for (int i = 0; i < ARRAY_COUNT(path_dirs); ++i) {
            Vec2i next = pos + path_dirs[i];

            if (next == v2i(e->pos)) {
                e->vel += 4 * v2Norm(v2(pos) + v2(0.5) - e->pos) * dt;
                return;
            }

            path_finder.push(next);
        }
    }
}

static void updateEntityAI(GameState* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        Entity* e = &gs->entity_array[i];

        switch (e->ai) {
            // Worker AI:
            case AI_WorkerIdle: {
                if (findClosestWorkOrder(e, gs)) {
                    e->ai = AI_WorkerExecuteOrder;
                }
            } break;
            case AI_WorkerExecuteOrder: {
                moveEntityTowardTarget(e, gs, dt);

                if (Tile* tile = gs->map.getTile(e->target_pos.x, e->target_pos.y)) {
                    if (tile->order) {
                        if (v2DistSq(e->pos, v2(e->target_pos) + v2(0.5)) <= (0.6 + entity_info_table[e->type].rad)) {
                            switch (tile->order) {
                                case OrderType_DestroyTile: {
                                    destroyTile(tile);
                                } break;
                            }

                            e->target_pos = {};
                            e->ai = AI_WorkerIdle;
                            tile->worker_id = 0;
                        }
                    } else {
                        e->target_pos = {};
                        e->ai = AI_WorkerIdle;
                        tile->worker_id = 0;
                    }
                }
            } break;
            // Guard AI:
            case AI_GuardIdle: {
                //e->vel += 6 * randUnitVec2(&rs) * dt;
            } break;
        }
    }
}

static c2Circle getEntityCircle(const Entity* e) {
    c2Circle c;
    c.p = { e->pos.x, e->pos.y };
    c.r = entity_info_table[e->type].rad;
    return c;
}

static void handleEntityCollisions(GameState* gs, f32 dt) {
    // @TODO: broad-phase this shit!
    c2Manifold m;
    for (u32 i = 0; i < gs->entity_count; ++i) {
        Entity*     a           = &gs->entity_array[i];
        c2Circle    a_circle    = getEntityCircle(a);

        for (u32 j = 0; j < gs->entity_count; ++j) {
            if (i == j) continue;
            const Entity* b = &gs->entity_array[j];

            c2CircletoCircleManifold(a_circle, getEntityCircle(b), &m);
            for (int k = 0; k < m.count; ++k) {
                a->pos.x -= m.depths[k] * m.n.x;
                a->pos.y -= m.depths[k] * m.n.y;
                a->vel.x -= m.depths[k] * m.n.x * dt;
                a->vel.y -= m.depths[k] * m.n.y * dt;
            }
        }

        Rect2i map_rect = { v2i(a->pos) - v2i(a->getInfo()->rad + 1), v2i(a->pos) + v2i(a->getInfo()->rad + 1) };
        for_rect2(map_rect, x, y) {
            if (!gs->map.isTraversable(x, y)) {
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

static void updateEntities(GameState* gs, f32 dt) {
    updateEntityAI(gs, dt);
    updateEntityPhysics(gs, dt);
    handleEntityCollisions(gs, dt);
}

static void updateMap(GameState* gs, f32 dt) {
    for_map(x, y) {
        Tile* tile = &gs->map.tiles[y][x];
        
        switch (tile->order) {
            case OrderType_DestroyTile: {
                if (tile->type == TileType_Dirt) {
                    tile->order = OrderType_None;
                }
            } break;
        }
    }
}

static void updateGame(GameState* gs, f32 dt) {
    updatePlayer(gs, dt);
    updateMap(gs, dt);
    updateEntities(gs, dt);
}

