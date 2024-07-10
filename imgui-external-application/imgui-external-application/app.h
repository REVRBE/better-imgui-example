#ifndef APP_H
#define APP_H

#include "pch.h"
#include "window.h"
#include "renderer.h"

class app {
public:
    static app& get_instance() {
        static app instance;
        return instance;
    }

    void run();

private:
    app() = default;
    ~app() = default;

    app(const app&) = delete;
    app& operator=(const app&) = delete;

    std::unique_ptr<window> m_window;
    std::unique_ptr<renderer> m_renderer;

    void initialize();
    void cleanup();

    void init_imgui_style();
    void init_imgui_fonts();
};

#endif // APP_H
