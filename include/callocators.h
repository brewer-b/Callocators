#pragma once

#include <stddef.h>

// Error codes
typedef enum {
    ALLOC_OK = 0,
    ALLOC_ERR_NULL_POINTER = 1,
    ALLOC_ERR_OUT_OF_MEMORY = 2,
    ALLOC_ERR_INVALID_ALIGNMENT = 3,
    ALLOC_ERR_CAPACITY_EXCEEDED = 4,
    ALLOC_ERR_ZERO_CAPACITY = 5,
} AllocErrorCode;

// Function to create a bump allocator
// Parameters:
//   capacity: The total capacity of the allocator in bytes
//   outAllocator: A pointer to a pointer where the created allocator will be stored
// Returns:
//   ALLOC_OK if the allocator was created successfully, or an error code otherwise
AllocErrorCode bump_allocator_create(size_t capacity, void** outAllocator);

// Function to allocate memory from a bump allocator with alignment requirement
// Parameters:
//   allocator: A pointer to the bump allocator from which memory will be allocated
//   size: The size of the memory block to allocate
//   alignment: The alignment requirement for the memory block (must be a non-zero power of 2)
//   outBlock: A pointer to a pointer where the allocated memory block address will be stored
// Returns:
//   ALLOC_OK if the allocation was successful, or an error code otherwise
AllocErrorCode bump_alloc(void* allocator, size_t size, size_t alignment, void** outBlock);

// Function to reset a bump allocator's offset
// Parameters:
//   allocator: A pointer to the bump allocator to reset
// Returns:
//   ALLOC_OK if the reset was successful, or an error code otherwise
AllocErrorCode bump_allocator_reset(void* allocator);

// Function to destroy a bump allocator and free its resources
// Parameters:
//   allocator: A pointer to the bump allocator to destroy
// Returns:
//   ALLOC_OK if the destruction was successful, or an error code otherwise
AllocErrorCode bump_allocator_destroy(void* allocator);
