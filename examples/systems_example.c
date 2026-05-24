#include <ash/entities.h>
#include <ash/components.h>
#include <ash/ash_system.h>
#include <stdio.h>

typedef struct
{
    float x, y;
} Position;

typedef struct
{
    float vx, vy;
} Velocity;

void MovementSystem(ASH_Entity entity, float deltaTime)
{
    Position *pos = (Position *)ASH_GetComponent(entity, 0); // Assuming ID 0 is Position
    Velocity *vel = (Velocity *)ASH_GetComponent(entity, 1); // Assuming ID 1 is Velocity

    pos->x += vel->vx * deltaTime;
    pos->y += vel->vy * deltaTime;

    printf("Entity %u moved to: (%.2f, %.2f)\n", entity.index, pos->x, pos->y);
}

int main()
{
    ASH_InitEntityPool();
    ASH_InitComponents();
    ASH_InitSystems();

    ASH_ComponentID posID = ASH_RegisterComponent(sizeof(Position));
    ASH_ComponentID velID = ASH_RegisterComponent(sizeof(Velocity));

    ASH_Entity e1 = ASH_CreateEntity();
    Position p1 = {0, 0};
    Velocity v1 = {1, 1};
    ASH_AddComponent(e1, posID, &p1);
    ASH_AddComponent(e1, velID, &v1);

    ASH_Entity e2 = ASH_CreateEntity();
    Position p2 = {10, 10};
    Velocity v2 = {-1, 0};
    ASH_AddComponent(e2, posID, &p2);
    ASH_AddComponent(e2, velID, &v2);

    ASH_Query moveQuery = ASH_CreateQuery();
    ASH_QueryInclude(&moveQuery, posID);
    ASH_QueryInclude(&moveQuery, velID);

    ASH_RegisterSystem("Movement", moveQuery, MovementSystem, 0);

    printf("Starting System Updates...\n");
    for (int i = 0; i < 5; i++)
    {
        printf("--- Frame %d ---\n", i);
        ASH_UpdateSystems(0.16f);
    }

    return 0;
}
