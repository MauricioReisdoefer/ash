#pragma once

#include <ash/entities.h>
#include <ash/components.h>

#define ASH_MAX_QUERY_COMPONENTS 32
#define ASH_MAX_SYSTEMS 128

typedef struct ASH_Query {
    ASH_ComponentID includes[ASH_MAX_QUERY_COMPONENTS];
    uint32_t includeCount;
} ASH_Query;

typedef void (*ASH_SystemFunc)(ASH_Entity entity, float deltaTime);

typedef struct ASH_System {
    const char* name;
    ASH_Query query;
    ASH_SystemFunc func;
    int priority;
    uint8_t active;
} ASH_System;

void ASH_InitSystems(void);
ASH_Query ASH_CreateQuery(void);
void ASH_QueryInclude(ASH_Query* query, ASH_ComponentID componentID);

void ASH_RegisterSystem(const char* name, ASH_Query query, ASH_SystemFunc func, int priority);
void ASH_UpdateSystems(float deltaTime);
void ASH_RemoveSystem(const char* name);
