
#ifndef MemoryUtils_h
#define MemoryUtils_h

#include <stdio.h>
#include "Macro.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Tengine_MEMORY_ALIGN_DEFAULT 64

/**
 * @brief alloc memory with given size & alignment.
 * @param size  given size. size should > 0.
 * @param align given alignment.
 * @return memory pointer.
 * @warning use `MemoryFreeAlign` to free returned pointer.
 * @sa MemoryFreeAlign
 */
Tengine_PUBLIC void* MemoryAllocAlign(size_t size, size_t align);

/**
 * @brief alloc memory with given size & alignment, and fill memory space with 0.
 * @param size  given size. size should > 0.
 * @param align given alignment.
 * @return memory pointer.
 * @warning use `MemoryFreeAlign` to free returned pointer.
 * @sa MemoryFreeAlign
 */
Tengine_PUBLIC void* MemoryCallocAlign(size_t size, size_t align);

/**
 * @brief free aligned memory pointer.
 * @param mem   aligned memory pointer.
 * @warning do NOT pass any pointer NOT returned by `MemoryAllocAlign` or `MemoryCallocAlign`.
 * @sa MemoryAllocAlign
 * @sa MemoryCallocAlign
 */
Tengine_PUBLIC void MemoryFreeAlign(void* mem);

#ifdef __cplusplus
}
#endif

#endif /* MemoryUtils_h */
