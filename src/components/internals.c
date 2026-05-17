#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ash/internals.h>
#include <ash/entities.h>

static _ASH_ComponentRegistry g_componentRegistry;

_ASH_Pool _ASH_CreatePool(size_t size)
{
    _ASH_Pool pool;

    pool.size = size;
    pool.capacity = size;
    pool.data = malloc(size);

    if (!pool.data)
    {
        printf("[ASH] Pool allocation failed\n");
    }

    return pool;
}

_ASH_ComponentStorage _ASH_CreateComponentStorage(size_t componentSize, uint32_t capacity)
{
    _ASH_ComponentStorage storage;

    storage.capacity = capacity;
    storage.count = 0;

    storage.dense = malloc(componentSize * capacity);
    storage.entity = malloc(sizeof(uint32_t) * capacity);
    storage.sparse = malloc(sizeof(uint32_t) * ASH_MAX_ENTITIES);

    if (!storage.dense || !storage.entity || !storage.sparse)
    {
        printf("[ASH] ComponentStorage allocation failed\n");
    }

    for (uint32_t i = 0; i < capacity; i++)
    {
        storage.entity[i] = ASH_INVALID_ENTITY_INDEX;
    }

    for (uint32_t i = 0; i < ASH_MAX_ENTITIES; i++)
    {
        storage.sparse[i] = ASH_INVALID_ENTITY_INDEX;
    }

    return storage;
}

void _ASH_InitComponentRegistry()
{
    g_componentRegistry.count = 0;
    g_componentRegistry.capacity = _ASH_MAX_COMPONENTS;

    g_componentRegistry.storages =
        malloc(sizeof(_ASH_ComponentStorage) * g_componentRegistry.capacity);

    if (!g_componentRegistry.storages)
    {
        printf("[ASH] Registry allocation failed\n");
    }

    for (uint32_t i = 0; i < g_componentRegistry.capacity; i++)
    {
        g_componentRegistry.storages[i].dense = NULL;
        g_componentRegistry.storages[i].entity = NULL;
        g_componentRegistry.storages[i].sparse = NULL;
        g_componentRegistry.storages[i].count = 0;
        g_componentRegistry.storages[i].capacity = 0;
        g_componentRegistry.storages[i].componentSize = 0;
    }
}