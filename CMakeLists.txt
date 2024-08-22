cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(RmlUi_SFML_tutorial)

add_executable(RmlUi_SFML_tutorial main.cpp
    backend/RmlUi_Backend_SFML_GL2.h backend/RmlUi_Backend.h backend/RmlUi_Include_Windows.h
    backend/RmlUi_Platform_SFML.cpp backend/RmlUi_Platform_SFML.h backend/RmlUi_Renderer_GL2.cpp
    backend/RmlUi_Renderer_GL2.h)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(RmlUi CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(RmlUi_SFML_tutorial PRIVATE
    sfml-system sfml-window sfml-graphics
    RmlCore RmlDebugger
    OpenGL::GL)