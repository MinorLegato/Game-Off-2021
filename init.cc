
static void generateMap(Map* map) {
    for_map(x, y) {
        TileType tile_type = TileType_Rock;

        if (randI32(&rs, 0, 100) < 10 || v2DistSq(v2(x + 0.5, y + 0.5), 0.5 * v2(MAP_SIZE, MAP_SIZE)) < 3 * 3) {
            tile_type = TileType_Dirt;
        }

        initTile(&map->tiles[y][x], tile_type);
    }
}

static void initGame(GameState* gs) {
    gs->init();
    gs->cam.pos = v3(0.5 * MAP_SIZE, 0.5 * MAP_SIZE, 8);

    generateMap(&gs->map);

    for (u32 i = 0; i < 3; ++i) {
        gs->newEntity(EntityType_Worker, 0.5 * v2(MAP_SIZE, MAP_SIZE) + randVec2(&rs, -3, 3));
    }

    for (u32 i = 0; i < 2; ++i) {
        gs->newEntity(EntityType_Guard, 0.5 * v2(MAP_SIZE, MAP_SIZE) + randVec2(&rs, -3, 3));
    }

    mouse_position = v3(gs->cam.pos.xy);
}

