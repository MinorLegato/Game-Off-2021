
enum EntityType : u32 {
    EntityType_None,
    // units:
    EntityType_Worker,
    EntityType_Guard,
    // enemy units:
    EntityType_Ant,
    // buildings/commands:
    EntityType_DestroyTile,
    //
    EntityType_Count,
};

enum AI_Type : u32 {
    AI_None,
    // Worker AI:
    AI_WorkerIdle,
    // Guard AI:
    AI_GuardIdle,
    //
    AI_Count,
};

struct EntityInfo {
    AI_Type     ai;
    f32         rad;
    u32         color;
    f32         max_life;
};

static EntityInfo entity_info_table[EntityType_Count];

struct Entity {
    EntityType  type;
    u32         id;

    Vec2        pos;
    Vec2        vel;
    f32         life;

    AI_Type     ai;

    const EntityInfo* getInfo() const {
        return &entity_info_table[type];
    }
};

static void initEntityInfoTable(void) {
    {
        EntityInfo* info = &entity_info_table[EntityType_None];

        info->rad   = 0.3;
        info->color = 0xff22bb22;
    }

    {
        EntityInfo* info = &entity_info_table[EntityType_Worker];

        info->ai        = AI_WorkerIdle;
        info->rad       = 0.18;
        info->color     = 0xff22bb22;
        info->max_life  = 1.0;
    }

    {
        EntityInfo* info = &entity_info_table[EntityType_Guard];

        info->ai        = AI_GuardIdle;
        info->rad       = 0.2;
        info->color     = 0xffbb4422;
        info->max_life  = 2.0;
    }

    {
        EntityInfo* info = &entity_info_table[EntityType_DestroyTile];

        info->rad       = 0.5;
        info->color     = 0x77777777;
        info->max_life  = 1.0;
    }

}

