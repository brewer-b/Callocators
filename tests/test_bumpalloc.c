#include "minunit.h"
#include "callocators.h"
#include <stdint.h>

MU_TEST(test_create_normal) {
    void *allocator;
    AllocErrorCode result = bump_allocator_create(1024, &allocator);
    mu_assert_int_eq(ALLOC_OK, result);
    mu_assert(allocator, "Allocator should not be NULL");
    bump_allocator_destroy(allocator);
}

MU_TEST(test_create_zero_capacity) {
    void *allocator;
    AllocErrorCode result = bump_allocator_create(0, &allocator);
    mu_assert_int_eq(ALLOC_ERR_ZERO_CAPACITY, result);
}

MU_TEST(test_alloc_normal) {
    void *allocator;
    bump_allocator_create(1024, &allocator);
    void *block;
    AllocErrorCode result = bump_alloc(allocator, 256, 16, &block);
    mu_assert_int_eq(ALLOC_OK, result);
    mu_assert(block, "Block should not be NULL");
    mu_assert(((uintptr_t)block % 16) == 0, "Block should be aligned");
    bump_allocator_destroy(allocator);
}

MU_TEST(test_alloc_exceed_capacity) {
    void *allocator;
    bump_allocator_create(1024, &allocator); 
    void *first_block;
    AllocErrorCode first_result = bump_alloc(allocator, 256, 16, &first_block);
    mu_assert_int_eq(ALLOC_OK, first_result);
    void *second_block;
    AllocErrorCode second_result = bump_alloc(allocator, 800, 16, &second_block);
    mu_assert_int_eq(ALLOC_ERR_CAPACITY_EXCEEDED, second_result);
    bump_allocator_destroy(allocator);
}


MU_TEST(test_alloc_invalid_alignment) {
    void *allocator;
    bump_allocator_create(1024, &allocator);
    void *block;
    AllocErrorCode result = bump_alloc(allocator, 256, 3, &block);
    mu_assert_int_eq(ALLOC_ERR_INVALID_ALIGNMENT, result);
    bump_allocator_destroy(allocator);
}

MU_TEST(test_reset) {
    void* allocator;
    bump_allocator_create(1024, &allocator);
    void* block_before_reset;
    AllocErrorCode result_before_reset = bump_alloc(allocator, 1000, 1, &block_before_reset);
    mu_assert_int_eq(ALLOC_OK, result_before_reset);
    AllocErrorCode reset_result = bump_allocator_reset(allocator);
    mu_assert_int_eq(ALLOC_OK, reset_result);
    void* block_after_reset;
    AllocErrorCode result_after_reset = bump_alloc(allocator, 1000, 1, &block_after_reset);
    mu_assert_int_eq(ALLOC_OK, result_after_reset);
    mu_assert(block_before_reset == block_after_reset, "Block addresses should match if reset was successful");
    bump_allocator_destroy(allocator);
}

MU_TEST_SUITE(bump_alloc_suite) {
    MU_RUN_TEST(test_create_normal);
    MU_RUN_TEST(test_create_zero_capacity);
    MU_RUN_TEST(test_alloc_normal);
    MU_RUN_TEST(test_alloc_exceed_capacity);
    MU_RUN_TEST(test_alloc_invalid_alignment);
    MU_RUN_TEST(test_reset);
}

int main() {
    MU_RUN_SUITE(bump_alloc_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
