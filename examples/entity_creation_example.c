#include <stdio.h>

#include <ash/entities.h>

int main(void)
{
    ASH_InitEntityPool();

    printf("=== ENTITY CREATION ===\n");

    ASH_Entity player = ASH_CreateEntity();

    printf(
        "Player created -> index: %u | generation: %u\n",
        player.index,
        player.generation);

    printf(
        "Player valid? %u\n",
        ASH_IsEntityValid(player));

    printf("\n=== ENTITY DESTRUCTION ===\n");

    ASH_DestroyEntity(player);

    printf(
        "Player valid after destroy? %u\n",
        ASH_IsEntityValid(player));

    printf("\n=== ENTITY REUSE ===\n");

    ASH_Entity zombie = ASH_CreateEntity();

    printf(
        "Zombie created -> index: %u | generation: %u\n",
        zombie.index,
        zombie.generation);

    printf("\n=== COMPARISON ===\n");

    printf(
        "Same index reused? %s\n",
        player.index == zombie.index ? "YES" : "NO");

    printf(
        "Different generation? %s\n",
        player.generation != zombie.generation ? "YES" : "NO");

    printf(
        "\nOld player handle still valid? %u\n",
        ASH_IsEntityValid(player));

    printf(
        "Zombie handle valid? %u\n",
        ASH_IsEntityValid(zombie));

    return 0;
}