
#define MAP_SIZE (256)

#define for_map(x, y) \
    for (i32 y = 0; y < MAP_SIZE; ++y) \
    for (i32 x = 0; x < MAP_SIZE; ++x) \

enum TileType : u16 {
    TileType_Rock,
    TileType_Dirt,
    TileType_RockWall,
    TileType_Count,
};

enum OrderType : u16 {
    OrderType_None,
    OrderType_DestroyTile,
    OrderType_BuildRockWall,
    OrderType_Count,
};

struct TileInfo {
    u32         is_wall : 1;
    u32         : 0;

    u32         color;
    f32         max_life;

    TileType    destroy_tile;
};

static TileInfo tile_info_table[TileType_Count];

struct Tile {
    TileType    type;

    OrderType   order;
    u32         worker_id;
    
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
        return tile && (!tile_info_table[tile->type].is_wall);
    }
};

static void initTileInfoTable(void) {
    for (u32 i = 0; i < TileType_Count; ++i) {
        TileInfo* info = &tile_info_table[i];

        info->destroy_tile = TileType_Dirt;
    }

    {
        TileInfo* info  = &tile_info_table[TileType_Dirt];
        info->color     = 0xff334566;
    }

    {
        TileInfo* info  = &tile_info_table[TileType_Rock];
        info->is_wall   = true;
    }

    {
        TileInfo* info  = &tile_info_table[TileType_RockWall];
        info->is_wall   = true;
        info->color     = 0xff555555;
    }

}

static void initTile(Tile* tile, TileType type) {
    TileInfo* info = &tile_info_table[type];
    
    tile->type  = type;
    tile->order = OrderType_None;
    tile->life  = info->max_life;
}

static void destroyTile(Tile* tile) {
    initTile(tile, tile_info_table[tile->type].destroy_tile);
}

