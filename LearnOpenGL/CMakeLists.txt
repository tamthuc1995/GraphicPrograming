cmake_minimum_required(VERSION 3.5.0)
project(hellowork VERSION 0.1.0 LANGUAGES C CXX)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

###
add_subdirectory(lib/glfw)
include_directories(lib/glfw)
include_directories(lib/glfw/deps)
include_directories(lib/glfw/include)

###
include_directories(include)


###
set(GLAD "include/glad/glad.h")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")


###############
set(SOURCE_FILES src/main.cpp src/glad.c)
add_executable(hellowork ${SOURCE_FILES} ${TINYCTHREAD} ${GETOPT} ${GLAD} )
target_link_libraries(hellowork glfw ${GLFW_LIBRARIES})