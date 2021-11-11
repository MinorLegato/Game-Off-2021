
#define MAP_SIZE (256)

#define for_map(x, y) \
    for (i32 y = 0; y < MAP_SIZE; ++y) \
    for (i32 x = 0; x < MAP_SIZE; ++x) \

typedef u16 tile_type_t;
enum {
    TILE_TYPE_DIRT,
    TILE_TYPE_ROCK,
    TILE_TYPE_COPPER,
    // 
    TILE_TYPE_ROCK_WALL,
    //
    TILE_TYPE_COUNT,
};

typedef struct tile_info_t {
    u32         is_wall : 1;
    u32         : 0;

    u32         color;
    f32         max_life;

    tile_type_t destroy_tile;

    char        name[32];
    char        texture[32];
} tile_info_t;

static tile_info_t tile_info_table[TILE_TYPE_COUNT] = {
    [TILE_TYPE_DIRT] = {
        .name       = "dirt",
        .texture    = "stone_floor",
        .color      = 0xffbbbbbb,
    },

    [TILE_TYPE_ROCK] = {
        .name       = "rock",
        .texture    = "stone",
        .is_wall    = true,
        .color      = 0xffffffff,
    },

    [TILE_TYPE_COPPER] = {
        .name       = "copper",
        .texture    = "copper",
        .is_wall    = true,
        .color      = 0xffffffff,
    },

    [TILE_TYPE_ROCK_WALL] = {
        .name       = "rock wall",
        .texture    = "stone_floor",
        .is_wall    = true,
        .color      = 0xffffffff,
    },
};

typedef struct tile_t {
    tile_type_t     type;
    order_type_t    order;
    u32             worker_id;
    f32             life;
} tile_t;

static const tile_info_t* tile_get_info(const tile_t* tile) {
    return &tile_info_table[tile->type];
}

static b32 tile_is_traversable(const tile_t* tile) {
    return tile && (!tile_info_table[tile->type].is_wall);
}

#define OFF_MAP(x, y) ((x) < 0 || (x) >= MAP_SIZE || (y) < 0 || (y) >= MAP_SIZE)

typedef struct map_t {
    tile_t      tiles[MAP_SIZE][MAP_SIZE];
} map_t;

static tile_t* map_get_tile(map_t* map, i32 x, i32 y) {
    if (OFF_MAP(x, y)) return NULL;
    return &map->tiles[y][x];
}

static b32 map_is_traversable(const map_t* map, i32 x, i32 y) {
    if (OFF_MAP(x, y)) return false;
    const tile_t* tile = &map->tiles[y][x];

    return tile_is_traversable(tile);
}

static void init_tile(tile_t* tile, tile_type_t type) {
    tile_info_t* info = &tile_info_table[type];
    
    tile->type  = type;
    tile->order = ORDER_TYPE_NONE;
    tile->life  = info->max_life;
}

static void destroy_tile(tile_t* tile) {
    init_tile(tile, tile_info_table[tile->type].destroy_tile);
}

