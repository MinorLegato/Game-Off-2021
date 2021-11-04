
static void updatePlayer(GameState* gs, f32 dt) {
    Camera* cam = &gs->cam;

    if (platform.keyboard.down[KEY_W]) { cam->pos.y += 8 * dt; }
    if (platform.keyboard.down[KEY_S]) { cam->pos.y -= 8 * dt; }
    if (platform.keyboard.down[KEY_A]) { cam->pos.x -= 8 * dt; }
    if (platform.keyboard.down[KEY_D]) { cam->pos.x += 8 * dt; }

    if (platform.keyboard.down[KEY_KP_ADD])         { cam->pos.z -= 8 * dt; }
    if (platform.keyboard.down[KEY_KP_SUBTRACT])    { cam->pos.z += 8 * dt; }
}

static void updateEntityPhysics(GameState* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        Entity* e = &gs->entity_array[i];

        e->pos += e->vel * dt;
        e->vel -= 4 * e->vel * dt;
    }
}

static void updateEntityAI(GameState* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        Entity* e = &gs->entity_array[i];

        switch (e->ai) {
            // Worker AI:
            case AI_WorkerIdle: {
                e->vel += 4 * randUnitVec2(&rs) * dt;
            } break;
            // Guard AI:
            case AI_GuardIdle: {
                e->vel += 6 * randUnitVec2(&rs) * dt;
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

static void updateGame(GameState* gs, f32 dt) {
    updatePlayer(gs, dt);
    updateEntities(gs, dt);
}

