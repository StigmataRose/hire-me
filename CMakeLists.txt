# CMakeLists.txt for hire_me
cmake_minimum_required(VERSION 3.10)
project(hire_me CXX)

set(CMAKE_EXECUTABLE_SUFFIX ".js")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FULL_ES3=1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1")

set(BGFX_CONFIG_RENDERER_OPENGLES 30 CACHE STRING "Force BGFX to use the OpenGLES 3.0+ renderer")

# add_subdirectory(/Users/skyler/Desktop/Crank/visage ${CMAKE_BINARY_DIR}/visage_build)

add_subdirectory(${CMAKE_SOURCE_DIR}/visage ${CMAKE_BINARY_DIR}/visage_build)
add_executable(hire_me_executable src/main.cpp)

target_include_directories(hire_me_executable PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    # Visage's generated headers are here:
    ${CMAKE_BINARY_DIR}/visage_build/visage_graphics/VisageEmbeddedFonts_generated
    ${CMAKE_BINARY_DIR}/visage_build/visage_graphics/VisageEmbeddedShaders_generated
    ${CMAKE_BINARY_DIR}/visage_build/visage_graphics/VisageEmbeddedIcons_generated
)

set_target_properties(hire_me_executable PROPERTIES
    RUNTIME_OUTPUT_NAME "hire_me_nvidia"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/web"
)

target_link_libraries(hire_me_executable PRIVATE
    visage # This should bring in VisageGraphics and its dependencies, and *their* public include directories
    html5
    GL
)

target_link_options(hire_me_executable PRIVATE
    "-s ALLOW_MEMORY_GROWTH=1"
    "-s FULL_ES3=1"
)