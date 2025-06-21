# CMakeLists.txt (Aggressive Version)
cmake_minimum_required(VERSION 3.10)
project(hire_me CXX)

set(CMAKE_EXECUTABLE_SUFFIX ".js")

# --- Force Emscripten flags at the global level ---
# This is aggressive, but ensures no sub-project can override it.
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FULL_ES3=1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1")

# --- BGFX Configuration ---
set(BGFX_CONFIG_RENDERER_OPENGLES 30 CACHE STRING "Force BGFX to use the OpenGLES 3.0+ renderer")

# Add the visage library
add_subdirectory(/Users/skyler/Desktop/Crank/visage ${CMAKE_BINARY_DIR}/visage_build)

# Define our main application executable
add_executable(hire_me_executable src/main.cpp)

# Tell our main executable where to find the header files
target_include_directories(hire_me_executable PRIVATE /Users/skyler/Desktop/Crank/visage)

# Define the base name and output directory
set_target_properties(hire_me_executable PROPERTIES
    RUNTIME_OUTPUT_NAME "hire_me_nvidia"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/web"
)

# Link against required libraries.
target_link_libraries(hire_me_executable PRIVATE
    visage
    html5
    GL
)

# We keep this as well for modern CMake best-practice, even though it's now redundant.
target_link_options(hire_me_executable PRIVATE
    "-s ALLOW_MEMORY_GROWTH=1"
    "-s FULL_ES3=1"
)