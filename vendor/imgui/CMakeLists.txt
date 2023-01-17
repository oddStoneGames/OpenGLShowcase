add_library(
        imgui STATIC
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
        imgui_internal.h
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        backends/imgui_impl_opengl3.cpp
        backends/imgui_impl_opengl3.h
        backends/imgui_impl_opengl3_loader.h
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_glfw.h
)

target_include_directories(
        imgui
        PUBLIC
        imgui
        imgui/backends
)