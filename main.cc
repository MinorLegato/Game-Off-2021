#define ATS_IMPL
#include "ats/ats.h"
#include "ats/ats_platform.h"

#include "types.h"

#include "init.cc"
#include "update.cc"
#include "render.cc"

static const char* vs = GLSL_SHADER(
    const vec4[] position = vec4[](
        vec4( 0,  1, 0, 1),
        vec4(-1, -1, 0, 1),
        vec4( 1, -1, 0, 1));

    void main() {
        gl_Position = position[gl_VertexID];
    });

static const char* fs = GLSL_SHADER(
    out vec4 out_color;

    void main() {
        out_color = vec4(1, 0, 1, 1);
    });

static GameState game_state;

int main(void) {
    initPlatform("Game Off 2021", 800, 600, 8);

    u32 program = gl_createShader(vs, fs);

    u32 vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GameState* gs = &game_state;
    initGame(gs);

    while (!platform.close) {
        f32 dt = platform.time.delta;

        if (platform.keyboard.pressed[KEY_ESCAPE]) { platform.close = true; }

        updateGame(gs, dt);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        renderGame(gs);

        updatePlatform();
    }
}

