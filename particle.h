
typedef struct particle_desc_t {
    u32     count;

    vec3_t  pos;
    vec3_t  vel;

    f32     rad;
    f32     life;
    f32     turbulance;

    vec4_t  start_color;
    vec4_t  end_color;

    struct {
        f32 pos;
        f32 vel;
        f32 rad;
        f32 start_color;
        f32 end_color;
    } rand;
} particle_desc_t;

typedef struct particle_t {
    vec3_t  pos;
    vec3_t  vel;

    f32     rad;
    f32     turbulance;

    f32     life;
    f32     max_life;

    u32     start_color;
    u32     end_color;
} particle_t;

