add_executable(TestBumpAllocator 
                test_bumpalloc.c)
target_link_libraries(TestBumpAllocator ${PROJECT_NAME})
target_include_directories(TestBumpAllocator PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME TestBumpAllocator COMMAND TestBumpAllocator)