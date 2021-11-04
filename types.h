
#define MAP_SIZE (256)

#define for_map(x, y) \
    for (i32 y = 0; y < MAP_SIZE; ++y) \
    for (i32 x = 0; x < MAP_SIZE; ++x) \

enum TileType : u32 {
    TileType_None,
    TileType_Dirt,
    TileType_Count,
};

struct Tile {
    TileType type;
};

struct Map {
    Tile tiles[MAP_SIZE][MAP_SIZE];
    
    inline Tile* getTile(i32 x, i32 y) {
        if (x < 0 || x >= MAP_SIZE) return NULL;
        if (y < 0 || y >= MAP_SIZE) return NULL;

        return &tiles[y][x];
    }
};

enum EntityType : u32 {
    EntityType_None,
    EntityType_Worker,
    EntityType_Guard,
    EntityType_Count,
};

struct Entity {
    EntityType  type;
    u32         id;

    Vec2        pos;
    Vec2        vel;

    f32         rad;
};

struct Particle {
    Vec3    pos;
    Vec3    vel;

    f32     rad;

    u32     start_color;
    u32     end_color;
};

struct Camera {
    Vec3    pos;
};

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
        e->rad  = 0.2;

        return e;
    }
};

