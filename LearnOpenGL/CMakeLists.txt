cmake_minimum_required(VERSION 3.5.0)
project(LearnOpenGL VERSION 0.1.0 LANGUAGES C CXX)

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
include_directories(src)

###
set(GLAD "include/glad/glad.h")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")


###############
set(SOURCE_FILES_CHAP1 src/glad.c src/Chap1.GettingStarted/chap1.cpp src/Chap1.GettingStarted/shader/shader.h src/Chap1.GettingStarted/shader/shader.cpp)
add_executable(chap1 ${SOURCE_FILES_CHAP1} ${TINYCTHREAD} ${GETOPT} ${GLAD})
target_link_libraries(chap1 glfw ${GLFW_LIBRARIES})
set_target_properties(chap1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap1)
file(COPY src/Chap1.GettingStarted/shader_source.vs src/Chap1.GettingStarted/shader_source.fs 
     DESTINATION ${CMAKE_BINARY_DIR}/chap1)
