
#define MAP_SIZE (256)

#define for_map(x, y) \
    for (i32 y = 0; y < MAP_SIZE; ++y) \
    for (i32 x = 0; x < MAP_SIZE; ++x) \

enum TileType : u32 {
    TileType_None,
    TileType_Dirt,
    TileType_Count,
};

struct TileInfo {
    u32         color;
    TileType    next;

    f32         max_life;
};

static TileInfo tile_info_table[TileType_Count];

struct Tile {
    TileType    type;
    f32         life;
};

#define OFF_MAP(x, y) ((x) < 0 || (x) >= MAP_SIZE || (y) < 0 || (y) >= MAP_SIZE)

struct Map {
    Tile tiles[MAP_SIZE][MAP_SIZE];
    
    inline Tile* getTile(i32 x, i32 y) {
        if (OFF_MAP(x, y)) return NULL;
        return &tiles[y][x];
    }

    inline const Tile* getTile(i32 x, i32 y) const {
        if (OFF_MAP(x, y)) return NULL;
        return &tiles[y][x];
    }

    inline b32 isTraversable(i32 x, i32 y) const {
        const Tile* tile = getTile(x, y);
        return tile && (tile->type != TileType_None);
    }
};

static void initTileInfoTable(void) {
    {
        TileInfo* info = &tile_info_table[TileType_Dirt];
        info->color = 0xff334566;
    }
}

