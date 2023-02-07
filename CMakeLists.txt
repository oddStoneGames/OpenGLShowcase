cmake_minimum_required(VERSION 3.25.1)

project(OpenGL-Showcase)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OPENGL
find_package(OpenGL REQUIRED)
list(APPEND EXTRA_INCLUDES ${OPENGL_INCLUDE_DIR})
list(APPEND EXTRA_LIBS ${OPENGL_LIBRARIES})

# GLFW
find_package(glfw3 REQUIRED)
list(APPEND EXTRA_INCLUDES vendor/glfw/include)
list(APPEND EXTRA_LIBS ${GLFW_LIBRARY_DIRS})

# GLAD
add_subdirectory(vendor/glad)
list(APPEND EXTRA_LIBS glad)

# IMGUI
add_subdirectory(vendor/imgui)
list(APPEND EXTRA_LIBS imgui)

# STB_IMAGE
list(APPEND EXTRA_INCLUDES vendor/stb)

# GLM
list(APPEND EXTRA_INCLUDES vendor/glm)

set(SOURCE_FILES    src/Showcase.cpp src/Showcase.h 
                    src/Lectures.cpp src/Lectures.h 
                    src/Lectures/GettingStarted/HelloWindow/HelloWindow.cpp     src/Lectures/GettingStarted/HelloWindow/HelloWindow.h
                    src/Lectures/GettingStarted/HelloTriangle/HelloTriangle.cpp src/Lectures/GettingStarted/HelloTriangle/HelloTriangle.h
                    src/Lectures/GettingStarted/Shader/Shader.h
                    src/Lectures/GettingStarted/Shader/ShaderLecture.cpp src/Lectures/GettingStarted/Shader/ShaderLecture.h
                    src/Lectures/GettingStarted/Texture/TextureLecture.cpp src/Lectures/GettingStarted/Texture/TextureLecture.h
                    src/Lectures/GettingStarted/Transformations/Transformations.cpp src/Lectures/GettingStarted/Transformations/Transformations.h
                    src/Lectures/GettingStarted/CoordinateSystem/CoordinateSystem.cpp src/Lectures/GettingStarted/CoordinateSystem/CoordinateSystem.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# PRECOMPILED HEADER
target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.h)
target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_DIR="${PROJECT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS} glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor)