cmake_minimum_required(VERSION 3.17)
project(GameProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
    headers/*.h
    #external/imgui-1.88/imgui.cpp
    #external/imgui-1.88/imgui_draw.cpp
    #external/imgui-1.88/imgui_widgets.cpp
    #external/imgui-1.88/imgui_tables.cpp
    #external/imgui-1.88/backends/imgui_impl_sdl.cpp
    #external/imgui-1.88/backends/imgui_impl_opengl3.cpp
    #external/imgui-sfml-2.5/imgui-SFML.cpp
    #external/imgui-1.88/imgui_demo.cpp
    
)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add executable directory
add_executable(GameProject ${SOURCES})

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning the entire build directory"
)

# Include paths
include_directories(
    headers
    C:/dev/SFML-2.5.1/include
    ${CMAKE_SOURCE_DIR}/external/tgui/include
    ${CMAKE_SOURCE_DIR}/external/imgui-1.88
    ${CMAKE_SOURCE_DIR}/external/imgui-1.88/backends
    ${CMAKE_SOURCE_DIR}/external/imgui-sfml-2.5
)


# Link SFML and TGUI dynamically
target_link_directories(GameProject PUBLIC
    C:/dev/SFML-2.5.1/lib
    ${CMAKE_SOURCE_DIR}/external/tgui/lib
)

find_package(OpenGL REQUIRED)

target_link_libraries(GameProject
    ${CMAKE_SOURCE_DIR}/external/tgui/bin/tgui.dll
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    OpenGL::GL
)

# Copy assets to bin directory
file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Copy DLLs for runtime
if(WIN32)
    file(GLOB SFML_DLLS "C:/dev/SFML-2.5.1/bin/*.dll")
    file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    file(GLOB TGUI_DLLS "${CMAKE_SOURCE_DIR}/external/tgui/lib/*.dll")
    file(COPY ${TGUI_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    file(GLOB MINGW_DLLS "C:/MinGW/bin/*.dll")
    file(COPY ${MINGW_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()