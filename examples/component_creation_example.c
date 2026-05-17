// Component_Creation_Example.c

#include <stdio.h>
#include <ash/components.h>
#include <ash/entities.h>

typedef struct Transform
{
    float x;
    float y;
} Transform;

int main()
{
    ASH_InitEntityPool();
    uint32_t transformId = ASH_RegisterComponent(sizeof(Transform));

    ASH_Entity entity = ASH_CreateEntity();

    Transform *t = (Transform *)ASH_AddComponent(transformId, entity);
    t->x = 10.0f;
    t->y = 20.0f;
    printf("Transform inicial: %.2f, %.2f\n", t->x, t->y);

    if (ASH_HasComponent(transformId, entity))
    {
        printf("Entity possui Transform!\n");
    }

    Transform *t2 = (Transform *)ASH_GetComponent(transformId, entity);
    if (t2)
    {
        printf("GetComponent: %.2f, %.2f\n", t2->x, t2->y);
    }

    ASH_RemoveComponent(transformId, entity);
    if (!ASH_HasComponent(transformId, entity))
    {
        printf("Transform removido da entidade!\n");
    }
    return 0;
}