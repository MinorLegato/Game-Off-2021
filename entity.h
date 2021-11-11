
typedef u32 entity_type_t;
enum {
    ENTITY_TYPE_NONE,
    // units:
    ENTITY_TYPE_WORKER,
    ENTITY_TYPE_GUARD,
    // enemy units:
    ENTITY_TYPE_ANT,
    //
    ENTITY_TYPE_COUNT,
};

typedef u32 ai_type_t;
enum {
    AI_NONE,
    // Worker AI:
    AI_WORKER_IDLE,
    AI_WORKER_EXECUTE_ORDER,
    // Guard AI:
    AI_GUARD_IDLE,
    AI_GUARD_KILL_TARGET,
    // Ant AI:
    AI_ANT_IDLE,
    AI_ANT_AGRO,
    //
    AI_Count,
};

typedef struct entity_info_t {
    ai_type_t   ai;
    f32         rad;
    u32         color;
    f32         max_life;
    
    char        name[32];
    char        texture[32];
} entity_info_t;

static entity_info_t entity_info_table[ENTITY_TYPE_COUNT] = {
    [ENTITY_TYPE_NONE] = {
        .rad   = 0.3,
        .color = 0xff22bb22,
    },

    [ENTITY_TYPE_WORKER] = {
        .name       = "worker",
        .texture    = "human",
        .ai         = AI_WORKER_IDLE,
        .rad        = 0.24,
        .color      = 0xff22bb22,
        .max_life   = 1.0,
    },

    [ENTITY_TYPE_GUARD] = {
        .name       = "guard",
        .texture    = "human",
        .ai         = AI_GUARD_IDLE,
        .rad        = 0.24,
        .color      = 0xffbb4422,
        .max_life   = 2.0,
    },

    [ENTITY_TYPE_ANT] = {
        .name       = "ant",
        .texture    = "bug",
        .ai         = AI_ANT_IDLE,
        .rad        = 0.24,
        .color      = 0xff2244bb,
        .max_life   = 1.0,
    },
};

typedef struct entity_desc_t {
    entity_type_t   type;
    vec2_t          pos;
    vec2_t          vel;
} entity_desc_t;

typedef struct entity_t {
    entity_type_t   type;
    u32             id;

    vec2_t          pos;
    vec2_t          vel;
    f32             life;

    ai_type_t       ai;

    u32             target_id;
    vec2_t          target_pos;
} entity_t;

static const entity_info_t* entity_get_info(const entity_t* e) {
    return &entity_info_table[e->type];
}

static rect2_t entity_get_rect(const entity_t* e) {
    const entity_info_t* info = entity_get_info(e);

    return (rect2_t) {
        .min = { e->pos.x - info->rad, e->pos.y - info->rad },
        .max = { e->pos.x + info->rad, e->pos.y + info->rad },
    };
}

