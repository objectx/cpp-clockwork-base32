
cmake_minimum_required (VERSION 3.17)

project (cpp-clockwork-base32 VERSION 3.0.0)

if (ENABLE_TESTS)
    include (${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif ()

set (t_ clockwork-base32)
    add_library (${t_} INTERFACE)
    target_include_directories (${t_} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_sources (${t_} INTERFACE include/clockwork-base32.hpp)
    target_compile_features (${t_} INTERFACE cxx_std_17)

if (ENABLE_TESTS)
    function (setvar_default_ var_)
        if (NOT DEFINED ${var_})
            set (${var_} ${ARGN} PARENT_SCOPE)
        endif ()
    endfunction ()

    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    add_subdirectory (test)
endif ()

