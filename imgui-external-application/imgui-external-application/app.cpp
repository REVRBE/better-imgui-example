#include "app.h"

void app::initialize() {
    m_window = std::make_unique<window>(L"better-imgui-example by REVRBE", 350, 450);

    if (!renderer::get_instance().create_device_d3d(m_window->get_hwnd())) {
        renderer::get_instance().cleanup_device_d3d();
        m_window.reset();
        throw std::runtime_error("Failed to create D3D device.");
    }

    ::ShowWindow(m_window->get_hwnd(), SW_SHOWDEFAULT);
    ::UpdateWindow(m_window->get_hwnd());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = NULL;

    init_imgui_style();
    init_imgui_fonts();

    ImGui_ImplWin32_Init(m_window->get_hwnd());
    ImGui_ImplDX11_Init(renderer::get_instance().get_device(), renderer::get_instance().get_device_context());
}

void app::cleanup() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    renderer::get_instance().cleanup_device_d3d();
}

void app::run() {
    initialize();

    bool done = false;

    std::thread render_thread([&]() {
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (!done) {
            renderer::get_instance().new_frame();

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)m_window->get_width(), (float)m_window->get_height());
            io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
            ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
            ImGui::SetNextWindowSize(io.DisplaySize);

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
                {
                    ImGui::Text("This is some useful text.");

                    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
                    ImGui::ColorEdit3("clear color", (float*)&clear_color);

                    if (ImGui::Button("Button"))
                        counter++;
                    ImGui::SameLine();
                    ImGui::Text("counter = %d", counter);

                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                }
                ImGui::End();
            }

            ImGui::Render();
            renderer::get_instance().render(clear_color);

            HRESULT hr = renderer::get_instance().present();
            renderer::get_instance().check_occlusion(hr);

            std::this_thread::sleep_for(std::chrono::milliseconds(4)); 
        }
        });

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    done = true; 
    render_thread.join(); 

    cleanup();
}

void app::init_imgui_style() { // Initialize your imgui style here, example provided.
    ImGuiStyle& style = ImGui::GetStyle();

    style.AntiAliasedLines = true;
    style.WindowBorderSize = 0.f;

    style.Colors[ImGuiCol_Border] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
}

void app::init_imgui_fonts() { // Initialize your fonts here, example provided.
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig font_config;
    font_config.FontDataOwnedByAtlas = true;
    font_config.OversampleH = 2;
    font_config.OversampleV = 2;

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 15.f, &font_config);
}
