#pragma once

#include <stdint.h>

#define ASH_MAX_ENTITIES 16512
#define ASH_INVALID_ENTITY_INDEX UINT32_MAX

typedef struct ASH_Entity
{
    uint32_t index;
    uint32_t generation;
} ASH_Entity;

ASH_Entity ASH_CreateEntity(void);
void ASH_DestroyEntity(ASH_Entity entity);

uint8_t ASH_IsEntityValid(ASH_Entity entity);
void ASH_InitEntityPool(void);