

#include <stdint.h>
#include <stdlib.h>
#include "MemoryUtils.h"
#include "Macro.h"
static inline void **alignPointer(void **ptr, size_t alignment) {
    return (void **)((intptr_t)((unsigned char *)ptr + alignment - 1) & -alignment);
}

extern "C" void *MemoryAllocAlign(size_t size, size_t alignment) {
    Tengine_ASSERT(size > 0);

#ifdef Tengine_DEBUG_MEMORY
    return malloc(size);
#else
    void **origin = (void **)malloc(size + sizeof(void *) + alignment);
    Tengine_ASSERT(origin != NULL);
    if (!origin) {
        return NULL;
    }

    void **aligned = alignPointer(origin + 1, alignment);
    aligned[-1]    = origin;
    return aligned;
#endif
}

extern "C" void *MemoryCallocAlign(size_t size, size_t alignment) {
    Tengine_ASSERT(size > 0);

#ifdef Tengine_DEBUG_MEMORY
    return calloc(size, 1);
#else
    void **origin = (void **)calloc(size + sizeof(void *) + alignment, 1);
    Tengine_ASSERT(origin != NULL)
    if (!origin) {
        return NULL;
    }
    void **aligned = alignPointer(origin + 1, alignment);
    aligned[-1]    = origin;
    return aligned;
#endif
}

extern "C" void MemoryFreeAlign(void *aligned) {
#ifdef Tengine_DEBUG_MEMORY
    free(aligned);
#else
    if (aligned) {
        void *origin = ((void **)aligned)[-1];
        free(origin);
    }
#endif
}
