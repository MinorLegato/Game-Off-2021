
enum entity_type_t : u32 {
    ENTITY_TYPE_NONE,
    // units:
    ENTITY_TYPE_WORKER,
    ENTITY_TYPE_GUARD,
    // enemy units:
    ENTITY_TYPE_ANT,
    //
    ENTITY_TYPE_COUNT,
};

enum ai_type_t : u32 {
    AI_NONE,
    // Worker AI:
    AI_WORKER_IDLE,
    AI_WORKER_EXECUTE_ORDER,
    // Guard AI:
    AI_GUARD_IDLE,
    //
    AI_Count,
};

struct entity_info_t {
    ai_type_t   ai;
    f32         rad;
    u32         color;
    f32         max_life;
};

static entity_info_t entity_info_table[ENTITY_TYPE_COUNT];

struct entity_t {
    entity_type_t   type;
    u32             id;

    vec2_t          pos;
    vec2_t          vel;
    f32             life;

    ai_type_t       ai;

    union {
        u32         target_id;
        vec2i_t     target_pos;
    };

    const entity_info_t* get_info(void) const {
        return &entity_info_table[type];
    }
};

static void init_entity_info_table(void) {
    {
        auto info = &entity_info_table[ENTITY_TYPE_NONE];

        info->rad   = 0.3;
        info->color = 0xff22bb22;
    }

    {
        auto info = &entity_info_table[ENTITY_TYPE_WORKER];

        info->ai        = AI_WORKER_IDLE;
        info->rad       = 0.18;
        info->color     = 0xff22bb22;
        info->max_life  = 1.0;
    }

    {
        auto info = &entity_info_table[ENTITY_TYPE_GUARD];

        info->ai        = AI_GUARD_IDLE;
        info->rad       = 0.2;
        info->color     = 0xffbb4422;
        info->max_life  = 2.0;
    }
}

