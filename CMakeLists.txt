cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(OpenGL-Showcase)

# OPENGL
find_package(OpenGL REQUIRED)
list(APPEND INCLUDES ${OPENGL_INCLUDE_DIR})
list(APPEND LIBS ${OPENGL_LIBRARIES})

# GLFW
add_subdirectory(vendor/glfw)
list(APPEND LIBS glfw)

# GLAD
add_subdirectory(vendor/glad)
list(APPEND LIBS glad)

# IMGUI
add_subdirectory(vendor/imgui)
list(APPEND LIBS imgui)

# STB_IMAGE
list(APPEND INCLUDES vendor/stb)

# GLM
list(APPEND INCLUDES vendor/glm)

# Add extra libraries based on the operating system.
if(WIN32)
    list(APPEND gdi32 user32)
else()
    list(APPEND LIBS X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor)
endif()

set(SOURCE_FILES    src/Showcase.cpp src/Showcase.h 
                    src/Lectures.cpp src/Lectures.h

                    src/Lectures/GettingStarted/HelloWindow/HelloWindow.cpp     src/Lectures/GettingStarted/HelloWindow/HelloWindow.h
                    src/Lectures/GettingStarted/HelloTriangle/HelloTriangle.cpp src/Lectures/GettingStarted/HelloTriangle/HelloTriangle.h
                    src/Lectures/GettingStarted/Shader/Shader.h
                    src/Lectures/GettingStarted/Shader/ShaderLecture.cpp src/Lectures/GettingStarted/Shader/ShaderLecture.h
                    src/Lectures/GettingStarted/Texture/TextureLecture.cpp src/Lectures/GettingStarted/Texture/TextureLecture.h
                    src/Lectures/GettingStarted/Transformations/Transformations.cpp src/Lectures/GettingStarted/Transformations/Transformations.h
                    src/Lectures/GettingStarted/CoordinateSystem/CoordinateSystem.cpp src/Lectures/GettingStarted/CoordinateSystem/CoordinateSystem.h
                    src/Lectures/GettingStarted/Camera/Camera.h src/Lectures/GettingStarted/Camera/Camera.cpp
                    src/Lectures/GettingStarted/Camera/CameraLecture.cpp src/Lectures/GettingStarted/Camera/CameraLecture.h

                    src/Lectures/Lighting/Colors/ColorLecture.cpp src/Lectures/Lighting/Colors/ColorLecture.h
                    src/Lectures/Lighting/BasicLighting/BasicLighting.cpp src/Lectures/Lighting/BasicLighting/BasicLighting.h
                    src/Lectures/Lighting/Materials/Materials.cpp src/Lectures/Lighting/Materials/Materials.h
                    src/Lectures/Lighting/LightingMaps/LightingMaps.cpp src/Lectures/Lighting/LightingMaps/LightingMaps.h
                    src/Lectures/Lighting/LightCasters/LightCasters.cpp src/Lectures/Lighting/LightCasters/LightCasters.h
                    src/Lectures/Lighting/MultipleLights/MultipleLights.cpp src/Lectures/Lighting/MultipleLights/MultipleLights.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Set this project as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# PRECOMPILED HEADER
target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.h)
target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_DIR="${PROJECT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})