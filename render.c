
static texture_table_t  texture_table = {0};
static gl_texture_t     texture_atlas = {0};

typedef struct light_t {
    vec3_t      pos;
    f32         range;
    f32         value;

    u32         color;
} light_t;

static u32      light_count;
static light_t  light_array[ENTITY_MAX];

static void add_light(const light_t* light) {
    if (frustum_intersect_sphere(frustum, (sphere_t) { .pos = light->pos, .rad = light->range })) {
        light_array[light_count++] = *light;
    }
}

static void enable_lights(void) {
    u32 count = MIN(light_count, 16);
    for (u32 i = 0; i < count; ++i) {
        const light_t* light = &light_array[i];

        vec3_t color = v3_from_packed_color(light->color);

        sr_set_light(i, &(sr_point_light_t) {
            .range      = light->range,
            .pos        = light->pos,
            .ambient    = color,
            .diffuse    = color,
            .specular   = color,
            .constant   = light->value,
            .linear     = light->value,
            .quadratic  = light->value,
        });
    }

    light_count = 0;
}

static void render_init(void) {
    texture_table = tt_load_from_dir("assets/textures/", &ma);
    texture_atlas = gl_texture_create(texture_table.image.pixels, texture_table.image.width, texture_table.image.height, false);

    sr_init();
    sr_init_bitmap();

    sr_set_texture(texture_atlas);
}

static void render_map(game_state_t* gs) {
    // render tiles:
    defer(sr_begin(GL_TRIANGLES, sr_shader), sr_end()) {
        for_map(x, y) {
            if (v2_dist_sq(gs->cam.pos.xy, v2(x + 0.5, y + 0.5)) > 32 * 32) {
                continue;
            }

            tile_t* tile = &gs->map.tiles[y][x];
            const tile_info_t* info = tile_get_info(tile);

            rect2_t tex_rect = tt_get(&texture_table, info->texture);

            if (info->is_wall) {
                sr_texture_box(tex_rect, (rect3_t) { x, y, 0, x + 1, y + 1, 1 }, info->color);
            } else {
                sr_texture_rect(tex_rect, (rect2_t) { x, y, x + 1, y + 1 }, 0, info->color);
            }

            if (tile->order) {
                sr_texture_rect(tex_rect, (rect2_t) { x, y, x + 1, y + 1 }, info->is_wall + 0.02, 0xff777777);
            }
        }
    }

    // render grid:
    defer(sr_begin(GL_LINES, sr_basic_shader), sr_end()) {
        sr_color(0x77000000);

        for (i32 i = 0; i < MAP_SIZE; ++i) {
            sr_vertex(0,        i,          0.01);
            sr_vertex(MAP_SIZE, i,          0.01);
            sr_vertex(i,        0,          0.01);
            sr_vertex(i,        MAP_SIZE,   0.01);

            sr_vertex(0,        i,          1.01);
            sr_vertex(MAP_SIZE, i,          1.01);
            sr_vertex(i,        0,          1.01);
            sr_vertex(i,        MAP_SIZE,   1.01);
        }
    }
}

static void render_entities(game_state_t* gs) {
    defer(sr_begin(GL_TRIANGLES, sr_texture_shader), sr_end()) {
        for (u32 i = 0; i < gs->entity_count; ++i) {
            entity_t*               e           = &gs->entity_array[i];
            const entity_info_t*    info        = entity_get_info(e);
            rect2_t                 tex_rect    = tt_get(&texture_table, info->texture);
            rect2_t                 rect        = entity_get_rect(e);

            add_light(&(light_t) {
                .pos        = v3(e->pos.x, e->pos.y, 1.5),
                .range      = 8,
                .value      = 1.0,
                .color      = pack_color_f32(1, 0.8, 0.4, 1),
            });

            sr_texture_rect(tex_rect, rect2(rect.min.x + 0.05, rect.min.y + 0.05, rect.max.x + 0.05, rect.max.y + 0.05), 0.019, 0xbb000000);
            sr_texture_rect(tex_rect, rect, 0.020, info->color);
        }
    }
}

static void render_game(game_state_t* gs) {
    camera_t* cam = &gs->cam;

    projection = m4_perspective(0.5 * PI, platform.aspect_ratio, 0.1, 32.0f);
    view       = m4_look_at(cam->pos, v3(.xy = cam->pos.xy), v3(0, 1, 0));
    pvm        = m4_mul(projection, view);
    frustum    = frustum_create(pvm, false);

    gl_shader_use(sr_basic_shader);
    gl_uniform_m4(gl_shader_location(sr_basic_shader, "pvm"), pvm);

    gl_shader_use(sr_texture_shader);
    gl_uniform_m4(gl_shader_location(sr_texture_shader, "pvm"), pvm);

    gl_shader_use(sr_shader);
    gl_uniform_m4(gl_shader_location(sr_shader, "pvm"), pvm);

    {
        add_light(&(light_t) {
            .pos        = v3(cam->pos.x, cam->pos.y, 4.0),
            .range      = 32,
            .value      = 0.4,
            .color      = pack_color_f32(1, 0.8, 0.4, 1),
        });


        render_map(gs);
        render_entities(gs);

        enable_lights();
    }

    defer(sr_begin(GL_TRIANGLES, sr_basic_shader), sr_end()) {
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

