
static const Vec2i path_dirs[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

static struct PathFinder {
    u32     begin;
    u32     end;

    Vec2i   queue   [MAP_SIZE * MAP_SIZE];
    b8      visited [MAP_SIZE][MAP_SIZE];

    inline void init(Vec2i pos, const Map* map) {
        begin   = 0;
        end     = 0;

        for_map(x, y) {
            visited[y][x] = !map->isTraversable(x, y);
        }

        queue[end++] = pos;
        visited[pos.y][pos.x] = true;
    }

    inline b32 empty(void) const {
        return begin >= end;
    }

    inline void push(Vec2i pos) {
        if (!visited[pos.y][pos.x]) {
            visited[pos.y][pos.x] = true;
            queue[end++] = pos;
        }
    }

    inline Vec2i pop(void) {
        return queue[begin++];
    }
} path_finder;

