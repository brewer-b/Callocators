#include "callocators.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BumpAllocator {
    uint8_t* start;
    size_t capacity;
    size_t offset;
} BumpAllocator;

AllocErrorCode bump_allocator_create(size_t capacity, void** outAllocator) {
    if (!outAllocator) return ALLOC_ERR_NULL_POINTER;
    if (!capacity) return ALLOC_ERR_ZERO_CAPACITY;
    
    BumpAllocator* allocator = malloc(sizeof(BumpAllocator));
    if (!allocator) return ALLOC_ERR_OUT_OF_MEMORY;
    
    allocator->start = malloc(capacity);
    if (!allocator->start) {
        free(allocator);
        *outAllocator = NULL; // Ensure the pointer is null after freeing.
        return ALLOC_ERR_OUT_OF_MEMORY;
    }
    
    allocator->capacity = capacity;
    allocator->offset = 0;
    *outAllocator = allocator;
    return ALLOC_OK;
}

AllocErrorCode bump_alloc(void* allocator, size_t size, size_t alignment, void** outBlock) {
    if (!allocator || !outBlock) return ALLOC_ERR_NULL_POINTER;
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) return ALLOC_ERR_INVALID_ALIGNMENT;

    BumpAllocator* bumpAllocator = (BumpAllocator*)allocator;
    size_t padding = (alignment - (size_t)(bumpAllocator->start + bumpAllocator->offset) % alignment) % alignment;
    size_t totalSize = size + padding;
    
    if (bumpAllocator->offset + totalSize > bumpAllocator->capacity) {
        *outBlock = NULL;
        return ALLOC_ERR_CAPACITY_EXCEEDED;
    }
    
    *outBlock = bumpAllocator->start + bumpAllocator->offset + padding;
    bumpAllocator->offset += totalSize;
    return ALLOC_OK;
}

AllocErrorCode bump_allocator_reset(void* allocator) {
    if (!allocator) return ALLOC_ERR_NULL_POINTER;
    BumpAllocator* bumpAllocator = (BumpAllocator*)allocator;
    bumpAllocator->offset = 0;
    return ALLOC_OK;
}

AllocErrorCode bump_allocator_destroy(void* allocator) {
    if (!allocator) return ALLOC_ERR_NULL_POINTER;
    BumpAllocator* bumpAllocator = (BumpAllocator*)allocator;
    free(bumpAllocator->start);
    free(bumpAllocator);
    return ALLOC_OK;
}
