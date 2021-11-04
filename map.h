
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
    u32     color;
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

static TileInfo tile_info_table[TileType_Count];

static void initTileInfoTable(void) {
    {
        TileInfo* info = &tile_info_table[TileType_Dirt];
        info->color = 0xff334566;
    }
}

