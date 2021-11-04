#define ATS_IMPL
#include "ats/ats.h"
#include "ats/ats_platform.h"

#include "map.h"
#include "entity.h"
#include "particle.h"
#include "camera.h"
#include "game_state.h"

static u32 rs = 0xdeadbeef;

#include "init.cc"
#include "update.cc"
#include "render.cc"

static GameState game_state;

int main(void) {
    initPlatform("Game Off 2021", 800, 600, 8);
    gl_state.init();

    initEntityInfoTable();
    initTileInfoTable();

    GameState* gs = &game_state;
    initGame(gs);

    while (!platform.close) {
        f32 dt = platform.time.delta;

        if (platform.keyboard.pressed[KEY_ESCAPE]) { platform.close = true; }

        updateGame(gs, dt);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderGame(gs);

        updatePlatform();
    }
}

