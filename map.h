
#define MAP_SIZE (256)

#define for_map(x, y) \
    for (i32 y = 0; y < MAP_SIZE; ++y) \
    for (i32 x = 0; x < MAP_SIZE; ++x) \

enum tile_type_t : u16 {
    TILE_TYPE_ROCK,
    TILE_TYPE_DIRT,
    TILE_TYPE_ROCK_WALL,
    TILE_TYPE_COUNT,
};

enum order_type_t : u16 {
    ORDER_TYPE_NONE,
    ORDER_TYPE_DESTROY_TILE,
    ORDER_TYPE_BUILD_ROCK_WALL,
    ORDER_TYPE_COUNT,
};

struct tile_info_t {
    u32 is_wall : 1;
    u32 : 0;

    u32 color;
    f32 max_life;

    tile_type_t destroy_tile;
};

static tile_info_t tile_info_table[TILE_TYPE_COUNT];

struct tile_t {
    tile_type_t     type;
    order_type_t    order;
    u32             worker_id;
    f32             life;
};

#define OFF_MAP(x, y) ((x) < 0 || (x) >= MAP_SIZE || (y) < 0 || (y) >= MAP_SIZE)

struct map_t {
    tile_t tiles[MAP_SIZE][MAP_SIZE];
    
    inline tile_t* get_tile(i32 x, i32 y) {
        if (OFF_MAP(x, y)) return NULL;
        return &tiles[y][x];
    }

    inline const tile_t* get_tile(i32 x, i32 y) const {
        if (OFF_MAP(x, y)) return NULL;
        return &tiles[y][x];
    }

    inline b32 is_traversable(i32 x, i32 y) const {
        auto tile = get_tile(x, y);
        return tile && (!tile_info_table[tile->type].is_wall);
    }
};

static void init_tile_info_table(void) {
    for (u32 i = 0; i < TILE_TYPE_COUNT; ++i) {
        auto info = &tile_info_table[i];

        info->destroy_tile = TILE_TYPE_DIRT;
    }

    {
        auto info = &tile_info_table[TILE_TYPE_DIRT];
        info->color = 0xff334566;
    }

    {
        auto info = &tile_info_table[TILE_TYPE_ROCK];
        info->is_wall = true;
    }

    {
        auto info = &tile_info_table[TILE_TYPE_ROCK_WALL];
        info->is_wall = true;
        info->color   = 0xff555555;
    }

}

static void init_tile(tile_t* tile, tile_type_t type) {
    auto info = &tile_info_table[type];
    
    tile->type  = type;
    tile->order = ORDER_TYPE_NONE;
    tile->life  = info->max_life;
}

static void destroy_tile(tile_t* tile) {
    init_tile(tile, tile_info_table[tile->type].destroy_tile);
}

