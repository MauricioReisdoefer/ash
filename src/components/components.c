#include <ash/components.h>
#include <stdlib.h>
#include <stdio.h>

static ASH_Component g_components[ASH_MAX_COMPONENTS];
static uint32_t g_componentCount;

uint32_t ASH_RegisterComponent(size_t size)
{
    uint32_t id = g_componentCount++;
    ASH_Component *component = &g_components[id];

    component->componentSize = size;
    component->pool = malloc(size * ASH_MAX_ENTITIES);
    component->active = 1;

    for (uint32_t i = 0; i < ASH_MAX_ENTITIES; i++)
    {
        component->entityHasComponent[i] = 0;
    }
    return id;
}

void *ASH_AddComponent(uint32_t componentId, ASH_Entity entity)
{
    if (componentId > ASH_MAX_COMPONENTS)
    {
        printf("ASH_AddComponent - ComponentID Out Of Range");
        return NULL;
    }
    ASH_Component *component = &g_components[componentId];
    if (component->active == 0)
        return NULL;

    void *ptr = (uint8_t *)component->pool + (entity.index * component->componentSize);
    component->entityHasComponent[entity.index] = 1;
    return ptr;
}

uint8_t ASH_HasComponent(uint32_t componentId, ASH_Entity entity)
{
    if (componentId >= g_componentCount)
        return 0;
    ASH_Component *component = &g_components[componentId];
    if (!component->active)
        return 0;
    return component->entityHasComponent[entity.index];
}

void *ASH_GetComponent(uint32_t componentId, ASH_Entity entity)
{
    if (componentId >= g_componentCount)
    {
        printf("ASH_GetComponent - ComponentID Out Of Range\n");
        return NULL;
    }
    ASH_Component *component = &g_components[componentId];
    if (!component->active)
        return NULL;

    if (!component->entityHasComponent[entity.index])
        return NULL;
    return (uint8_t *)component->pool +
           (entity.index * component->componentSize);
}

void ASH_RemoveComponent(uint32_t componentId, ASH_Entity entity)
{
    if (componentId >= g_componentCount)
    {
        printf("ASH_RemoveComponent - ComponentID Out Of Range\n");
        return;
    }
    ASH_Component *component = &g_components[componentId];

    if (!component->active)
        return;
    if (!component->entityHasComponent[entity.index])
        return;
    component->entityHasComponent[entity.index] = 0;
}