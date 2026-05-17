#include <stdlib.h>
#include <stdint.h>
#include <ash/internals.h>

_ASH_Pool _ASH_CreatePool(size_t size)
{
    _ASH_Pool pool = {
        malloc(size),
    };

    return pool;
}