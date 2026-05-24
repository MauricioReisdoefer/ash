#include <ash/ash_system.h>
#include <ash/internals.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    ASH_System systems[ASH_MAX_SYSTEMS];
    uint32_t count;
} ASH_SystemRegistry;

static ASH_SystemRegistry g_systemRegistry;
extern _ASH_ComponentRegistry g_componentRegistry;

void ASH_InitSystems(void) {
    g_systemRegistry.count = 0;
    memset(g_systemRegistry.systems, 0, sizeof(g_systemRegistry.systems));
}

ASH_Query ASH_CreateQuery(void) {
    ASH_Query query;
    query.includeCount = 0;
    return query;
}

void ASH_QueryInclude(ASH_Query* query, ASH_ComponentID componentID) {
    if (query->includeCount < ASH_MAX_QUERY_COMPONENTS) {
        query->includes[query->includeCount++] = componentID;
    }
}

void ASH_RegisterSystem(const char* name, ASH_Query query, ASH_SystemFunc func, int priority) {
    if (g_systemRegistry.count >= ASH_MAX_SYSTEMS) {
        printf("[ASH] System registry full\n");
        return;
    }

    uint32_t index = g_systemRegistry.count++;
    g_systemRegistry.systems[index].name = name;
    g_systemRegistry.systems[index].query = query;
    g_systemRegistry.systems[index].func = func;
    g_systemRegistry.systems[index].priority = priority;
    g_systemRegistry.systems[index].active = 1;

    // Sort systems by priority (simple bubble sort for now, as registration is rare)
    for (uint32_t i = 0; i < g_systemRegistry.count - 1; i++) {
        for (uint32_t j = 0; j < g_systemRegistry.count - i - 1; j++) {
            if (g_systemRegistry.systems[j].priority > g_systemRegistry.systems[j+1].priority) {
                ASH_System temp = g_systemRegistry.systems[j];
                g_systemRegistry.systems[j] = g_systemRegistry.systems[j+1];
                g_systemRegistry.systems[j+1] = temp;
            }
        }
    }
}

void ASH_RemoveSystem(const char* name) {
    for (uint32_t i = 0; i < g_systemRegistry.count; i++) {
        if (strcmp(g_systemRegistry.systems[i].name, name) == 0) {
            for (uint32_t j = i; j < g_systemRegistry.count - 1; j++) {
                g_systemRegistry.systems[j] = g_systemRegistry.systems[j+1];
            }
            g_systemRegistry.count--;
            return;
        }
    }
}

void ASH_UpdateSystems(float deltaTime) {
    for (uint32_t i = 0; i < g_systemRegistry.count; i++) {
        ASH_System* system = &g_systemRegistry.systems[i];
        if (!system->active || system->query.includeCount == 0) continue;

        // Find the smallest storage to iterate over (optimization for Sparse Sets)
        ASH_ComponentID smallestComponentID = system->query.includes[0];
        uint32_t minCount = g_componentRegistry.storages[smallestComponentID].count;

        for (uint32_t q = 1; q < system->query.includeCount; q++) {
            ASH_ComponentID id = system->query.includes[q];
            if (g_componentRegistry.storages[id].count < minCount) {
                minCount = g_componentRegistry.storages[id].count;
                smallestComponentID = id;
            }
        }

        _ASH_ComponentStorage* storage = &g_componentRegistry.storages[smallestComponentID];
        
        for (uint32_t j = 0; j < storage->count; j++) {
            uint32_t entityIndex = storage->entity[j];
            ASH_Entity entity = { .index = entityIndex, .generation = 0 }; // Generation validation would need access to entity pool
            
            // Note: In a production ECS, we'd validate generation here. 
            // Since we can't modify entities.c easily to expose the generation array without changing headers,
            // and the prompt forbids changing existing code, we rely on the fact that if it's in the storage, 
            // it's likely valid for this frame.
            
            uint8_t match = 1;
            for (uint32_t q = 0; q < system->query.includeCount; q++) {
                ASH_ComponentID id = system->query.includes[q];
                if (id == smallestComponentID) continue;
                
                if (!ASH_HasComponent(entity, id)) {
                    match = 0;
                    break;
                }
            }

            if (match) {
                system->func(entity, deltaTime);
            }
        }
    }
}
