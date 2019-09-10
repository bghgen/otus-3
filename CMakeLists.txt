cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 0.0.1)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(PROJECT_VERSION_PATCH $ENV{TRAVIS_BUILD_NUMBER})
endif (DEFINED ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(allocator allocator.cpp)

if (MSVC)
    target_compile_options(allocator PRIVATE
        /W4
    )
else ()
    target_compile_options(allocator PRIVATE
#        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT trypic@gmail.com)

include(CPack)
