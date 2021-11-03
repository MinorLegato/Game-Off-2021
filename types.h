
#define MAP_SIZE (256)

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
    EntityType_Warrior,
    EntityType_Count,
};

struct Entity {
    EntityType  type;
    u32         id;

    Vec2        pos;
    Vec2        vel;
};

struct Camera {
    Vec3    pos;
};

#define ENTITY_MAX (2048)

struct GameState {
    Camera      cam;
    Map         map;

    u32         next_id;
    u32         entity_count;
    Entity      entity_array[ENTITY_MAX];

    inline void init() {
        next_id         = 0;

        entity_count    = 0;
        particle_count  = 0;
    }
};

