#define ATS_IMPL
#include "ats/ats.h"
#include "ats/ats_platform.h"

int main(void) {
    initPlatform("Game Off 2021", 800, 600, 8);

    const char* vs = GLSL_SHADER(
        const vec4[] position = vec4[](
            vec4(0, 1, 0, 1),
            vec4(-1, -1, 0, 1),
            vec4(1, -1, 0, 1));

        void main() {
            gl_Position = position[gl_VertexID];
        });

    const char* fs = GLSL_SHADER(
        out vec4 out_color;
        void main() {
            out_color = vec4(1, 0, 1, 1);
        });

    u32 program = gl_createShader(vs, fs);

    u32 vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    while (!platform.close) {
        f32 dt = platform.time.delta;

        if (platform.keyboard.pressed[KEY_ESCAPE]) { platform.close = true; }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        updatePlatform();
    }
}

