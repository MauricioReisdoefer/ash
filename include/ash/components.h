#pragma once

#include <stdint.h>
#include <ash/entities.h>

#define ASH_MAX_COMPONENTS 33024

typedef struct ASH_Component
{
    void *pool;
    size_t componentSize;
    uint8_t active;
    uint8_t entityHasComponent[ASH_MAX_ENTITIES];
} ASH_Component;

uint32_t ASH_RegisterComponent(size_t size);
void *ASH_AddComponent(uint32_t componentId, ASH_Entity entity);
uint8_t ASH_HasComponent(uint32_t componentId, ASH_Entity entity);
void *ASH_GetComponent(uint32_t componentId, ASH_Entity entity);
void ASH_RemoveComponent(uint32_t componentId, ASH_Entity entity);