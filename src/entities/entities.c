#include <include/ash/entities.h>

static uint32_t g_entityFreelist[ASH_MAX_ENTITIES];
static uint32_t g_entityFreelistCount;
static uint32_t g_entityGenerations[ASH_MAX_ENTITIES];
static uint8_t g_entityAlive[ASH_MAX_ENTITIES];

static ASH_Entity invalidEntity = {
    .index = ASH_INVALID_ENTITY_INDEX,
    .generation = 0};

ASH_Entity ASH_CreateEntity(void)
{
    if (g_entityFreelistCount == 0)
    {
        return invalidEntity;
    }
    uint32_t index = g_entityFreelist[--g_entityFreelistCount];

    ASH_Entity current_entity = {
        .index = index,
        .generation = g_entityGenerations[index]};

    g_entityAlive[index] = 1;
    return current_entity;
}

void ASH_DestroyEntity(ASH_Entity entity)
{
    if (!ASH_IsEntityValid(entity))
        return;

    g_entityFreelist[g_entityFreelistCount++] = entity.index;
    g_entityGenerations[entity.index]++;
    g_entityAlive[entity.index] = 0;
}

uint8_t ASH_IsEntityValid(ASH_Entity entity)
{
    if (entity.index >= ASH_MAX_ENTITIES)
        return 0;

    uint8_t isAlive = g_entityAlive[entity.index];
    uint8_t generationValid = g_entityGenerations[entity.index] == entity.generation;

    if (isAlive == 0 || generationValid == 0)
        return 0;
    return 1;
}
void ASH_InitEntityPool(void)
{
    g_entityFreelistCount = ASH_MAX_ENTITIES;
    for (uint32_t i = 0; i < ASH_MAX_ENTITIES; i++)
    {
        g_entityFreelist[i] = i;
        g_entityAlive[i] = 0;
        g_entityGenerations[i] = 0;
    }
}