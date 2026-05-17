#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    void *data;
    size_t size;
    size_t capacity;
} _ASH_Pool;

typedef struct
{
    void *dense;
    uint32_t *entity;
    uint32_t *sparse;

    size_t componentSize;

    uint32_t count;
    uint32_t capacity;
} _ASH_ComponentStorage;

typedef struct
{
    _ASH_ComponentStorage *storages;
    uint32_t count;
    uint32_t capacity;
} _ASH_ComponentRegistry;

_ASH_Pool _ASH_CreatePool(size_t size);
_ASH_ComponentStorage _ASH_CreateComponentStorage(size_t componentSize, uint32_t capacity);
void _ASH_InitComponentRegistry();