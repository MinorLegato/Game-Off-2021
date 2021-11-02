#include "ats.h"
#include <stdio.h>

int main(void) {
    initPlatform("Game Off 2021", 800, 600);

    puts((char*)glGetString(GL_VERSION));

    auto pos = v2(0, 0);

    while (!platform.close) {
        f32 dt = platform.time.delta;

        if (platform.keyboard.pressed[KEY_ESCAPE]) { platform.close = true; }

        if (platform.keyboard.down[KEY_W]) pos.y += dt;
        if (platform.keyboard.down[KEY_S]) pos.y -= dt;
        if (platform.keyboard.down[KEY_A]) pos.x -= dt;
        if (platform.keyboard.down[KEY_D]) pos.x += dt;

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(m4Perspective(0.5 * PI, platform.window.aspect_ratio, 0.1, 32).getPtr());
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(m4LookAt(v3(0, 0, 2), v3(0, 0, 0), v3(0, 1, 0)).getPtr());

        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex3f(pos.x + 0, pos.y + 1, 0);
        glColor3f(0, 1, 0); glVertex3f(pos.x - 1, pos.y - 1, 0);
        glColor3f(0, 0, 1); glVertex3f(pos.x + 1, pos.y - 1, 0);
        glEnd();

        updatePlatform();
    }

    exitPlatform();
}

