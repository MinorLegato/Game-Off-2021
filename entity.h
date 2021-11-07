
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
    //
    AI_Count,
};

typedef struct entity_info_t {
    ai_type_t   ai;
    f32         rad;
    u32         color;
    f32         max_life;
    
    char        name[32];
} entity_info_t;

static entity_info_t entity_info_table[ENTITY_TYPE_COUNT] = {
    [ENTITY_TYPE_NONE] = {
        .rad   = 0.3,
        .color = 0xff22bb22,
    },

    [ENTITY_TYPE_WORKER] = {
        .name       = "worker",
        .ai         = AI_WORKER_IDLE,
        .rad        = 0.18,
        .color      = 0xff22bb22,
        .max_life   = 1.0,
    },

    [ENTITY_TYPE_GUARD] = {
        .name       = "guard",
        .ai         = AI_GUARD_IDLE,
        .rad        = 0.2,
        .color      = 0xffbb4422,
        .max_life   = 2.0,
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

    union {
        u32         target_id;
        vec2i_t     target_pos;
    };
} entity_t;

const entity_info_t* entity_get_info(const entity_t* e) {
    return &entity_info_table[e->type];
}

