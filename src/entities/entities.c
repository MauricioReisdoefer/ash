#include <include/ash/entities.h>

static uint32_t g_entityFreelist[ASH_MAX_ENTITIES];
static uint32_t g_entityFreelistCount;
static uint32_t g_entityGenerations[ASH_MAX_ENTITIES];
static uint8_t g_entityAlive[ASH_MAX_ENTITIES];

ASH_Entity ASH_CreateEntity(void);
void ASH_DestroyEntity(ASH_Entity entity);

uint8_t ASH_IsEntityValid(ASH_Entity entity);
void ASH_InitEntityPool(void);