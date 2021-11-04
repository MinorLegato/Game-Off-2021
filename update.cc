
static void updateCamera(GameState* gs, f32 dt) {
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

static void updateEntities(GameState* gs, f32 dt) {
    updateEntityAI(gs, dt);
    updateEntityPhysics(gs, dt);
}

static void updateGame(GameState* gs, f32 dt) {
    updateCamera(gs, dt);
    updateEntities(gs, dt);
}

