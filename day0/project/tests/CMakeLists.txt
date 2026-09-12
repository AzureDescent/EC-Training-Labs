add_executable(test_encoder test_encoder.cpp)
target_link_libraries(test_encoder PRIVATE day0_core)
add_test(NAME encoder COMMAND test_encoder)

add_executable(test_clamp test_clamp.cpp)
target_link_libraries(test_clamp PRIVATE day0_core)
add_test(NAME clamp COMMAND test_clamp)
