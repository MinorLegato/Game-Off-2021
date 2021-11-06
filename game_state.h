
#define ENTITY_MAX      (2 * 1024)
#define PARTICLE_MAX    (8 * 1024)

struct game_state_t {
    camera_t        cam;
    map_t           map;

    order_type_t    order_tool;

    u32             next_id;
    u32             entity_count;
    entity_t        entity_array[ENTITY_MAX];

    u32             particle_count;
    particle_t      particle_array[PARTICLE_MAX];
};

static entity_t* new_entity(game_state_t* gs, entity_type_t type, vec2_t pos, vec2_t vel = {}) {
    if (gs->entity_count >= ENTITY_MAX) return NULL;

    entity_t* e = &gs->entity_array[gs->entity_count++];
    const entity_info_t* info = &entity_info_table[type];

    e->type     = type;
    e->id       = ++gs->next_id;
    e->pos      = pos;
    e->vel      = vel;
    e->life     = info->max_life;
    e->ai       = info->ai;

    return e;
}

static entity_t* get_entity(game_state_t* gs, u32 id) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t* e = &gs->entity_array[i];
        if (id == e->id) {
            return e;
        }
    }

    return NULL;
}

