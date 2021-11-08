
typedef u16 order_type_t;
enum {
    ORDER_TYPE_NONE,
    ORDER_TYPE_DESTROY_TILE,
    ORDER_TYPE_BUILD_ROCK_WALL,
    ORDER_TYPE_COUNT,
};

typedef struct order_info_t {
    const char* name;
    b32         on_ground;

    i32         cost;
} order_info_t;

static order_info_t order_info_table[ORDER_TYPE_COUNT] = {
    [ORDER_TYPE_DESTROY_TILE] = {
        .name = "destroy tile",
    },

    [ORDER_TYPE_BUILD_ROCK_WALL] = {
        .name       = "build rock wall",
        .cost       = 50,
    },
};

