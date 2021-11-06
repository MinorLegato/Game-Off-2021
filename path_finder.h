
static const vec2i_t path_dirs[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

u32     path_begin;
u32     path_end;

vec2i_t path_queue   [MAP_SIZE * MAP_SIZE];
b8      path_visited [MAP_SIZE][MAP_SIZE];

static void path_init(vec2i_t pos, map_t* map) {
    path_begin   = 0;
    path_end     = 0;

    for_map(x, y) {
        path_visited[y][x] = !map_is_traversable(map, x, y);
    }

    path_queue[path_end++] = pos;
    path_visited[pos.y][pos.x] = true;
}

static b32 path_empty(void) {
    return path_begin >= path_end;
}

static void path_push(vec2i_t pos) {
    if (!path_visited[pos.y][pos.x]) {
        path_visited[pos.y][pos.x]  = true;
        path_queue[path_end++]      = pos;
    }
}

static vec2i_t path_pop(void) {
    return path_queue[path_begin++];
}

