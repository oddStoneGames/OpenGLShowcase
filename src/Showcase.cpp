#include "Showcase.h"
#include "Lectures/GettingStarted/Camera/Camera.h"
#include <stb_image.h>

/// @brief Creates, Runs & Destroys the Showcase Application.
void ShowcaseApplication::Run()
{
    //If GLFW fails to create a window, then exit the app.
    if(!Init()) return;
    // Initialize ImGui Context.
    InitImGui();
    // Initialize Lectures
    Lectures::CreateInstance();
    Lectures::m_Instance->m_Lectures.push_back(new GettingStarted::HelloWindow());
    Lectures::m_Instance->m_Lectures.push_back(new GettingStarted::HelloTriangle());
    Lectures::m_Instance->m_Lectures.push_back(new GettingStarted::ShaderLecture());
    Lectures::m_Instance->m_Lectures.push_back(new GettingStarted::TextureLecture());
    Lectures::m_Instance->m_Lectures.push_back(new GettingStarted::Transformations());
    int w, h; glfwGetFramebufferSize(m_Window, &w, &h);
    Lectures::m_Instance->m_Lectures.push_back(new GettingStarted::CoordinateSystem(w, h));
    Lectures::m_Instance->m_Lectures.push_back(new GettingStarted::CameraLecture(w, h));

    Lectures::m_Instance->m_Lectures.push_back(new Lighting::ColorLecture(w, h));
    Lectures::m_Instance->m_Lectures.push_back(new Lighting::BasicLighting(w, h));
    Lectures::m_Instance->m_Lectures.push_back(new Lighting::Materials(w, h));
    Lectures::m_Instance->m_Lectures.push_back(new Lighting::LightingMaps(w, h));
    Lectures::m_Instance->m_Lectures.push_back(new Lighting::LightCasters(w, h));
    Lectures::m_Instance->m_Lectures.push_back(new Lighting::MultipleLights(w, h));

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

    // Make window the current GLFW context.
    glfwMakeContextCurrent(m_Window);

    // Set Mouse Button Callback
    GLFWCallbackWrapper::SetApplication(this);
    // Set Mouse Button Callback
    glfwSetMouseButtonCallback(m_Window, GLFWCallbackWrapper::MouseButtonCallback);
    // Set Mouse Move Callback
    glfwSetCursorPosCallback(m_Window, GLFWCallbackWrapper::MouseMoveCallback);
    // Set Mouse Scroll Callback
    glfwSetScrollCallback(m_Window, GLFWCallbackWrapper::MouseScrollCallback);
    // Set Key Callback
    glfwSetKeyCallback(m_Window, GLFWCallbackWrapper::KeyCallback);
    // Set Window Resize Callback
    glfwSetWindowSizeCallback(m_Window, GLFWCallbackWrapper::ResizeCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load GLAD.\nExiting...\n";
        glfwTerminate();
        return false;
    }

    // Set Window Icon.
    GLFWimage icon[1]; 
    icon[0].pixels = stbi_load(PROJECT_DIR"/src/Assets/Textures/icon.png", &icon[0].width, &icon[0].height, 0, 4);
    glfwSetWindowIcon(m_Window, 1, icon); 
    stbi_image_free(icon[0].pixels);

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
        // Get Per frame based DeltaTime.
        float currentFrame = static_cast<float>(glfwGetTime());
        m_DeltaTime = currentFrame - m_LastTime;
        m_LastTime = currentFrame;

        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();
        ProcessInput();

        // Start New ImGui Frame.
        ImGuiBeginFrame();
        // Render ImGui.
        ImGuiRender();
        // Render Current Lecture.
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        Lectures::m_Instance->RenderLecture(m_CurrentLectureIndex, m_SettingsVisible, width, height);
        // Show the output of ImGui.
        ImGuiEndFrame();
        // Swap Buffers!
        glfwSwapBuffers(m_Window);
    }
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
        // Non zero value for offsets to make sure that the title bar can render on windows.
        glfwSetWindowMonitor(m_Window, nullptr, 20, 40, WIDTH, HEIGHT, REFRESHRATE);
    }

    // Set Context to Current & scale Viewport.
    glfwMakeContextCurrent(m_Window);
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
}

/// @brief Switches Cursor Lock State.
void ShowcaseApplication::SwitchCursorLock()
{
    m_CursorLocked = !m_CursorLocked;
    if(m_CursorLocked)
    {
        // Set Cursor position to be in the middle of the screen.
        int display_w, display_h;
        glfwGetFramebufferSize(m_Window, &display_w, &display_h);
        glfwSetCursorPos(m_Window, display_w * 0.5f, display_h * 0.5f);
    }
    glfwSetInputMode(m_Window, GLFW_CURSOR, m_CursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void ShowcaseApplication::ShowHideFPS()
{
    m_FPSVisible = !m_FPSVisible;
}

void ShowcaseApplication::ShowHideMenu()
{
    m_MenuVisible = !m_MenuVisible;
}

void ShowcaseApplication::ShowHideSettings()
{
    m_SettingsVisible = !m_SettingsVisible;
}

void ShowcaseApplication::Quit()
{
    // Set Window to Close.
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

/// @brief Creates a New ImGui Frame.
void ShowcaseApplication::ImGuiBeginFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// @brief Renders all the stuff we send to ImGui
void ShowcaseApplication::ImGuiRender()
{

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{12.0f, 6.5f});
    
    if (m_MenuVisible && ImGui::BeginMainMenuBar())
    {
        #pragma region Options

        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Enter/Exit Fullscreen", "F"))
                Fullscreen();

            if (ImGui::MenuItem("Show/Hide FPS", "Ctrl+F"))
                ShowHideFPS();

            if (ImGui::MenuItem("Show/Hide Menu", "Ctrl+H"))
                ShowHideMenu();

            if (ImGui::MenuItem("Show/Hide Settings", "Ctrl+S"))
                ShowHideSettings();

            if (ImGui::MenuItem("Show/Hide Cursor", "RMB"))
                SwitchCursorLock();

            if (ImGui::MenuItem("Open Current Lecture", "Ctrl+I"))
                Lectures::m_Instance->OpenLectureLink(m_CurrentLectureIndex);

            ImGui::Separator();

            if (ImGui::MenuItem("Quit", "Ctrl+Q")) 
                Quit();
            
            ImGui::EndMenu();
        }

        #pragma endregion

        #pragma region Sections

        if (ImGui::BeginMenu("Sections"))
        {
            if (ImGui::BeginMenu("Getting Started"))
            {
                if (ImGui::MenuItem("Hello Window"))        { m_CurrentLectureIndex = 0; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Hello Triangle"))      { m_CurrentLectureIndex = 1; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Shaders"))             { m_CurrentLectureIndex = 2; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Textures"))            { m_CurrentLectureIndex = 3; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Transformations"))     { m_CurrentLectureIndex = 4; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Coordinate Systems"))  { m_CurrentLectureIndex = 5; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Camera"))              { m_CurrentLectureIndex = 6; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Lighting"))
            {
                if (ImGui::MenuItem("Colors"))              { m_CurrentLectureIndex = 7; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Basic Lighting"))      { m_CurrentLectureIndex = 8; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Materials"))           { m_CurrentLectureIndex = 9; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Lighting Maps"))       { m_CurrentLectureIndex = 10; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Light Casters"))       { m_CurrentLectureIndex = 11; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Multiple lights"))     { m_CurrentLectureIndex = 12; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Model Loading")) 
            { 
                m_CurrentLectureIndex = 13;
                Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex);
            }

            if (ImGui::BeginMenu("Advanced OpenGL"))
            {
                if (ImGui::MenuItem("Depth testing"))       { m_CurrentLectureIndex = 14; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Stencil testing"))     { m_CurrentLectureIndex = 15; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Blending"))            { m_CurrentLectureIndex = 16; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Face culling"))        { m_CurrentLectureIndex = 17; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Framebuffers"))        { m_CurrentLectureIndex = 18; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Cubemaps"))            { m_CurrentLectureIndex = 19; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Advanced Data"))       { m_CurrentLectureIndex = 20; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Advanced GLSL"))       { m_CurrentLectureIndex = 21; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Geometry Shader"))     { m_CurrentLectureIndex = 22; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Instancing"))          { m_CurrentLectureIndex = 23; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Anti Aliasing"))       { m_CurrentLectureIndex = 24; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Advanced Lighting"))
            {
                if (ImGui::MenuItem("Advanced lighting"))   { m_CurrentLectureIndex = 25; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); } 
                if (ImGui::MenuItem("Gamma Correction"))    { m_CurrentLectureIndex = 26; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::BeginMenu("Shadows")) 
                {
                    if (ImGui::MenuItem("Shadow Mapping"))  { m_CurrentLectureIndex = 27; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); } 
                    if (ImGui::MenuItem("Point Shadows"))   { m_CurrentLectureIndex = 28; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Normal Mapping"))      { m_CurrentLectureIndex = 29; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Parallax Mapping"))    { m_CurrentLectureIndex = 30; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("HDR"))                 { m_CurrentLectureIndex = 31; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Bloom"))               { m_CurrentLectureIndex = 32; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Deferred Shading"))    { m_CurrentLectureIndex = 33; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("SSAO"))                { m_CurrentLectureIndex = 34; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("PBR"))
            {
                if (ImGui::MenuItem("Lighting"))            { m_CurrentLectureIndex = 35; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::BeginMenu("IBL")) 
                {
                    if (ImGui::MenuItem("Diffuse irradiance"))  { m_CurrentLectureIndex = 36; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                    if (ImGui::MenuItem("Specular IBL"))        { m_CurrentLectureIndex = 37; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("In Practice"))
            {
                if (ImGui::MenuItem("Debugging"))           { m_CurrentLectureIndex = 38; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("Text Rendering"))      { m_CurrentLectureIndex = 39; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                if (ImGui::MenuItem("2D Game"))             { m_CurrentLectureIndex = 40; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Guest Articles"))
            {
                if (ImGui::BeginMenu("2020")) 
                {
                    if (ImGui::BeginMenu("OIT")) 
                    {
                        if (ImGui::MenuItem("Introduction"))    { m_CurrentLectureIndex = 41; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); } 
                        if (ImGui::MenuItem("Weight Blended"))  { m_CurrentLectureIndex = 42; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("Skeletal Animation"))  { m_CurrentLectureIndex = 43; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("2021")) 
                {
                    if (ImGui::MenuItem("CSM"))                 { m_CurrentLectureIndex = 44; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                    if (ImGui::BeginMenu("Scene")) 
                    {
                        if (ImGui::MenuItem("Scene Graph"))     { m_CurrentLectureIndex = 45; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                        if (ImGui::MenuItem("Frustum Culling")) { m_CurrentLectureIndex = 46; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Tessellation")) 
                    {
                        if (ImGui::MenuItem("Height map"))      { m_CurrentLectureIndex = 47; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                        if (ImGui::MenuItem("Tessellation"))    { m_CurrentLectureIndex = 48; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("DSA"))                 { m_CurrentLectureIndex = 49; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("2022")) 
                {
                    if (ImGui::BeginMenu("Compute Shaders")) 
                    {
                        if (ImGui::MenuItem("Introduction"))    { m_CurrentLectureIndex = 50; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("Phys. Based Bloom"))   { m_CurrentLectureIndex = 51; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                    if (ImGui::MenuItem("Area Lights"))         { m_CurrentLectureIndex = 52; Lectures::m_Instance->OpenLecture(m_CurrentLectureIndex); }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        #pragma endregion

        // Show FPS
        if(m_FPSVisible)
        {
            ImGui::SameLine();
            // Width of FPS
            float labelWidth = ImGui::CalcTextSize("FPS: 999.99").x;
            // Set Cursor Position to be at the end of screen minus width of fps label.
            ImGui::SetCursorScreenPos(ImVec2{ImGui::GetWindowWidth() - labelWidth, 0.0f});
            ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleVar();
}

/// @brief Marks the end of an ImGui Frame.
void ShowcaseApplication::ImGuiEndFrame()
{
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
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

/// @brief Mouse Click Callback from GLFW
void ShowcaseApplication::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    // Switch Cursor Lock state on right click press.
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        SwitchCursorLock();
}

void ShowcaseApplication::MouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    float xposF = static_cast<float>(xpos);
    float yposF = static_cast<float>(ypos);

    if(m_CursorLocked)
    {
        if (m_FirstMouse)
        {
            m_LastX = xposF;
            m_LastY = yposF;
            m_FirstMouse = false;
        }

        float xoffset = xposF - m_LastX;
        float yoffset = m_LastY - yposF; // reversed since y-coordinates go from bottom to top

        GettingStarted::Camera::GetInstance()->ProcessMouseMovement(xoffset, yoffset);
    }
    else
    {
        m_FirstMouse = true;
    }

    m_LastX = xposF;
    m_LastY = yposF;
}

void ShowcaseApplication::MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    GettingStarted::Camera::GetInstance()->ProcessMouseScroll(static_cast<float>(yoffset));
}

/// @brief Responsible for the Functionality of Keyboard Shortcuts.
void ShowcaseApplication::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_F && action == GLFW_PRESS && mods != GLFW_MOD_CONTROL)
        Fullscreen();   // Pressed F->Enter/Exit Fullscreen.
 
    if(mods == GLFW_MOD_CONTROL)
    {
        // Ctrl Pressed!
        if(key == GLFW_KEY_F && action == GLFW_PRESS) // Ctrl + F
            ShowHideFPS();
        if(key == GLFW_KEY_H && action == GLFW_PRESS) // Ctrl + H
            ShowHideMenu();
        if(key == GLFW_KEY_S && action == GLFW_PRESS) // Ctrl + S
            ShowHideSettings();
        if(key == GLFW_KEY_I && action == GLFW_PRESS) // Ctrl + I
            Lectures::m_Instance->OpenLectureLink(m_CurrentLectureIndex);
        if(key == GLFW_KEY_Q && action == GLFW_PRESS) // Ctrl + Q
            Quit();
    }
}

void ShowcaseApplication::ResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ShowcaseApplication::ProcessInput()
{
    // Send Camera Keyboard Input.
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        GettingStarted::Camera::GetInstance()->ProcessKeyboard(GettingStarted::Camera_Movement::FORWARD, m_DeltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        GettingStarted::Camera::GetInstance()->ProcessKeyboard(GettingStarted::Camera_Movement::BACKWARD, m_DeltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        GettingStarted::Camera::GetInstance()->ProcessKeyboard(GettingStarted::Camera_Movement::LEFT, m_DeltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        GettingStarted::Camera::GetInstance()->ProcessKeyboard(GettingStarted::Camera_Movement::RIGHT, m_DeltaTime);
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

ShowcaseApplication* ShowcaseApplication::GLFWCallbackWrapper::s_application = nullptr;

void ShowcaseApplication::GLFWCallbackWrapper::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    s_application->MouseButtonCallback(window, button, action, mods);
}

void ShowcaseApplication::GLFWCallbackWrapper::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    s_application->KeyCallback(window, key, scancode, action, mods);
}

void ShowcaseApplication::GLFWCallbackWrapper::MouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    s_application->MouseMoveCallback(window, xpos, ypos);
}

void ShowcaseApplication::GLFWCallbackWrapper::MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    s_application->MouseScrollCallback(window, xoffset, yoffset);
}

void ShowcaseApplication::GLFWCallbackWrapper::ResizeCallback(GLFWwindow* window, int width, int height)
{
    s_application->ResizeCallback(window, width, height);
}

void ShowcaseApplication::GLFWCallbackWrapper::SetApplication(ShowcaseApplication *application)
{
    GLFWCallbackWrapper::s_application = application;
}