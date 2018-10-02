#include "xalloc.h"
#include "error.h"

void *xmalloc(size_t size)
{
    void *result = malloc(size);
    if (!result)
    {
        error("could not allocate memory.");
    } else {
        return result;
    }
}