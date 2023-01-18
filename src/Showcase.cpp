#include "Showcase.h"

/// @brief Creates, Runs & Destroys the Showcase Application.
void ShowcaseApplication::Run()
{
    //If GLFW fails to create a window, then exit the app.
    if(!Init()) return;

    // Initialize ImGui Context.
    InitImGui();
    // Main Render Loop.        
    RenderLoop();
    // Free the memory allocations.
    Cleanup();
}

/// @brief Initializes GLFW & GLAD & Creates a GLFWwindow on success.
/// @return True if Initialization & Window creation was successful.
bool ShowcaseApplication::Init()
{
    // Initialize GLFW & Let GLFW know what version of OpenGL we will use.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Window.
    m_Window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Showcase", NULL, NULL);
    if(m_Window == NULL)
    {
        std::cout << "Failed to create GLFW Window! \n Exiting... \n";
        glfwTerminate();
        return false;
    }

    //Make window the current GLFW context.
    glfwMakeContextCurrent(m_Window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load GLAD.\nExiting...\n";
        glfwTerminate();
        return false;
    }
    glViewport(0, 0, WIDTH, HEIGHT);

    return true;
}

/// @brief Initialize Dear ImGui
void ShowcaseApplication::InitImGui()
{
    // GLSL version ImGui will use.
    const char* glsl_version = "#version 330";

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    SetCustomStyle();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

/// @brief Custom Styling for ImGui
/// Credits: https://github.com/malamanteau
void ShowcaseApplication::SetCustomStyle()
{
	ImGuiStyle & style = ImGui::GetStyle();
	ImVec4 * colors = style.Colors;
		
	colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border]                 = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button]                 = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator]              = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

	style.PopupRounding = 3;

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding  = ImVec2(6, 4);
	style.ItemSpacing   = ImVec2(6, 2);

	style.ScrollbarSize = 18;

	style.WindowBorderSize = 1;
	style.ChildBorderSize  = 1;
	style.PopupBorderSize  = 1;
	style.FrameBorderSize  = 0; 

	style.WindowRounding    = 3;
	style.ChildRounding     = 3;
	style.FrameRounding     = 3;
	style.ScrollbarRounding = 2;
	style.GrabRounding      = 3;
}

/// @brief Main Render Loop.
void ShowcaseApplication::RenderLoop()
{
    while(!glfwWindowShouldClose(m_Window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();
        // Process Custom Input for Keyboard shortcuts.
        ProcessInput();
        // Start New ImGui Frame.
        ImGuiBeginFrame();
        // Render ImGui.
        ImGuiRender();
        // Show the output of ImGui.
        ImGuiEndFrame();
        // Swap Buffers!
        glfwSwapBuffers(m_Window);
    }
}

/// @brief Responsible for the Functionality of Keyboard Shortcuts.
void ShowcaseApplication::ProcessInput()
{
    if(m_PressedOnce)
    {
        m_FramesPassedSinceKeyPressed++;
        if(m_FramesPassedSinceKeyPressed > 15){  m_PressedOnce = false; m_FramesPassedSinceKeyPressed = 0;}
        else return;
    }
    
    if(glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_REPEAT)
        std::cout << "Repeating\n";
    if(glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_PRESS)
    {
        // Pressed F
        // Enter/Exit Fullscreen.
        Fullscreen();
    }
 
    if(glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        SwitchCursorLock();

    if(glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || 
       glfwGetKey(m_Window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        // Ctrl Pressed!
        if(glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_KEY_DOWN) // Ctrl + F
            ShowHideFPS();
        if(glfwGetKey(m_Window, GLFW_KEY_H) == GLFW_PRESS) // Ctrl + H
            ShowHideMenu();
        if(glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) // Ctrl + S
            ShowHideSettings();
        if(glfwGetKey(m_Window, GLFW_KEY_I) == GLFW_PRESS) // Ctrl + I
            OpenCurrentLecture();
        if(glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS) // Ctrl + Q
            Quit();
    }

    m_PressedOnce = true;
}

/// @brief Handles entering & exit of fullscreen mode.
void ShowcaseApplication::Fullscreen()
{
    // Switch Fullscreen!
    m_Fullscreen = !m_Fullscreen;

    if(m_Fullscreen)
    {
        // Set the window to be in Full screen mode.
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }else
    {
        // Reset window.
        glfwSetWindowMonitor(m_Window, nullptr, 0, 0, WIDTH, HEIGHT, REFRESHRATE);
    }

    // Set Context to Current & scale Viewport.
    glfwMakeContextCurrent(m_Window);
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
}

void ShowcaseApplication::SwitchCursorLock()
{
}

void ShowcaseApplication::ShowHideFPS()
{
}

void ShowcaseApplication::ShowHideMenu()
{
    m_MenuVisible = !m_MenuVisible;
}

void ShowcaseApplication::ShowHideSettings()
{
}

void ShowcaseApplication::OpenCurrentLecture()
{
}

void ShowcaseApplication::Quit()
{
}

/// @brief Creates a New ImGui Frame.
void ShowcaseApplication::ImGuiBeginFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

/// @brief Renders all the stuff we send to ImGui
void ShowcaseApplication::ImGuiRender()
{
    static bool s=true;
    ImGui::ShowDemoWindow(&s);

    if (ImGui::BeginMainMenuBar())
    {
        #pragma region Options

        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Enter/Exit Fullscreen", "F"))
                Fullscreen();

            if (ImGui::MenuItem("Show/Hide FPS", "Ctrl+F"))
                std::cout << "Show/Hide FPS\n";

            if (ImGui::MenuItem("Show/Hide Menu", "Ctrl+H"))
                ShowHideMenu();

            if (ImGui::MenuItem("Show/Hide Settings", "Ctrl+S"))
                std::cout << "Show/Hide Settings\n";

            if (ImGui::MenuItem("Show/Hide Cursor", "RMB"))
                std::cout << "Show/Hide Cursor\n";

            if (ImGui::MenuItem("Open Current Lecture", "Ctrl+I"))
                std::cout << "Open Current Lecture\n";

            ImGui::Separator();

            if (ImGui::MenuItem("Quit", "Ctrl+Q")) 
                std::cout << "Quit\n";

            ImGui::EndMenu();
        }

        #pragma endregion

        #pragma region Sections

        if (ImGui::BeginMenu("Sections"))
        {
            if (ImGui::BeginMenu("Getting Started"))
            {
                if (ImGui::MenuItem("Hello Window")) {}
                if (ImGui::MenuItem("Hello Triangle")) {}
                if (ImGui::MenuItem("Shaders")) {}
                if (ImGui::MenuItem("Textures")) {}
                if (ImGui::MenuItem("Transformations")) {}
                if (ImGui::MenuItem("Coordinate Systems")) {}
                if (ImGui::MenuItem("Camera")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Lighting"))
            {
                if (ImGui::MenuItem("Colors")) {}
                if (ImGui::MenuItem("Basic Lighting")) {}
                if (ImGui::MenuItem("Materials")) {}
                if (ImGui::MenuItem("Lighting Maps")) {}
                if (ImGui::MenuItem("Light Casters")) {}
                if (ImGui::MenuItem("Multiple lights")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Model Loading"))
            {
                if (ImGui::MenuItem("Assimp")) {}
                if (ImGui::MenuItem("Mesh")) {}
                if (ImGui::MenuItem("Model")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Advanced OpenGL"))
            {
                if (ImGui::MenuItem("Depth testing")) {}
                if (ImGui::MenuItem("Stencil testing")) {}
                if (ImGui::MenuItem("Blending")) {}
                if (ImGui::MenuItem("Face culling")) {}
                if (ImGui::MenuItem("Framebuffers")) {}
                if (ImGui::MenuItem("Cubemaps")) {}
                if (ImGui::MenuItem("Advanced Data")) {}
                if (ImGui::MenuItem("Advanced GLSL")) {}
                if (ImGui::MenuItem("Geometry Shader")) {}
                if (ImGui::MenuItem("Instancing")) {}
                if (ImGui::MenuItem("Anti Aliasing")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Advanced Lighting"))
            {
                if (ImGui::MenuItem("Advanced lighting")) {}
                if (ImGui::MenuItem("Gamma Correction")) {}
                if (ImGui::BeginMenu("Shadows")) 
                {
                    if (ImGui::MenuItem("Shadow Mapping")) {}
                    if (ImGui::MenuItem("Point Shadows")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Normal Mapping")) {}
                if (ImGui::MenuItem("Parallax Mapping")) {}
                if (ImGui::MenuItem("HDR")) {}
                if (ImGui::MenuItem("Bloom")) {}
                if (ImGui::MenuItem("Deferred Shading")) {}
                if (ImGui::MenuItem("SSAO")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("PBR"))
            {
                if (ImGui::MenuItem("Lighting")) {}
                if (ImGui::BeginMenu("IBL")) 
                {
                    if (ImGui::MenuItem("Diffuse irradiance")) {}
                    if (ImGui::MenuItem("Specular IBL")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("In Practice"))
            {
                if (ImGui::MenuItem("Debugging")) {}
                if (ImGui::MenuItem("Text Rendering")) {}
                if (ImGui::MenuItem("2D Game")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Guest Articles"))
            {
                if (ImGui::BeginMenu("2020")) 
                {
                    if (ImGui::BeginMenu("OIT")) 
                    {
                        if (ImGui::MenuItem("Introduction")) {}
                        if (ImGui::MenuItem("Weight Blended")) {}
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("Skeletal Animation")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("2021")) 
                {
                    if (ImGui::MenuItem("CSM")) {}
                    if (ImGui::BeginMenu("Scene")) 
                    {
                        if (ImGui::MenuItem("Scene Graph")) {}
                        if (ImGui::MenuItem("Frustum Culling")) {}
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Tessellation")) 
                    {
                        if (ImGui::MenuItem("Height map")) {}
                        if (ImGui::MenuItem("Tessellation")) {}
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("DSA")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("2022")) 
                {
                    if (ImGui::BeginMenu("Compute Shaders")) 
                    {
                        if (ImGui::MenuItem("Introduction")) {}
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("Phys. Based Bloom")) {}
                    if (ImGui::MenuItem("Area Lights")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        #pragma endregion

        ImGui::EndMainMenuBar();
    }
}

/// @brief Marks the end of an ImGui Frame.
void ShowcaseApplication::ImGuiEndFrame()
{
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// @brief Free all the Memory allocations after their use.
void ShowcaseApplication::Cleanup()
{
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup GLFW window & Context.
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

int main()
{
    ShowcaseApplication app;
    try 
    {
        app.Run();
    } catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}