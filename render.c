
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
            const tile_info_t* info = tile_get_info(tile);

            u32 r = hash_u32(y * MAP_SIZE + x);
            u8  c = rand_i32(&r, 30, 34);

            u32 color = pack_color_u8(c, c, c, 255);

            if (info->color) {
                color = info->color;
            }

            sr_rect((rect2_t) { x, y, x + 1, y + 1 }, 0, color);

            if (tile->order) {
                sr_rect((rect2_t) { x, y, x + 1, y + 1 }, 0.01, 0x11eeeeee);
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
    camera_t* cam = &gs->cam;

    projection = m4_perspective(0.5 * PI, platform.aspect_ratio, 0.1, 32.0f);
    view       = m4_look_at(cam->pos, v3(.xy = cam->pos.xy), v3(0, 1, 0));
    pvm        = m4_mul(projection, view);

    gl_shader_use(shader);
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

    defer(sr_begin(GL_TRIANGLES, sr_ui_text_shader), sr_end()) {
        sr_render_string_format(32, 32, 0, 12, 12, 0xffbbbbbb, order_info_table[gs->order_tool].name);

        tile_t* tile = map_get_tile(&gs->map, mouse_position.x, mouse_position.y);
        if (tile) {
            const tile_info_t* info = tile_get_info(tile);

            if (info->name[0] != '\0') {
                sr_render_string_format(platform.mouse.pos.x + 24, platform.mouse.pos.y + 12, 0, 12, 12, 0xffffffff, info->name);
            }
        }
    }
}

