#include "renderer.h"

renderer::renderer() {
    
}

renderer::~renderer() {
    cleanup_render_target();
    m_main_render_target_view.Reset();
    m_p_swap_chain.Reset();
    m_pd3d_device_context.Reset();
    m_pd3d_device.Reset();
}

bool renderer::create_device_d3d(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT create_device_flags = 0;
    D3D_FEATURE_LEVEL feature_level;
    const D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, feature_level_array, 2, D3D11_SDK_VERSION, &sd, &m_p_swap_chain, &m_pd3d_device, &feature_level, &m_pd3d_device_context);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, create_device_flags, feature_level_array, 2, D3D11_SDK_VERSION, &sd, &m_p_swap_chain, &m_pd3d_device, &feature_level, &m_pd3d_device_context);
    if (res != S_OK)
        return false;

    create_render_target();
    return true;
}

void renderer::cleanup_device_d3d() {
    cleanup_render_target();
    m_main_render_target_view.Reset();
    m_p_swap_chain.Reset();
    m_pd3d_device_context.Reset();
    m_pd3d_device.Reset();
}

void renderer::create_render_target() {
    Microsoft::WRL::ComPtr<ID3D11Texture2D> p_back_buffer;
    m_p_swap_chain->GetBuffer(0, IID_PPV_ARGS(&p_back_buffer));
    m_pd3d_device->CreateRenderTargetView(p_back_buffer.Get(), nullptr, &m_main_render_target_view);
}

void renderer::cleanup_render_target() {
    m_main_render_target_view.Reset();
}

void renderer::new_frame() {
    if (m_swap_chain_occluded && m_p_swap_chain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) {
        ::Sleep(10);
        return;
    }
    m_swap_chain_occluded = false;

    if (m_resize_width != 0 && m_resize_height != 0) {
        cleanup_render_target();
        m_p_swap_chain->ResizeBuffers(0, m_resize_width, m_resize_height, DXGI_FORMAT_UNKNOWN, 0);
        m_resize_width = m_resize_height = 0;
        create_render_target();
    }
}

void renderer::render(const ImVec4& clear_color) {
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    m_pd3d_device_context->OMSetRenderTargets(1, m_main_render_target_view.GetAddressOf(), nullptr);
    m_pd3d_device_context->ClearRenderTargetView(m_main_render_target_view.Get(), clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

HRESULT renderer::present() {
    return m_p_swap_chain->Present(1, 0);
}

void renderer::check_occlusion(HRESULT hr) {
    m_swap_chain_occluded = (hr == DXGI_STATUS_OCCLUDED);
}

void renderer::set_resize(UINT width, UINT height) {
    m_resize_width = width;
    m_resize_height = height;
}
