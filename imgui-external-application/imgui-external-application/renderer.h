#ifndef RENDERER_H
#define RENDERER_H

#include "pch.h"

class renderer {
public:
    static renderer& get_instance() {
        static renderer instance;
        return instance;
    }

    bool create_device_d3d(HWND hwnd);
    void cleanup_device_d3d();
    void new_frame();
    void render(const ImVec4& clear_color);
    HRESULT present();
    void check_occlusion(HRESULT hr);
    void set_resize(UINT width, UINT height);

    ID3D11Device* get_device() const { return m_pd3d_device.Get(); }
    ID3D11DeviceContext* get_device_context() const { return m_pd3d_device_context.Get(); }

    ~renderer();

private:
    renderer();
    renderer(const renderer&) = delete;
    renderer& operator=(const renderer&) = delete;

    void create_render_target();
    void cleanup_render_target();

    Microsoft::WRL::ComPtr<ID3D11Device> m_pd3d_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pd3d_device_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_p_swap_chain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_main_render_target_view;
    bool m_swap_chain_occluded = false;
    UINT m_resize_width = 0;
    UINT m_resize_height = 0;
};

#endif // RENDERER_H
