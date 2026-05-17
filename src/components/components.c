#include <ash/components.h>
#include <stdlib.h>

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
}