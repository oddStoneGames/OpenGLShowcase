cmake_minimum_required(VERSION 3.25.1)

project(OpenGL-Showcase)

set(CMAKE_CXX_STANDARD 11)

#OPEN GL
find_package(OpenGL REQUIRED)
list(APPEND EXTRA_INCLUDES ${OPENGL_INCLUDE_DIR})
list(APPEND EXTRA_LIBS ${OPENGL_LIBRARIES})

#GLFW
find_package(glfw3 REQUIRED)
list(APPEND EXTRA_INCLUDES vendor/glfw/include)
list(APPEND EXTRA_LIBS ${GLFW_LIBRARY_DIRS})

#GLAD
add_subdirectory(vendor/glad)
list(APPEND EXTRA_LIBS glad)

#IMGUI
add_subdirectory(vendor/imgui)
list(APPEND EXTRA_LIBS imgui)

set(SOURCE_FILES src/Showcase.cpp src/Showcase.h src/Lectures.cpp src/Lectures.h src/Lectures/HelloWindow.cpp src/Lectures/HelloWindow.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS} glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor)