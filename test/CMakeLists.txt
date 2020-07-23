
cmake_minimum_required (VERSION 3.17)

find_package (doctest REQUIRED)
find_package (rapidcheck REQUIRED)

set (t_ clang-sanitizers)
    add_library (${t_} INTERFACE)
    if (CMAKE_CXX_COMPILER_ID MATCHES "^Clang$")
        target_compile_options (${t_} INTERFACE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
        target_link_options (${t_} INTERFACE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
    endif ()

set (t_ test-clockwork-base32)
    add_executable (${t_} main.cpp base32.cpp)
    target_link_libraries (${t_} PRIVATE doctest::doctest rapidcheck::rapidcheck clang-sanitizers)
    target_link_libraries (${t_} PRIVATE clockwork-base32)
    target_compile_definitions (${t_} PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS=1)
