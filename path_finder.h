
static const vec2i_t path_dirs[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

static u32 path_id;
static u32 path_begin;
static u32 path_end;

static vec2i_t path_queue[MAP_SIZE * MAP_SIZE];
static u32 path_visited[MAP_SIZE][MAP_SIZE];

static void path_init(vec2i_t pos) {
    ++path_id;

    path_begin                  = 0;
    path_end                    = 0;
    path_queue[path_end++]      = pos;
    path_visited[pos.y][pos.x]  = path_id;
}

static b32 path_empty(void) {
    return path_begin >= path_end;
}

static void path_push(vec2i_t pos, const map_t* map) {
    if ((path_visited[pos.y][pos.x] == path_id) || !map_is_traversable(map, pos.x, pos.y)) return;

    path_visited[pos.y][pos.x]  = path_id;
    path_queue[path_end++]      = pos;
}

static vec2i_t path_pop(void) {
    return path_queue[path_begin++];
}

static vec2_t path_get_direction_towards(vec2_t start_position, vec2_t target_position, const map_t* map) {
    vec2i_t start_tile  = v2_cast(vec2i_t, start_position);
    vec2i_t target_tile = v2_cast(vec2i_t, target_position);

    path_init(target_tile);

    while (!path_empty()) {
        vec2i_t current = path_pop();

        for (u32 i = 0; i < ARRAY_COUNT(path_dirs); ++i) {
            vec2i_t next = v2i_add(current, path_dirs[i]);

            if (next.x == start_tile.x && next.y == start_tile.y) {
                return v2_norm(v2_sub(v2(current.x + 0.5, current.y + 0.5), start_position));
            }

            path_push(next, map);
        }
    }

    return v2(0);
}

