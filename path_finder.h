
static const vec2i_t path_dirs[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

static struct path_finder_t {
    u32     begin;
    u32     end;

    vec2i_t queue   [MAP_SIZE * MAP_SIZE];
    b8      visited [MAP_SIZE][MAP_SIZE];

    inline void init(vec2i_t pos, const map_t* map) {
        begin   = 0;
        end     = 0;

        for_map(x, y) {
            visited[y][x] = !map->is_traversable(x, y);
        }

        queue[end++] = pos;
        visited[pos.y][pos.x] = true;
    }

    inline b32 empty(void) const {
        return begin >= end;
    }

    inline void push(vec2i_t pos) {
        if (!visited[pos.y][pos.x]) {
            visited[pos.y][pos.x] = true;
            queue[end++] = pos;
        }
    }

    inline vec2i_t pop(void) {
        return queue[begin++];
    }
} path_finder;

