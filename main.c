#define ATS_IMPL
#define ATS_OGL33
#include "../ats/ats.h"
#include "../ats/ats_platform.h"
#include "../ats/ats_sr.h"

#define TEXTURE_BORDER (0.02)
#include "../ats/ats_texture_table.h"

#define CUTE_C2_IMPLEMENTATION
#include "../ats/ext/cute_c2.h"

static u32 rs = 0xdeadbeef;

#include "order.h"
#include "map.h"
#include "entity.h"
#include "particle.h"
#include "camera.h"
#include "game_state.h"

#include "path_finder.h"

static memory_arena_t   ma              = {0};
static game_state_t*    game_state      = NULL;
static mat4_t           projection      = {0};
static mat4_t           view            = {0};
static mat4_t           pvm             = {0};
static frustum_t        frustum         = {0};
static vec3_t           mouse_position  = { 0.5 * MAP_SIZE, 0.5 * MAP_SIZE };

#include "init.c"
#include "update.c"
#include "render.c"

static u8 memory[GB];

int main(void) {
    ma              = ma_create(memory, ARRAY_COUNT(memory));
    game_state      = ma_type(&ma, game_state_t);

    platform_init("Game Off 2021", 1200, 800, 0);
    render_init();

    game_state_t* gs = game_state;
    init_game(gs);

    while (!platform.close) {
        f32 dt = platform.time.delta;

        if (platform.keyboard.pressed[KEY_ESCAPE])  { platform.close = true; }
        if (platform.keyboard.pressed[KEY_F1])      { platform.fullscreen = !platform.fullscreen; }

        if (platform.keyboard.pressed[KEY_T]) {
            printf("%u\n", gs->order_tool);
        }

        update_game(gs, dt);

        defer (sr_begin_frame(), sr_end_frame()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            render_game(gs);
        }

        mouse_position = gl_get_world_position(platform.mouse.pos.x, platform.mouse.pos.y, projection, view);
        platform_update();
    }
}

