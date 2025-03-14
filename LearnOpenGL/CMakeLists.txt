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
file(COPY src/Chap1.GettingStarted/shader_source_p5.vs src/Chap1.GettingStarted/shader_source_p5.fs DESTINATION ${CMAKE_BINARY_DIR}/chap1p5)
file(COPY src/Chap1.GettingStarted/materials/container.jpg src/Chap1.GettingStarted/materials/awesomeface.png DESTINATION ${CMAKE_BINARY_DIR}/chap1p5)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap1.GettingStarted/shader_source_p5.vs ${CMAKE_BINARY_DIR}/chap1p5/shader_source_p5.vs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap1.GettingStarted/shader_source_p5.fs ${CMAKE_BINARY_DIR}/chap1p5/shader_source_p5.fs)
add_executable(chap1p5 ${SOURCE_FILES_CHAP1P5} ${TINYCTHREAD} ${GETOPT} ${GLAD} ${STB})
target_link_libraries(chap1p5 glfw ${GLFW_LIBRARIES})
set_target_properties(chap1p5 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap1p5)


###############
set(SOURCE_FILES_CHAP1P6 src/glad.c src/Chap1.GettingStarted/chap1P6.cpp src/Chap1.GettingStarted/shader/shader.h src/Chap1.GettingStarted/shader/shader.cpp)
file(COPY src/Chap1.GettingStarted/shader_source_p6.vs src/Chap1.GettingStarted/shader_source_p6.fs DESTINATION ${CMAKE_BINARY_DIR}/chap1p6)
file(COPY src/Chap1.GettingStarted/materials/container.jpg src/Chap1.GettingStarted/materials/awesomeface.png DESTINATION ${CMAKE_BINARY_DIR}/chap1p6)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap1.GettingStarted/shader_source_p6.vs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p6.vs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap1.GettingStarted/shader_source_p6.fs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p6.fs)
add_executable(chap1p6 ${SOURCE_FILES_CHAP1P6} ${TINYCTHREAD} ${GETOPT} ${GLAD} ${STB})
target_link_libraries(chap1p6 glfw ${GLFW_LIBRARIES})
set_target_properties(chap1p6 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap1p6)

###############
set(SOURCE_FILES_CHAP1P7 src/glad.c src/Chap1.GettingStarted/chap1P7.cpp src/Chap1.GettingStarted/shader/shader.h src/Chap1.GettingStarted/shader/shader.cpp)
file(COPY src/Chap1.GettingStarted/shader_source_p7.vs src/Chap1.GettingStarted/shader_source_p7.fs DESTINATION ${CMAKE_BINARY_DIR}/chap1p7)
file(COPY src/Chap1.GettingStarted/materials/container.jpg src/Chap1.GettingStarted/materials/awesomeface.png DESTINATION ${CMAKE_BINARY_DIR}/chap1p7)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap1.GettingStarted/shader_source_p7.vs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p7.vs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap1.GettingStarted/shader_source_p7.fs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p7.fs)
add_executable(chap1p7 ${SOURCE_FILES_CHAP1P7} ${TINYCTHREAD} ${GETOPT} ${GLAD} ${STB})
target_link_libraries(chap1p7 glfw ${GLFW_LIBRARIES})
set_target_properties(chap1p7 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap1p7)


###############
set(SOURCE_FILES_CHAP1P8 
        src/glad.c 
        src/Chap1.GettingStarted/chap1P8.cpp 
        src/Chap1.GettingStarted/shader/shader.h 
        src/Chap1.GettingStarted/shader/shader.cpp 
        src/Chap1.GettingStarted/camera/camera.h 
        src/Chap1.GettingStarted/camera/camera.cpp
)
file(COPY src/Chap1.GettingStarted/shader_source_p8.vs src/Chap1.GettingStarted/shader_source_p8.fs DESTINATION ${CMAKE_BINARY_DIR}/chap1p8)
file(COPY src/Chap1.GettingStarted/materials/container.jpg src/Chap1.GettingStarted/materials/awesomeface.png DESTINATION ${CMAKE_BINARY_DIR}/chap1p8)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap1.GettingStarted/shader_source_p8.vs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p8.vs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap1.GettingStarted/shader_source_p8.fs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p8.fs)
add_executable(chap1p8 ${SOURCE_FILES_CHAP1P8} ${TINYCTHREAD} ${GETOPT} ${GLAD} ${STB})
target_link_libraries(chap1p8 glfw ${GLFW_LIBRARIES})
set_target_properties(chap1p8 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap1p8)


###############
set(SOURCE_FILES_CHAP2P01 
        src/glad.c 
        src/Chap2.Light/chap2p01.cpp 
        src/Chap2.Light/shader/shader.h 
        src/Chap2.Light/shader/shader.cpp 
        src/Chap2.Light/camera/camera.h 
        src/Chap2.Light/camera/camera.cpp
)
file(COPY src/Chap2.Light/shader_light_source_p1.vs src/Chap2.Light/shader_light_source_p1.fs DESTINATION ${CMAKE_BINARY_DIR}/chap2p01)
file(COPY src/Chap2.Light/shader_source_p1.vs src/Chap2.Light/shader_source_p1.fs DESTINATION ${CMAKE_BINARY_DIR}/chap2p01)
file(COPY src/Chap2.Light/materials/container.jpg src/Chap2.Light/materials/container2.png src/Chap2.Light/materials/container2_specular.png src/Chap2.Light/materials/awesomeface.png DESTINATION ${CMAKE_BINARY_DIR}/chap2p01)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap2.Light/shader_light_source_p1.vs ${CMAKE_BINARY_DIR}/chap1p6/shader_light_source_p1.vs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap2.Light/shader_light_source_p1.fs ${CMAKE_BINARY_DIR}/chap1p6/shader_light_source_p1.fs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap2.Light/shader_source_p1.vs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p1.vs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap2.Light/shader_source_p1.fs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p1.fs)
add_executable(chap2p01 ${SOURCE_FILES_CHAP2P01} ${TINYCTHREAD} ${GETOPT} ${GLAD} ${STB})
target_link_libraries(chap2p01 glfw ${GLFW_LIBRARIES})
set_target_properties(chap2p01 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap2p01)



###############
set(SOURCE_FILES_CHAP2P02 
        src/glad.c 
        src/Chap2.Light/chap2p02.cpp 
        src/Chap2.Light/shader/shader.h 
        src/Chap2.Light/shader/shader.cpp 
        src/Chap2.Light/camera/camera.h 
        src/Chap2.Light/camera/camera.cpp
)
file(COPY src/Chap2.Light/shader_light_source_p2.vs src/Chap2.Light/shader_light_source_p2.fs DESTINATION ${CMAKE_BINARY_DIR}/chap2p02)
file(COPY src/Chap2.Light/shader_source_p2.vs src/Chap2.Light/shader_source_p2.fs DESTINATION ${CMAKE_BINARY_DIR}/chap2p02)
file(COPY src/Chap2.Light/materials/container.jpg src/Chap2.Light/materials/container2.png src/Chap2.Light/materials/container2_specular.png src/Chap2.Light/materials/awesomeface.png DESTINATION ${CMAKE_BINARY_DIR}/chap2p02)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap2.Light/shader_light_source_p2.vs ${CMAKE_BINARY_DIR}/chap1p6/shader_light_source_p2.vs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap2.Light/shader_light_source_p2.fs ${CMAKE_BINARY_DIR}/chap1p6/shader_light_source_p2.fs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap2.Light/shader_source_p2.vs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p2.vs)
configure_file(${CMAKE_SOURCE_DIR}/src/Chap2.Light/shader_source_p2.fs ${CMAKE_BINARY_DIR}/chap1p6/shader_source_p2.fs)
add_executable(chap2p02 ${SOURCE_FILES_CHAP2P02} ${TINYCTHREAD} ${GETOPT} ${GLAD} ${STB})
target_link_libraries(chap2p02 glfw ${GLFW_LIBRARIES})
set_target_properties(chap2p02 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/chap2p02)