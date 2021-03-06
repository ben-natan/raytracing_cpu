cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)
project(Raytracing)

find_package(SDL2 REQUIRED)
find_package(OpenMP REQUIRED)


add_subdirectory(external)

file(GLOB_RECURSE RAYTRACING_FILES
    src/*)

include_directories(include)
include_directories(Raytracing ${SDL2_INCLUDE_DIRS})

add_executable(raytracing ${RAYTRACING_FILES})

target_link_libraries(raytracing ${SDL2_LIBRARIES} OpenMP::OpenMP_CXX)

# if (APPLE)
#   target_link_libraries(raytracing "-framework OpenGL")
# endif()

target_include_directories(raytracing
    SYSTEM PUBLIC
    external/sfml/include/
)
