#ifndef WINDOW_H
#define WINDOW_H

#include "pch.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class window {
public:
    window(const std::wstring& title, int width, int height);
    ~window();

    HWND get_hwnd() const { return m_hwnd; }
    int get_width() const;
    int get_height() const;

private:
    HWND m_hwnd;
    WNDCLASSEXW m_wc;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void register_window_class();
    void center_window();
    void set_dark_mode(BOOL enable);
};

#endif // WINDOW_H
