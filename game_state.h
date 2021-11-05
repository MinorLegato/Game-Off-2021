
#define ENTITY_MAX      (2 * 1024)
#define PARTICLE_MAX    (8 * 1024)

struct GameState {
    Camera      cam;
    Map         map;

    OrderType   order_tool;

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

        Entity*     e       = &entity_array[entity_count++];
        EntityInfo* info    = &entity_info_table[type];

        e->type     = type;
        e->id       = ++next_id;
        e->pos      = pos;
        e->vel      = vel;
        e->life     = info->max_life;
        e->ai       = info->ai;

        return e;
    }

    inline Entity* getEntity(u32 id) {
        for (u32 i = 0; i < entity_count; ++i) {
            Entity* e = &entity_array[i];
            if (id == e->id) {
                return e;
            }
        }

        return NULL;
    }
};

