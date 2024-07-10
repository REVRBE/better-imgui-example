#include "window.h"
#include "renderer.h"

#include "resource.h"

window::window(const std::wstring& title, int width, int height) {
    register_window_class();

    center_window();

    m_hwnd = ::CreateWindowW(m_wc.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, m_wc.hInstance, nullptr);
    if (!m_hwnd) {
        throw std::runtime_error("Failed to create window.");
    }

    set_dark_mode(true);
}

window::~window() {
    ::DestroyWindow(m_hwnd);
    ::UnregisterClassW(m_wc.lpszClassName, m_wc.hInstance);
}

void window::register_window_class() {
    m_wc = { sizeof(m_wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr),
             LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1)), 
             LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, L"better-imgui-example by REVRBE",
             nullptr };

    m_wc.hIconSm = (HICON)LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0); 

    ::RegisterClassExW(&m_wc);
}

void window::center_window() {
    RECT desktop_rect;
    GetClientRect(GetDesktopWindow(), &desktop_rect);

    int screen_width = desktop_rect.right;
    int screen_height = desktop_rect.bottom;

    int window_width = get_width();
    int window_height = get_height();

    int posX = (screen_width - window_width) / 2;
    int posY = (screen_height - window_height) / 2;

    ::SetWindowPos(m_hwnd, nullptr, posX, posY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void window::set_dark_mode(BOOL enable) {
    DwmSetWindowAttribute(m_hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &enable, sizeof(enable));
}

int window::get_width() const {
    RECT rect;
    ::GetClientRect(m_hwnd, &rect);
    return rect.right - rect.left;
}

int window::get_height() const {
    RECT rect;
    ::GetClientRect(m_hwnd, &rect);
    return rect.bottom - rect.top;
}

LRESULT CALLBACK window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    renderer& renderer = renderer::get_instance();

    switch (msg) {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        renderer.set_resize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hwnd, msg, wParam, lParam);
}
