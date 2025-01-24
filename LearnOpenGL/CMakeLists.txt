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
set(STB "include/stb/stb_image.h"
        "include/stb/stb_image.cpp")

set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")


###############
set(SOURCE_FILES_CHAP1P4 src/glad.c src/Chap1.GettingStarted/chap1P4.cpp src/Chap1.GettingStarted/shader/shader.h src/Chap1.GettingStarted/shader/shader.cpp)
add_executable(chap1p4 ${SOURCE_FILES_CHAP1P4} ${TINYCTHREAD} ${GETOPT} ${GLAD})
target_link_libraries(chap1p4 glfw ${GLFW_LIBRARIES})
set_target_properties(chap1p4 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap1p4)
file(COPY src/Chap1.GettingStarted/shader_source.vs src/Chap1.GettingStarted/shader_source.fs DESTINATION ${CMAKE_BINARY_DIR}/chap1p4)



###############
set(SOURCE_FILES_CHAP1P5 src/glad.c src/Chap1.GettingStarted/chap1P5.cpp src/Chap1.GettingStarted/shader/shader.h src/Chap1.GettingStarted/shader/shader.cpp)
add_executable(chap1p5 ${SOURCE_FILES_CHAP1P5} ${TINYCTHREAD} ${GETOPT} ${GLAD} ${STB})
target_link_libraries(chap1p5 glfw ${GLFW_LIBRARIES})
set_target_properties(chap1p5 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap1p5)
file(COPY src/Chap1.GettingStarted/shader_source_p5.vs src/Chap1.GettingStarted/shader_source_p5.fs DESTINATION ${CMAKE_BINARY_DIR}/chap1p5)
file(COPY src/Chap1.GettingStarted/materials/container.jpg DESTINATION ${CMAKE_BINARY_DIR}/chap1p5)
