
static const char* vertex_shader = GLSL_SHADER(
    layout (location = 0) in vec3 in_position;
    layout (location = 1) in vec4 in_color;

    out vec4 frag_color;

    uniform mat4 pvm;

    void main() {
        frag_color = in_color;
        gl_Position = pvm * vec4(in_position, 1);
    });

static const char* fragment_shader = GLSL_SHADER(
    in vec4 frag_color;
    out vec4 out_color;

    void main() {
        out_color = frag_color;
    });

struct GL_Vertex {
    Vec3    pos;
    u32     color;
};

struct GL_Range {
    u32     type;
    u32     flags;
    u32     index;
    u32     count;
};

#define GL_STATE_VERTEX_MAX (1024 * 1024)

static struct GL_State {
    u32         shader;

    u32         pvm;

    u32         vao;
    u32         vbo;

    GL_Range    range;

    u32         vertex_count;
    GL_Vertex   vertex_array[GL_STATE_VERTEX_MAX];

    u32         range_count;
    GL_Range    range_array[GL_STATE_VERTEX_MAX];

    void init(void) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        glClearDepth(1.0f);

        glDepthFunc(GL_LESS);

        glEnable(GL_DEPTH_TEST);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        shader = gl_createShader(vertex_shader, fragment_shader);
        pvm    = glGetUniformLocation(shader, "pvm");

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (GL_Vertex), (void*)offsetof(GL_Vertex, pos));
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof (GL_Vertex), (void*)offsetof(GL_Vertex, color));
    }

    inline void begin(u32 type) {
        range.type   = type;
        range.index  = vertex_count;
        range.count  = 0;
    }

    inline void end(void) {
        range.count = vertex_count - range.index;

        if (range.count > 0) {
            range_array[range_count++] = range;
        }

        range = {};
    }

    inline void vertex(Vec3 pos, u32 color) {
        GL_Vertex vertex = {};

        vertex.pos      = pos;
        vertex.color    = color;

        vertex_array[vertex_count++] = vertex;
    }

    void render(void) {
        glBufferData(GL_ARRAY_BUFFER, sizeof (vertex_array), vertex_array, GL_STATIC_DRAW);

        for (u32 i = 0; i < range_count; ++i) {
            const GL_Range* range = &range_array[i];
            glDrawArrays(range->type, range->index, range->count);
        }

        vertex_count = 0;
        range_count  = 0;
    }
} gl_state;

static void renderMap(GameState* gs) {
    // render tiles:
    defer (gl_state.begin(GL_TRIANGLES), gl_state.end()) {
        for_map(x, y) {
            if (v2DistSq(gs->cam.pos.xy, v2(x + 0.5, y + 0.5)) > 32 * 32) {
                continue;
            }

            Tile* tile = &gs->map.tiles[y][x];

            u32 r = hashU32(y * MAP_SIZE + x);
            u8  c = randI32(&r, 30, 34);

            u32 color = packColorU8(c, c, c, 255);

            switch (tile->type) {
                case TileType_Dirt: {
                    color = 0xff334566;
                } break;
            }

            gl_state.vertex(v3(x + 0, y + 0, 0), color);
            gl_state.vertex(v3(x + 1, y + 0, 0), color);
            gl_state.vertex(v3(x + 1, y + 1, 0), color);

            gl_state.vertex(v3(x + 1, y + 1, 0), color);
            gl_state.vertex(v3(x + 0, y + 1, 0), color);
            gl_state.vertex(v3(x + 0, y + 0, 0), color);
        }
    }

    // render grid:
    defer (gl_state.begin(GL_LINES), gl_state.end()) {
        for (i32 i = 0; i < MAP_SIZE; ++i) {
            gl_state.vertex(v3(0, i, 0.01), 0x77000000);
            gl_state.vertex(v3(MAP_SIZE, i, 0.01), 0x77000000);
            gl_state.vertex(v3(i, 0, 0.01), 0x77000000);
            gl_state.vertex(v3(i, MAP_SIZE, 0.01), 0x77000000);
        }
    }
}

static void renderEntities(GameState* gs) {
    defer (gl_state.begin(GL_TRIANGLES), gl_state.end()) {
        for (u32 i = 0; i < gs->entity_count; ++i) {
            Entity* e = &gs->entity_array[i];
            
            u32 color = 0xffffffff;

            switch (e->type) {
                case EntityType_Worker: {
                    color = 0xff22bb22;
                } break;
                case EntityType_Guard: {
                    color = 0xffbb4422;
                } break;
            }

            gl_state.vertex(v3(e->pos.x - e->rad, e->pos.y - e->rad, 0.02), color);
            gl_state.vertex(v3(e->pos.x + e->rad, e->pos.y - e->rad, 0.02), color);
            gl_state.vertex(v3(e->pos.x + e->rad, e->pos.y + e->rad, 0.02), color);

            gl_state.vertex(v3(e->pos.x + e->rad, e->pos.y + e->rad, 0.02), color);
            gl_state.vertex(v3(e->pos.x - e->rad, e->pos.y + e->rad, 0.02), color);
            gl_state.vertex(v3(e->pos.x - e->rad, e->pos.y - e->rad, 0.02), color);
        }
    }
}

static void renderGame(GameState* gs) {
    glUseProgram(gl_state.shader);

    Camera* cam = &gs->cam;

    Mat4 projection = m4Perspective(0.5 * PI, platform.aspect_ratio, 0.1, 32.0f);
    Mat4 view       = m4LookAt(cam->pos, v3(cam->pos.xy), v3(0, 1, 0));
    Mat4 pvm        = projection * view;

    glUniformMatrix4fv(gl_state.pvm, 1, GL_FALSE, &pvm.x.x);

    renderMap(gs);
    renderEntities(gs);

    gl_state.render();
}

