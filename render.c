
static gl_shader_t  shader;
static u32          pvm_location;

static void render_init(void) {
    sr_init();
    sr_init_bitmap();

    shader = sr_basic_shader;

    gl_shader_use(shader);
    pvm_location = gl_shader_location(shader, "pvm");

}

static void render_map(game_state_t* gs) {
    // render tiles:
    defer(sr_begin(GL_TRIANGLES, shader), sr_end()) {
        for_map(x, y) {
            if (v2_dist_sq(gs->cam.pos.xy, v2(x + 0.5, y + 0.5)) > 32 * 32) {
                continue;
            }

            tile_t* tile = &gs->map.tiles[y][x];

            u32 r = hash_u32(y * MAP_SIZE + x);
            u8  c = rand_i32(&r, 30, 34);

            u32 color = pack_color_u8(c, c, c, 255);

            if (tile->type) {
                color = tile_info_table[tile->type].color;
            }

            sr_color(color);

            sr_vertex(x + 0, y + 0, 0);
            sr_vertex(x + 1, y + 0, 0);
            sr_vertex(x + 1, y + 1, 0);

            sr_vertex(x + 1, y + 1, 0);
            sr_vertex(x + 0, y + 1, 0);
            sr_vertex(x + 0, y + 0, 0);

            if (tile->order) {
                sr_color(0x11eeeeee);

                sr_vertex(x + 0, y + 0, 0.01);
                sr_vertex(x + 1, y + 0, 0.01);
                sr_vertex(x + 1, y + 1, 0.01);

                sr_vertex(x + 1, y + 1, 0.01);
                sr_vertex(x + 0, y + 1, 0.01);
                sr_vertex(x + 0, y + 0, 0.01);
            }
        }
    }

    // render grid:
    defer(sr_begin(GL_LINES, shader), sr_end()) {
        sr_color(0x77000000);

        for (i32 i = 0; i < MAP_SIZE; ++i) {
            sr_vertex(0,        i,          0.01);
            sr_vertex(MAP_SIZE, i,          0.01);
            sr_vertex(i,        0,          0.01);
            sr_vertex(i,        MAP_SIZE,   0.01);
        }
    }
}

static void render_entities(game_state_t* gs) {
    defer(sr_begin(GL_TRIANGLES, shader), sr_end()) {
        for (u32 i = 0; i < gs->entity_count; ++i) {
            entity_t* e = &gs->entity_array[i];
            const entity_info_t* info = entity_get_info(e);

            sr_color(info->color);

            sr_vertex(e->pos.x - info->rad, e->pos.y - info->rad, 0.02);
            sr_vertex(e->pos.x + info->rad, e->pos.y - info->rad, 0.02);
            sr_vertex(e->pos.x + info->rad, e->pos.y + info->rad, 0.02);

            sr_vertex(e->pos.x + info->rad, e->pos.y + info->rad, 0.02);
            sr_vertex(e->pos.x - info->rad, e->pos.y + info->rad, 0.02);
            sr_vertex(e->pos.x - info->rad, e->pos.y - info->rad, 0.02);
        }
    }
}

static void render_game(game_state_t* gs) {
    sr_begin_frame();

    gl_shader_use(shader);

    camera_t* cam = &gs->cam;

    mat4_t projection = m4_perspective(0.5 * PI, platform.aspect_ratio, 0.1, 32.0f);
    mat4_t view       = m4_look_at(cam->pos, v3(.xy = cam->pos.xy), v3(0, 1, 0));
    mat4_t pvm        = m4_mul(projection, view);

    gl_uniform_m4(pvm_location, pvm);

    render_map(gs);
    render_entities(gs);

    defer(sr_begin(GL_TRIANGLES, shader), sr_end()) {
        vec3_t pos = mouse_position;

        pos.x = floorf(pos.x);
        pos.y = floorf(pos.y);

        sr_color(0x33ffffff);

        sr_vertex(pos.x - 0, pos.y - 0, 0.021);
        sr_vertex(pos.x + 1, pos.y - 0, 0.021);
        sr_vertex(pos.x + 1, pos.y + 1, 0.021);

        sr_vertex(pos.x + 1, pos.y + 1, 0.021);
        sr_vertex(pos.x - 0, pos.y + 1, 0.021);
        sr_vertex(pos.x - 0, pos.y - 0, 0.021);
    }

    {
        defer(sr_begin(GL_TRIANGLES, sr_ui_text_shader), sr_end()) {
            sr_render_string_format(32, 32, 0, 16, 16, 0xffbbbbbb, order_info_table[gs->order_tool].name);
        }
    }

    sr_end_frame();

    mouse_position = gl_get_world_position(platform.mouse.pos.x, platform.mouse.pos.y, projection, view);
}

