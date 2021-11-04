
#define ENTITY_MAX      (2 * 1024)
#define PARTICLE_MAX    (8 * 1024)

struct GameState {
    Camera      cam;
    Map         map;

    u32         next_id;
    u32         entity_count;
    Entity      entity_array[ENTITY_MAX];

    u32         particle_count;
    Particle    particle_array[PARTICLE_MAX];

    inline void init() {
        next_id         = 0;

        entity_count    = 0;
        particle_count  = 0;
    }

    inline Entity* newEntity(EntityType type, Vec2 pos, Vec2 vel = {}) {
        if (entity_count >= ENTITY_MAX) return NULL;
        Entity* e = &entity_array[entity_count++];

        e->type = type;
        e->id   = ++next_id;
        e->pos  = pos;
        e->vel  = vel;
        e->ai   = entity_info_table[type].ai;

        return e;
    }
};

