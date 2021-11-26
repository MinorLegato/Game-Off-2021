
#define ENTITY_MAX      (2 * 1024)
#define PARTICLE_MAX    (8 * 1024)

typedef struct game_state_t {
    camera_t        cam;
    map_t           map;

    order_type_t    order_tool;

    u32             next_id;
    u32             entity_count;
    entity_t        entity_array[ENTITY_MAX];

    u32             particle_count;
    particle_t      particle_array[PARTICLE_MAX];
} game_state_t;

static entity_t* add_entity(game_state_t* gs, const entity_desc_t* desc) {
    if (gs->entity_count >= ENTITY_MAX) return NULL;

    entity_t* e = &gs->entity_array[gs->entity_count++];
    const entity_info_t* info = &entity_info_table[desc->type];

    e->type     = desc->type;
    e->id       = ++gs->next_id;
    e->pos      = desc->pos;
    e->vel      = desc->vel;
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

static vec4_t randomize_color(vec4_t color, f32 r) {
    if (r == 0) return color;

    color.r = clamp_f32(color.r + rand_f32(&rs, -r, r), 0, 1);
    color.g = clamp_f32(color.g + rand_f32(&rs, -r, r), 0, 1);
    color.b = clamp_f32(color.b + rand_f32(&rs, -r, r), 0, 1);

    return color;
}

static void add_particle(game_state_t* gs, const particle_desc_t* desc) {
    u32 count = CLAMP_MIN(desc->count, 1);

    for (u32 i = 0; (i < count) && (gs->particle_count < PARTICLE_MAX); ++i) {
        particle_t* p = &gs->particle_array[gs->entity_count++];

        p->pos          = v3_add(desc->pos, v3_scale(rand_unit_v3(&rs), desc->rand.pos));
        p->vel          = v3_add(desc->vel, v3_scale(rand_unit_v3(&rs), desc->rand.vel));
        p->rad          = desc->rad + rand_f32(&rs, -desc->rad, desc->rad);

        p->turbulance   = desc->turbulance;

        p->life         = desc->life;
        p->max_life     = p->life;

        p->start_color  = pack_color_v4(randomize_color(desc->start_color, desc->rand.start_color));
        p->end_color    = pack_color_v4(randomize_color(desc->end_color, desc->rand.end_color));
    }
}

