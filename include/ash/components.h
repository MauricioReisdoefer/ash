#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <ash/entities.h>
#include <ash/internals.h>

typedef uint32_t ASH_ComponentID;

void ASH_InitComponents();

ASH_ComponentID ASH_RegisterComponent(size_t componentSize);

void ASH_AddComponent(
    ASH_Entity entity,
    ASH_ComponentID componentID,
    void *componentData);

void ASH_RemoveComponent(
    ASH_Entity entity,
    ASH_ComponentID componentID);

void *ASH_GetComponent(
    ASH_Entity entity,
    ASH_ComponentID componentID);

uint8_t ASH_HasComponent(
    ASH_Entity entity,
    ASH_ComponentID componentID);