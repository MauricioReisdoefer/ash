#include <ash/components.h>
#include <ash/internals.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern _ASH_ComponentRegistry g_componentRegistry;

void ASH_InitComponents()
{
    _ASH_InitComponentRegistry();
}

ASH_ComponentID ASH_RegisterComponent(size_t componentSize)
{
    if (g_componentRegistry.count >= g_componentRegistry.capacity)
    {
        printf("[ASH] Component registry is full. Cannot register more components.\n");
        return UINT32_MAX;
    }

    ASH_ComponentID id = g_componentRegistry.count;
    g_componentRegistry.storages[id] = _ASH_CreateComponentStorage(componentSize, ASH_MAX_ENTITIES);
    g_componentRegistry.storages[id].componentSize = componentSize;
    g_componentRegistry.count++;
    return id;
}

void ASH_AddComponent(
    ASH_Entity entity,
    ASH_ComponentID componentID,
    void *componentData)
{
    if (componentID >= g_componentRegistry.count)
    {
        printf("[ASH] Invalid ComponentID: %u\n", componentID);
        return;
    }

    _ASH_ComponentStorage *storage = &g_componentRegistry.storages[componentID];

    if (ASH_HasComponent(entity, componentID))
    {
        void *existingComponent = ASH_GetComponent(entity, componentID);
        memcpy(existingComponent, componentData, storage->componentSize);
        return;
    }

    if (storage->count >= storage->capacity)
    {
        printf("[ASH] Component storage for ID %u is full. Cannot add more components.\n", componentID);
        return;
    }

    uint32_t denseIndex = storage->count;
    uint32_t entityIndex = entity.index;

    memcpy((uint8_t *)storage->dense + denseIndex * storage->componentSize, componentData, storage->componentSize);

    storage->entity[denseIndex] = entityIndex;
    storage->sparse[entityIndex] = denseIndex;

    storage->count++;
}

void ASH_RemoveComponent(
    ASH_Entity entity,
    ASH_ComponentID componentID)
{
    if (componentID >= g_componentRegistry.count)
    {
        printf("[ASH] Invalid ComponentID: %u\n", componentID);
        return;
    }

    _ASH_ComponentStorage *storage = &g_componentRegistry.storages[componentID];

    if (!ASH_HasComponent(entity, componentID))
    {
        return;
    }

    uint32_t removedDenseIndex = storage->sparse[entity.index];
    uint32_t lastDenseIndex = storage->count - 1;

    if (removedDenseIndex != lastDenseIndex)
    {
        uint32_t lastEntityIndex = storage->entity[lastDenseIndex];

        memcpy((uint8_t *)storage->dense + removedDenseIndex * storage->componentSize,
               (uint8_t *)storage->dense + lastDenseIndex * storage->componentSize,
               storage->componentSize);

        storage->entity[removedDenseIndex] = lastEntityIndex;
        storage->sparse[lastEntityIndex] = removedDenseIndex;
    }

    storage->sparse[entity.index] = ASH_INVALID_ENTITY_INDEX;
    storage->entity[lastDenseIndex] = ASH_INVALID_ENTITY_INDEX;

    storage->count--;
}

void *ASH_GetComponent(
    ASH_Entity entity,
    ASH_ComponentID componentID)
{
    if (componentID >= g_componentRegistry.count)
    {
        printf("[ASH] Invalid ComponentID: %u\n", componentID);
        return NULL;
    }

    _ASH_ComponentStorage *storage = &g_componentRegistry.storages[componentID];

    if (!ASH_IsEntityValid(entity) || entity.index >= ASH_MAX_ENTITIES)
    {
        return NULL;
    }

    uint32_t sparseIndex = entity.index;
    if (storage->sparse[sparseIndex] == ASH_INVALID_ENTITY_INDEX)
    {
        return NULL;
    }

    uint32_t denseIndex = storage->sparse[sparseIndex];
    if (denseIndex >= storage->count || storage->entity[denseIndex] != entity.index)
    {
        return NULL;
    }

    return (uint8_t *)storage->dense + denseIndex * storage->componentSize;
}

uint8_t ASH_HasComponent(
    ASH_Entity entity,
    ASH_ComponentID componentID)
{
    if (componentID >= g_componentRegistry.count)
    {
        return 0;
    }
    _ASH_ComponentStorage *storage = &g_componentRegistry.storages[componentID];

    if (!ASH_IsEntityValid(entity) || entity.index >= ASH_MAX_ENTITIES)
    {
        return 0;
    }

    uint32_t sparseIndex = entity.index;
    if (storage->sparse[sparseIndex] == ASH_INVALID_ENTITY_INDEX)
    {
        return 0;
    }

    uint32_t denseIndex = storage->sparse[sparseIndex];
    if (denseIndex >= storage->count || storage->entity[denseIndex] != entity.index)
    {
        return 0;
    }

    return 1;
}