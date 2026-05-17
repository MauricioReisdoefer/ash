#include <include/ash/entities.h>

static uint32_t g_entityFreelist[ASH_MAX_ENTITIES];
static uint32_t g_entityFreelistCount;
static uint32_t g_entityGenerations[ASH_MAX_ENTITIES];
static uint8_t g_entityAlive[ASH_MAX_ENTITIES];

ASH_Entity ASH_CreateEntity(void)
{
    uint32_t index = g_entityFreelist[--g_entityFreelistCount];

    ASH_Entity current_entity = {
        .index = index,
        .generation = g_entityGenerations[index]};

    g_entityAlive[index] = 1;
    return current_entity;
}

void ASH_DestroyEntity(ASH_Entity entity)
{
    g_entityFreelist[g_entityFreelistCount++] = entity.index;
    g_entityGenerations[entity.index]++;
    g_entityAlive[entity.index] = 0;
}

uint8_t ASH_IsEntityValid(ASH_Entity entity);
void ASH_InitEntityPool(void);